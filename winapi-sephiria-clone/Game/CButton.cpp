#include "pch.h"
#include "CButton.h"

#include "CMouse.h"

#include "CImgMgr.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CFontMgr.h"
#include "CSceneMgr.h"

CButton::CButton()
	: m_onClick(nullptr), m_bCol(false), m_rcPrint{}, m_rcScreen{}, m_strBtn{}, m_vCellSize{}, m_bEnable(true)
{
	ZeroMemory(&m_vScrollOffset, sizeof(VEC));
}

CButton::~CButton()
{
	Release();
}

void CButton::Initialize()
{
	m_fUIScale = PIXEL_SCALE;

	// 따로 FrameKey 변경이 없었다면 기본 버튼 이미지 사용
	m_pFrameKey = L"Button";
	m_vCellSize = { 61.f, 24.f };
	m_tInfo.vPoint = { 0.f, 0.f };
	m_tInfo.vSize = m_vCellSize * m_fUIScale;

	m_eRender = RENDERID::UI;
	m_iRenderLayer = 2;
}

int CButton::Update()
{
	if (m_bDead)
		return DEAD;
	if (!m_bView)
		return NOEVENT;

	// 스크린 렉트 업데이트 
	m_rcScreen = m_rcPrint;
	m_rcScreen.X += m_vScrollOffset.fX;
	m_rcScreen.Y += m_vScrollOffset.fY;

	UpdateRect();
	return NOEVENT;
}

void CButton::LateUpdate()
{
	if (!m_bView)
		return;

	if (nullptr == m_pMouse)
		return;

	m_bCol = CCollisionMgr::CollisionMouse(m_pMouse->GetInfo().vPoint, m_tRect);
	if (m_bCol && m_bEnable && KEY_DOWN(VK_LBUTTON))
	{
		// 콜백 함수 실행
		Click();
	}
}

void CButton::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	Image* pImg(nullptr);
	pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

#ifdef _DEBUG
	SolidBrush whiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&whiteBrush, m_rcScreen);
#endif // _DEBUG

	int iFrame(0);
	if (m_bCol)
		iFrame = 1;
	if (!m_bEnable)
		iFrame = 2;

	pGraphics->DrawImage(
		pImg, m_rcScreen,
		m_vCellSize.fX * iFrame,
		0,
		m_vCellSize.fX,
		m_vCellSize.fY,
		UnitPixel
	);

	if (m_strBtn.empty())
		return;

	
	RectF rcStringRect = {
		m_rcScreen.X + m_rcScreen.Width / 10.f,
		m_rcScreen.Y + m_rcScreen.Height / 10.f,
		m_rcScreen.Width - m_rcScreen.Width / 5.f,
		m_rcScreen.Height - m_rcScreen.Height / 5.f
	};

#ifdef _DEBUG
	SolidBrush magentaBrush(Color(255, 255, 0, 255));
	pGraphics->FillRectangle(&magentaBrush, rcStringRect);
#endif // _DEBUG

	Color fontColor = m_bEnable ? Color{ 255, 255, 255, 255 } : Color{ 255, 120, 120, 120 };

	CFontMgr::GetInstance()->DrawString(pGraphics, m_strBtn, FONT_TYPE::NORMAL, rcStringRect, fontColor);
}

void CButton::Release()
{
}

void CButton::UpdateRect()
{
	m_tRect = { static_cast<LONG>(m_rcScreen.X), 
				static_cast<LONG>(m_rcScreen.Y), 
				static_cast<LONG>(m_rcScreen.X + m_rcScreen.Width), 
				static_cast<LONG>(m_rcScreen.Y + m_rcScreen.Height) };
	m_tInfo.vPoint = { m_rcScreen.X + m_rcScreen.Width * 0.5f, m_rcScreen.Y + m_rcScreen.Height * 0.5f };
	m_tInfo.vSize = { m_rcScreen.Width, m_rcScreen.Height };
}

void CButton::Click()
{
	if (nullptr != m_onClick)
		m_onClick();
}
