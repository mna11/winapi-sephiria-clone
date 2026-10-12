#include "pch.h"
#include "CBossStage.h"

#include "CCamera.h"
#include "CPlayer.h"
#include "CGolem.h"
#include "CErma.h"
#include "CMonster.h"
#include "CFade.h"
#include "CSpeechBubble.h"

#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include "CTimeMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"
#include "CUIMgr.h"
#include "CSceneMgr.h"
#include "CEffectMgr.h"
#include "CSoundMgr.h"

CBossStage::CBossStage()
	: CState(BOSS_STAGE_STATE::END, BOSS_STAGE_STATE::WAIT),
	m_pErma(nullptr), m_pGolem(nullptr),
	m_dIntroTime(0.), m_eIntroState(BOSS_STAGE_INTRO_STATE::END),
	m_dFinalTime(0.), m_eFinalState(BOSS_STAGE_FINAL_STATE::END)
{
}

CBossStage::~CBossStage()
{
	Release();
}

void CBossStage::Initialize()
{
	CTileMgr::GetInstance()->LoadTile(SCENEID::BOSS_STAGE);
	CTileMgr::GetInstance()->SetInteractionBlocked(false);

	InitializeRooms();
	Init_CreateObj();
    Init_LoadImg(L"../Resource/Image/Stage/BossStage.png");
	Init_BGM(L"DugeonLibrary_Field.wav", 0.3f);
}

void CBossStage::Update()
{
	ApplyChange();
	UpdateTime();

	CObjMgr::GetInstance()->Update();
}

void CBossStage::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();

	if (m_eCurState == BOSS_STAGE_STATE::WAIT ||
		m_eCurState == BOSS_STAGE_STATE::CLEAR)
	{
		HotKey();
	}

	HandleCollisionBattleRoom();
	HandleCollisionStair();
	EndBattle();
}

void CBossStage::Render(Graphics* pGraphics)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
	VEC vViewStart = vScroll * -1;

	int srcLeft = max(0, vViewStart.fX);
	int srcTop = max(0, vViewStart.fY);
	int srcRight = min(TILECX * TILEX, vViewStart.fX + WINCX);
	int srcBottom = min(TILECY * TILEY, vViewStart.fY + WINCY);

	int copyWidth = srcRight - srcLeft;
	int copyHeight = srcBottom - srcTop;

	int dstX = srcLeft - vViewStart.fX;
	int dstY = srcTop - vViewStart.fY;

	HDC hBackDC = pGraphics->GetHDC();

	TransparentBlt(
		hBackDC,
		dstX,
		dstY,
		copyWidth,
		copyHeight,
		m_hMapDC,
		srcLeft,
		srcTop,
		copyWidth,
		copyHeight,
		RGB(255, 0, 255)
	);

	pGraphics->ReleaseHDC(hBackDC);

#ifdef _DEBUG

	Pen roomPen(Color(255, 255, 0, 0), 2.f);

	for (const ROOM_INFO& room : m_vecRooms)
	{
		const RECT& rc = room.rcTrigger;

		pGraphics->DrawRectangle(
			&roomPen,
			RectF(
				rc.left + vScroll.fX,
				rc.top + vScroll.fY,
				static_cast<float>(rc.right - rc.left),
				static_cast<float>(rc.bottom - rc.top)
			)
		);
	}

	pGraphics->DrawRectangle(
		&roomPen,
		RectF(
			m_rcStair.left + vScroll.fX,
			m_rcStair.top + vScroll.fY,
			static_cast<float>(m_rcStair.right - m_rcStair.left),
			static_cast<float>(m_rcStair.bottom - m_rcStair.top)
		)
	);
#endif // _DEBUG


    // ReleaseHDC 이후부터 GDI+ 사용
    CTileMgr::GetInstance()->Render(pGraphics);
    CObjMgr::GetInstance()->Render(pGraphics);
}

void CBossStage::Release()
{
}

void CBossStage::Init_CreateObj()
{
	if (nullptr == CObjMgr::GetInstance()->GetPlayer())
		CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(3000.f, 1500.f));
	else
		CObjMgr::GetInstance()->GetPlayer()->SetPos({412.f, 1024.f});

	CCameraMgr::GetInstance()->SetCameraTarget(CObjMgr::GetInstance()->GetPlayer());

	// 보스 추가하기
	m_pGolem = static_cast<CGolem*>(CAbstractFactory<CGolem>::CreateObj(3960.f, 1050.f));
	m_pErma = static_cast<CErma*>(CAbstractFactory<CErma>::CreateObj(3950.f, 1605.f));
	CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pGolem);
	CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pErma);
	m_pGolem->InitializeParts();
	m_pGolem->SetErma(m_pErma);
}

void CBossStage::InitializeRooms()
{
	m_vecRooms.clear();

	ROOM_INFO tBossRoom;
	tBossRoom.rcTrigger = { 3500, 1200, 4500, 2000 };

	m_vecRooms.push_back(tBossRoom);

	SetRect(&m_rcStair, 5100, 4300, 5700, 4700);
}

void CBossStage::SpawnMonster(int iRoomIdx)
{
}

void CBossStage::HandleCollisionBattleRoom()
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	// 현재 싸우고 있는 상태가 아니라면
	if (-1 == m_iCurBattleRoomIdx)
	{
		// 방을 순회한다.
		for (int i = 0; i < m_vecRooms.size(); ++i)
		{
			ROOM_INFO& room = m_vecRooms[i];

			// 이미 클리어 한 방이라면 다른 방 순회
			if (room.eState == ROOM_STATE::CLEAR)
				continue;

			// 만약 준비중인 방과 트리거가 충돌이 된다면
			if (CCollisionMgr::CollisionRect(pPlayer->GetRect(), room.rcTrigger))
			{
				// 인트로 상태로 전환
				StartBattleRoom(i);
				RequestChange(BOSS_STAGE_STATE::INTRO);
				break;
			}
		}
	}
	else
	{
		// 전투 중인 상황인데, 현재 몬스터를 다 죽였다면 방을 클리어 상태로 변경한다.
		if (CObjMgr::GetInstance()->ObjEmpty(OBJID::MONSTER))
			ClearCurRoom();
	}
}

void CBossStage::HandleCollisionStair()
{
	if (CCollisionMgr::CollisionRect(CObjMgr::GetInstance()->GetPlayer()->GetRect(), m_rcStair) && KEY_DOWN('F'))
	{
		CSceneMgr::GetInstance()->RequestChange(SCENEID::END);
	}
}

void CBossStage::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case BOSS_STAGE_STATE::WAIT:
			break;
		case BOSS_STAGE_STATE::INTRO:
		{
			CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
			if (pPlayer != nullptr)
			{
				// 플레이어 조작 불가
				pPlayer->RequestChange(PLAYER_STATE::IDLE);
				pPlayer->SetPlayerBehaviorEnable(false);

				// 카메라 이동 및 인트로 상태 변경
				CCameraMgr::GetInstance()->MoveCamera({3950.f, 1605.f});
				m_eIntroState = BOSS_STAGE_INTRO_STATE::CAMERA_MOVE;

				// UI는 다 닫고, 레터 박스만 열리게 구현
				CUIMgr::GetInstance()->HideAllUI();
				CUIMgr::GetInstance()->ShowUI(UIID::LETTER_BOX);
			}
			break;
		}
		case BOSS_STAGE_STATE::BATTLE:
		{
			if (m_pGolem != nullptr)
				m_pGolem->RequestChange(GOLEM_STATE::COMBAT);

			CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
			if (pPlayer != nullptr)
			{
				pPlayer->SetPlayerBehaviorEnable(true);
				CCameraMgr::GetInstance()->SetCameraTarget(pPlayer);
			}

			CUIMgr::GetInstance()->HideUI(UIID::LETTER_BOX);
			CUIMgr::GetInstance()->ShowUI(UIID::BASIC_INFO);
			CUIMgr::GetInstance()->ShowUI(UIID::BOSS_HP, m_pGolem);
			break;
		}
		case BOSS_STAGE_STATE::FINAL:
		{
			CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
			if (pPlayer != nullptr)
			{
				// 플레이어 조작 불가
				pPlayer->RequestChange(PLAYER_STATE::IDLE);
				pPlayer->SetPlayerBehaviorEnable(false);

				// 카메라 이동 및 인트로 상태 변경
				CCameraMgr::GetInstance()->MoveCamera(m_pErma->GetInfo().vPoint);
				m_eFinalState = BOSS_STAGE_FINAL_STATE::CAMERA_MOVE1;

				// UI는 다 닫고, 레터 박스만 열리게 구현
				CUIMgr::GetInstance()->HideAllUI();
				CUIMgr::GetInstance()->ShowUI(UIID::LETTER_BOX);
			}
			break;
		}
		case BOSS_STAGE_STATE::CLEAR:
		{
			CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
			if (pPlayer != nullptr)
			{
				pPlayer->SetPlayerBehaviorEnable(true);
				CCameraMgr::GetInstance()->SetCameraTarget(pPlayer);
			}

			CUIMgr::GetInstance()->HideUI(UIID::LETTER_BOX);
			CUIMgr::GetInstance()->ShowUI(UIID::BASIC_INFO);
			CUIMgr::GetInstance()->HideUI(UIID::BOSS_HP);
			break;
		}
		default:
			break;
		}

		m_eCurState = m_eNextState;
	}
}


void CBossStage::UpdateTime()
{
	switch (m_eCurState)
	{
	case BOSS_STAGE_STATE::INTRO:
	{
		UpdateIntroTime();
		break;
	}
	case BOSS_STAGE_STATE::FINAL:
	{
		UpdateFinalTime();
		break;
	}
	default:
		break;
	}
}

void CBossStage::UpdateIntroTime()
{
	m_dIntroTime += DT;

	switch (m_eIntroState)
	{
	case BOSS_STAGE_INTRO_STATE::CAMERA_MOVE:
		// 카메라가 특정 위치에 도착하면 다음 상태로 변경
		if (CCameraMgr::GetInstance()->GetCamera()->GetState() == CCamera::STATE::FIXED)
			ChangeIntroState(BOSS_STAGE_INTRO_STATE::DIALOG1);
		break;
	case BOSS_STAGE_INTRO_STATE::DIALOG1:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->GetUI(UIID::SPEECH_BUBBLE));
		// 말풍선이 종료되면 다음 상태로 이동
		if (nullptr != pBubble && pBubble->IsFinished())
			ChangeIntroState(BOSS_STAGE_INTRO_STATE::EXCLAMTION);

		break;
	}
	case BOSS_STAGE_INTRO_STATE::EXCLAMTION:
		// 1초 지나면 다음 상태
		if (m_dIntroTime >= 1.f)
			ChangeIntroState(BOSS_STAGE_INTRO_STATE::DIALOG2);
		break;
	case BOSS_STAGE_INTRO_STATE::DIALOG2:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->GetUI(UIID::SPEECH_BUBBLE));
		// 말풍선이 종료되면 다음 상태로 이동
		if (nullptr != pBubble && pBubble->IsFinished())
			ChangeIntroState(BOSS_STAGE_INTRO_STATE::DIALOG3);
		break;
	}
	case BOSS_STAGE_INTRO_STATE::DIALOG3:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->GetUI(UIID::SPEECH_BUBBLE));
		// 말풍선이 종료되면 다음 상태로 이동
		if (nullptr != pBubble && pBubble->IsFinished())
			ChangeIntroState(BOSS_STAGE_INTRO_STATE::ENTER_GOLEM);
		break;
	}
	case BOSS_STAGE_INTRO_STATE::ENTER_GOLEM:
	{
		if (m_pErma->IsReturnFinish())
			ChangeIntroState(BOSS_STAGE_INTRO_STATE::FINISH);

		break;
	}
	case BOSS_STAGE_INTRO_STATE::FINISH:
		if (m_dIntroTime >= 2.f)
			RequestChange(BOSS_STAGE_STATE::BATTLE);
		break;
	}
}

void CBossStage::UpdateFinalTime()
{
	m_dFinalTime += DT;

	switch (m_eFinalState)
	{
	case BOSS_STAGE_FINAL_STATE::CAMERA_MOVE1:
		// 카메라가 특정 위치에 도착하면 다음 상태로 변경
		if (CCameraMgr::GetInstance()->GetCamera()->GetState() == CCamera::STATE::FIXED)
			ChangeFinalState(BOSS_STAGE_FINAL_STATE::DIALOG1);
		break;
	case BOSS_STAGE_FINAL_STATE::DIALOG1:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->GetUI(UIID::SPEECH_BUBBLE));
		// 말풍선이 종료되면 다음 상태로 이동
		if (nullptr != pBubble && pBubble->IsFinished())
			ChangeFinalState(BOSS_STAGE_FINAL_STATE::ENTER_GOLEM);

		break;
	}
	case BOSS_STAGE_FINAL_STATE::ENTER_GOLEM:
		if (m_pErma->IsReturnFinish())
			ChangeFinalState(BOSS_STAGE_FINAL_STATE::DIALOG2);
		break;
	case BOSS_STAGE_FINAL_STATE::DIALOG2:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->GetUI(UIID::SPEECH_BUBBLE));
		// 말풍선이 종료되면 다음 상태로 이동
		if (nullptr != pBubble && pBubble->IsFinished())
			ChangeFinalState(BOSS_STAGE_FINAL_STATE::DIALOG3);

		break;
	}
	case BOSS_STAGE_FINAL_STATE::DIALOG3:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->GetUI(UIID::SPEECH_BUBBLE));
		// 말풍선이 종료되면 다음 상태로 이동
		if (nullptr != pBubble && pBubble->IsFinished())
			ChangeFinalState(BOSS_STAGE_FINAL_STATE::BROKEN1);

		break;
	}
	case BOSS_STAGE_FINAL_STATE::BROKEN1:
		if (m_dFinalTime >= 1.5f)
			ChangeFinalState(BOSS_STAGE_FINAL_STATE::BROKEN2);
		break;
	case BOSS_STAGE_FINAL_STATE::BROKEN2:
		if (m_dFinalTime >= 1.5f)
			ChangeFinalState(BOSS_STAGE_FINAL_STATE::DOWN);
		break;
	case BOSS_STAGE_FINAL_STATE::DOWN:
		if (m_dFinalTime >= 1.f)
			ChangeFinalState(BOSS_STAGE_FINAL_STATE::CAMERA_MOVE2);
		break;
	case BOSS_STAGE_FINAL_STATE::CAMERA_MOVE2:
		// 카메라가 특정 위치에 도착하면 다음 상태로 변경
		if (CCameraMgr::GetInstance()->GetCamera()->GetState() == CCamera::STATE::FIXED)
			ChangeFinalState(BOSS_STAGE_FINAL_STATE::DIALOG4);
		break;
	case BOSS_STAGE_FINAL_STATE::DIALOG4:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->GetUI(UIID::SPEECH_BUBBLE));
		// 말풍선이 종료되면 다음 상태로 이동
		if (nullptr != pBubble && pBubble->IsFinished())
			ChangeFinalState(BOSS_STAGE_FINAL_STATE::FINISH);

		break;
	}
	case BOSS_STAGE_FINAL_STATE::FINISH:
		break;
	}
}

void CBossStage::ChangeIntroState(BOSS_STAGE_INTRO_STATE eState)
{
	if (m_eIntroState == eState)
		return;

	m_eIntroState = eState;
	m_dIntroTime = 0.;

	switch (m_eIntroState)
	{
	case BOSS_STAGE_INTRO_STATE::CAMERA_MOVE:
		break;
	case BOSS_STAGE_INTRO_STATE::DIALOG1:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->ShowUI(UIID::SPEECH_BUBBLE));
		if (nullptr != pBubble && nullptr != m_pErma)
		{
			pBubble->SetText(m_pErma, L"에르마", L"암암암암암1", 3.0, 70.f);
			pBubble->SetOffset(VEC{ 0.f, m_pErma->GetInfo().vSize.fY * -0.5f });
		}
		break;
	}
	case BOSS_STAGE_INTRO_STATE::EXCLAMTION:
	{
		if (nullptr == m_pErma)
			return;

		m_pErma->SetFrameKey(L"Erma_L");

		VEC vRenderPoint = m_pErma->GetInfo().vPoint;
		vRenderPoint.fX -= 50.f;
		vRenderPoint.fY -= 50.f;
		CEffectMgr::GetInstance()->CreateEffect(L"Emoj_Exclamation", vRenderPoint, EFTMGR_FIXED | EFTMGR_SCROLL | EFTMGR_IMAGE, 0.f, 0.1f);
		break;
	}
	case BOSS_STAGE_INTRO_STATE::DIALOG2:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->ShowUI(UIID::SPEECH_BUBBLE));
		if (nullptr != pBubble && nullptr != m_pErma)
		{
			pBubble->SetText(m_pErma, L"에르마", L"암암암암암2", 3.0, 70.f);
			pBubble->SetOffset(VEC{ 0.f, m_pErma->GetInfo().vSize.fY * -0.5f });
		}
		break;
	}
	case BOSS_STAGE_INTRO_STATE::DIALOG3:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->ShowUI(UIID::SPEECH_BUBBLE));
		if (nullptr != pBubble && nullptr != m_pErma)
		{
			pBubble->SetText(m_pErma, L"에르마", L"암암암암암3", 3.0, 70.f);
			pBubble->SetOffset(VEC{ 0.f, m_pErma->GetInfo().vSize.fY * -0.5f });
		}
		break;
	}
	case BOSS_STAGE_INTRO_STATE::ENTER_GOLEM:
	{
		if (nullptr == m_pErma)
			return;

		CCameraMgr::GetInstance()->SetCameraTarget(m_pErma);
		m_pErma->StartReturn();
		break;
	}
	case BOSS_STAGE_INTRO_STATE::FINISH:
		CSoundMgr::GetInstance()->PlaySound(L"DungeonLibrary_Boss.wav", CHANNEL_GROUPID::BGM, 0.5f);
		break;
	}
}

void CBossStage::ChangeFinalState(BOSS_STAGE_FINAL_STATE eState)
{
	if (m_eFinalState == eState)
		return;

	m_eFinalState = eState;
	m_dFinalTime = 0.;

	switch (m_eFinalState)
	{
	case BOSS_STAGE_FINAL_STATE::CAMERA_MOVE1:
		CSoundMgr::GetInstance()->StopAll();
		break;
	case BOSS_STAGE_FINAL_STATE::DIALOG1:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->ShowUI(UIID::SPEECH_BUBBLE));
		if (nullptr != pBubble && nullptr != m_pErma)
		{
			pBubble->SetText(m_pErma, L"에르마", L"암암암암암1", 3.0, 70.f);
			pBubble->SetOffset(VEC{ 0.f, m_pErma->GetInfo().vSize.fY * -0.5f });
		}
		break;
	}
	case BOSS_STAGE_FINAL_STATE::ENTER_GOLEM:
		if (nullptr == m_pErma)
			return;

		CCameraMgr::GetInstance()->SetCameraTarget(m_pErma);
		m_pErma->StartReturn();
		break;
	case BOSS_STAGE_FINAL_STATE::DIALOG2:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->ShowUI(UIID::SPEECH_BUBBLE));
		if (nullptr != pBubble && nullptr != m_pErma)
		{
			pBubble->SetText(m_pErma, L"에르마", L"암암암암암2", 3.0, 70.f);
			pBubble->SetOffset(VEC{ 0.f, m_pErma->GetInfo().vSize.fY * -0.5f });
		}
		break;
	}
	case BOSS_STAGE_FINAL_STATE::DIALOG3:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->ShowUI(UIID::SPEECH_BUBBLE));
		if (nullptr != pBubble && nullptr != m_pErma)
		{
			pBubble->SetText(m_pErma, L"에르마", L"암암암암암3", 3.0, 70.f);
			pBubble->SetOffset(VEC{ 0.f, m_pErma->GetInfo().vSize.fY * -0.5f });
		}
		break;
	}
	case BOSS_STAGE_FINAL_STATE::BROKEN1:
	{
		VEC vOffset{ -20.f, 0.f };
		CEffectMgr::GetInstance()->CreateEffect(L"Golem_Core_Broken", m_pErma->GetInfo().vPoint + vOffset, EFTMGR_FIXED | EFTMGR_SCROLL | EFTMGR_IMAGE, 0.f, 0.1f);
		break;
	}
	case BOSS_STAGE_FINAL_STATE::BROKEN2:
	{
		VEC vOffset{ -20.f, 0.f };
		CEffectMgr::GetInstance()->CreateEffect(L"Golem_Core_Broken", m_pErma->GetInfo().vPoint + vOffset, EFTMGR_FIXED | EFTMGR_SCROLL | EFTMGR_IMAGE, 0.f, 0.1f);
		break;
	}
	case BOSS_STAGE_FINAL_STATE::DOWN:
		m_pGolem->RequestChange(GOLEM_STATE::DOWN);
		break;
	case BOSS_STAGE_FINAL_STATE::CAMERA_MOVE2:
		CCameraMgr::GetInstance()->MoveCamera(m_pErma->GetInfo().vPoint);
		break;
	case BOSS_STAGE_FINAL_STATE::DIALOG4:
	{
		CSpeechBubble* pBubble = static_cast<CSpeechBubble*>(CUIMgr::GetInstance()->ShowUI(UIID::SPEECH_BUBBLE));
		if (nullptr != pBubble && nullptr != m_pErma)
		{
			m_pErma->RequestChange(ERMA_STATE::DOWN);
			pBubble->SetText(m_pErma, L"에르마", L"암암암암암4", 3.0, 70.f);
			pBubble->SetOffset(VEC{ 0.f, m_pErma->GetInfo().vSize.fY * -0.5f });
		}
		break;
	}
	case BOSS_STAGE_FINAL_STATE::FINISH:
		CFade* pFade = static_cast<CFade*>(CUIMgr::GetInstance()->ShowUI(UIID::FADE));
		pFade->SetFadeType(FADE_TYPE::RECTANGLE);

		CSoundMgr::GetInstance()->PlaySound(L"DugeonLibrary_Field.wav", CHANNEL_GROUPID::BGM, 0.5f);
		RequestChange(BOSS_STAGE_STATE::CLEAR);
		break;
	}
}
