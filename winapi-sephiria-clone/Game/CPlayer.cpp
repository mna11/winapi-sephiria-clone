#include "pch.h"
#include "CPlayer.h"

#include "CWeaponController.h"

#include "CAbstractFactory.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"

CPlayer::CPlayer()
	: CState(PLAYER_STATE::IDLE, PLAYER_STATE::END), m_pWeaponController(nullptr)
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
	m_fSpeed = 200.f;

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
}

int CPlayer::Update()
{
	if (m_bDead)
		return DEAD;

	// 이번 프레임 상태 결정
	ApplyChange();
	// 무기 컨트롤러 업데이트
	m_pWeaponController->Update();

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
		m_eCurState = PLAYER_STATE::WALK;
	}
	else
	{
		m_eCurState = PLAYER_STATE::IDLE;
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
	if (KEY_PRESS(VK_LBUTTON))
		m_pWeaponController->Attack();
	else if (KEY_PRESS(VK_RBUTTON))
		m_pWeaponController->SpecialAttack();
}

void CPlayer::ApplyChange()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
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
			SetFrame(0, 9, 3, 0.2);
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

		m_ePreState = m_eCurState;
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