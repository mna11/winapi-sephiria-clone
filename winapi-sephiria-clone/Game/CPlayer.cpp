#include "pch.h"
#include "CPlayer.h"

#include "CWeaponController.h"

#include "CAbstractFactory.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CEffectMgr.h"

CPlayer::CPlayer()
	: CState(PLAYER_STATE::END, PLAYER_STATE::IDLE), m_pWeaponController(nullptr),
	m_dDustInterval(0.5), m_dDustElapseTime(0.), m_fNormalSpeed(200.f), m_fRunSpeed(400.f)
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	// 기초 정보 초기화
	m_tInfo = { WINCX >> 1, WINCY >> 1, 30.f, 40.f };
	m_eRender = RENDERID::GAMEOBJECT;
	m_fSpeed = m_fNormalSpeed;

	// 스프라이트 시트 Insert 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Player/Player_LEFTDOWN.png", L"Player_LD");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Player/Player_LEFTUP.png", L"Player_LU");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Player/Player_RIGHTDOWN.png", L"Player_RD");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Player/Player_RIGHTUP.png", L"Player_RU");

	// 애니메이션 프레임 초기화
	m_pFrameKey = L"Player_RD";
	SetFrame(0, 5, 0, 0.2);

	// 무기 컨트롤러 초기화 - 무기는 누가 사용하는지 참조 시켜야 함
	m_pWeaponController = CAbstractFactory<CWeaponController>::CreateWeaponController(this);

	// 렌더 레이어 - 0 ~ 5 사이 플레이어는 중간인 3
	m_iRenderLayer = 3;

	m_iDash = m_iMaxDash = 100;
}

int CPlayer::Update()
{
	if (m_bDead)
		return DEAD;

	// Elapse 변수들 업데이트
	UpdateTime();

	// 이번 프레임 상태 결정
	ApplyChange();
	// 무기 컨트롤러 업데이트
	m_pWeaponController->Update();

	// 이번 프레임 이동 전 위치 기억
	m_vPrePoint = m_tInfo.vPoint;

	// 플레이어 대쉬
	Dash();
	// 플레이어 이동
	Move(); 
	// 플레이어 회전 
	Rotate();
	// 플레이어 공격 
	Attack();

	// 충돌 박스 & 애니메이션 프레임 업데이트
	__super::UpdateRect();
	__super::UpdateFrame();

#ifdef _DEBUG
	PrintInfo();
#endif // _DEBUG

	return NOEVENT;
}

void CPlayer::LateUpdate()
{
	m_pWeaponController->LateUpdate();
	
	// 이펙트 생성
	CreateEffect();
}

void CPlayer::Render(Graphics* pGraphics)
{

	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

#ifdef _DEBUG
	SolidBrush blackBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&blackBrush, (int)(m_tRect.left + vScroll.fX),
								  (int)(m_tRect.top + vScroll.fY), 
								  (int)m_tInfo.vSize.fX, 
								  (int)m_tInfo.vSize.fY);

	m_pWeaponController->Render(pGraphics);
#endif // _DEBUG

	Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

	VEC vCellSize{ 18.f, 19.f };
	VEC vImgSize = vCellSize * PIXEL_SCALE;

	RectF rcDest{	m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX, 
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY, 
					vImgSize.fX,
					vImgSize.fY};

	pGraphics->DrawImage(
		pImg, rcDest,
		vCellSize.fX * m_tFrame.iStart,
		vCellSize.fY * m_tFrame.iMotion,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);
}

void CPlayer::Release()
{
	SafeDelete<CWeaponController*>(m_pWeaponController);
}

void CPlayer::UpdateTime()
{
	m_dDustElapseTime += DT;
	if (m_iDash < m_iMaxDash)
		m_dDashCountRecoveryElapseTime += DT;
}

void CPlayer::Move()
{
	// 플레이어 이동
	VEC vDir{};

	if (KEY_PRESS('W'))
		vDir.fY = -1.f;
	if (KEY_PRESS('A'))
		vDir.fX = -1.f;
	if (KEY_PRESS('S'))
		vDir.fY = 1.f;
	if (KEY_PRESS('D'))
		vDir.fX = 1.f;

	if (vDir != VEC{ 0.f, 0.f })
	{
		m_tInfo.vPoint += vDir.Normalize() * m_fSpeed * DT;
		m_eNextState = PLAYER_STATE::WALK;
	}
	else 
	{
		// 공격 상태는 무기에서 풀어주게 설계함
		if (m_eCurState != PLAYER_STATE::ATTACK
			&& m_eCurState != PLAYER_STATE::HEAVY_ATTACK)
			m_eNextState = PLAYER_STATE::IDLE;
	}
}

void CPlayer::Dash()
{
	if (KEY_DOWN(VK_SPACE))
	{
		if (m_iDash > 0)
		{
			--m_iDash;
			m_fSpeed = m_fRunSpeed * 20.f;
		}
	}
	else if (KEY_HOLD(VK_SPACE))
	{
		m_fSpeed = m_fRunSpeed;
		m_tFrame.dFrameSpeed = 0.05f;
	}
	else if (KEY_UP(VK_SPACE))
	{
		m_fSpeed = m_fNormalSpeed;
		m_tFrame.dFrameSpeed = 0.2f;
	}

	if (m_dDashCountRecorveyInterval <= m_dDashCountRecoveryElapseTime)
	{
		m_iDash = (m_iDash + 1 >= m_iMaxDash) ? m_iMaxDash : m_iDash + 1;
	}
}

void CPlayer::Rotate()
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	// 스크롤을 더한다라는 행위 -> 월드 좌표에서 화면 좌표로 바꾸겠다.
	// 스크롤을 빼는 행위       -> 화면 좌표에서 월드 좌표로 바꾸겠다.
	// 플레이어의 현재 월드 좌표와 마우스의 위치를 비교해야되기 때문에
	// 마우스에는 스크롤을 빼는 것이다.
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
	VEC vTargetPoint{ ptMouse.x - vScroll.fX, ptMouse.y - vScroll.fY };

	VEC vDir = vTargetPoint - m_tInfo.vPoint;

	// 플레이어 스프라이트 정하기
	if (vDir.fX > 0)
	{
		if (vDir.fY < 0)
			m_pFrameKey = L"Player_RU";
		else
			m_pFrameKey = L"Player_RD";
	}
	else
	{
		if (vDir.fY < 0)
			m_pFrameKey = L"Player_LU";
		else
			m_pFrameKey = L"Player_LD";
	}
		
	m_fAngle = atan2f(vDir.fY, vDir.fX);
}

void CPlayer::Attack()
{
	// 공격 상태 돌입 등과 같은건 무기에서 설정함
	if (KEY_DOWN(VK_LBUTTON))
	{
		m_pWeaponController->Attack();
	}
	else if (KEY_DOWN(VK_RBUTTON))
	{
		m_pWeaponController->SpecialAttack();
	}
}

void CPlayer::CreateEffect()
{
	// 먼지 이펙트 생성
	if (m_vPrePoint != m_tInfo.vPoint && m_dDustInterval <= m_dDustElapseTime)
	{
		// 일반 걷기
		if (m_fSpeed == m_fNormalSpeed)
		{
			CEffectMgr::GetInstance()->CreateEffect(L"RunDust", m_tInfo.vPoint, 0.f);
		}
		// 대쉬 - 와다다
		else if (m_fSpeed == m_fRunSpeed)
		{
			VEC vDir = m_tInfo.vPoint - m_vPrePoint;
			CEffectMgr::GetInstance()->CreateEffect(L"DashDust", m_tInfo.vPoint, atan2f(vDir.fY, vDir.fX));
		}
		m_dDustElapseTime = 0.;
	}

	// 대쉬 잔상 표현
	// KEYDOWN은 딱 해당 프레임에만 true이므로 이렇게 처리함
	if (m_fSpeed > m_fRunSpeed)
	{
		VEC vDir = m_tInfo.vPoint - m_vPrePoint;
		// 이전 위치에서 현재 위치까지 0 ~ 5 내분이라고 생각했을 때, 1 2 3 4 포인트에만 그리겠다는 뜻
		for (float i = 0.25f; i <= 1.f; i+=0.25f)
		{
			VEC vTrail = vDir * i + m_vPrePoint;
			CEffectMgr::GetInstance()->CreateEffect(L"DashTrail", vTrail, i);
		}
	}
}

void CPlayer::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case PLAYER_STATE::IDLE:
			SetFrame(0, 5, 0, 0.1);
			break;
		case PLAYER_STATE::WALK:
			SetFrame(0, 7, 1, 0.1);
			break;
		case PLAYER_STATE::ATTACK:
			SetFrame(0, 2, 2, 0.2);
			break;
		case PLAYER_STATE::HEAVY_ATTACK:
			SetFrame(0, 9, 3, 0.02);
			break;
		case PLAYER_STATE::WHIRLWIND_READY:
			SetFrame(0, 3, 4, 0.2);
			break;
		case PLAYER_STATE::WHIRLWIND_CYCLE:
			SetFrame(0, 6, 5, 0.2);
			break;
		case PLAYER_STATE::AIR:
			SetFrame(0, 2, 6, 0.2);
			break;
		case PLAYER_STATE::DOWN:
			SetFrame(0, 0, 7, 0.2);
			break;
		default:
			break;
		}

		m_eCurState = m_eNextState;
	}
}


#ifdef _DEBUG
void CPlayer::PrintInfo()
{
	m_dPrintInterval -= DT;
	if (m_dPrintInterval <= 0)
	{
		cout << "플레이어 위치 : " << m_tInfo.vPoint.fX << ", " << m_tInfo.vPoint.fY << endl;
		cout << "플레이어 각도 : " << m_fAngle << endl;
		m_dPrintInterval = 3.;
	}
}
#endif // _DEBUG