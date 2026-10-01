#include "pch.h"
#include "CButton.h"

#include "CMouse.h"

#include "CImgMgr.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CFontMgr.h"
#include "CSceneMgr.h"

CButton::CButton()
	: m_onClick(nullptr), m_bCol(false), m_rcPrint{}, m_strBtn{}, m_vCellSize{}
{
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
	m_iRenderLayer = 4;
}

int CButton::Update()
{
	if (m_bDead)
		return DEAD;
	if (!m_bView)
		return NOEVENT;

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
	if (m_bCol && KEY_DOWN(VK_LBUTTON))
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
	pGraphics->FillRectangle(&whiteBrush, m_rcPrint);
#endif // _DEBUG


	pGraphics->DrawImage(
		pImg, m_rcPrint,
		(m_bCol ? m_vCellSize.fX : 0),
		0,
		m_vCellSize.fX,
		m_vCellSize.fY,
		UnitPixel
	);

	if (m_strBtn.empty() || lstrcmp(m_strBtn.c_str(), L""))
		return;

	
	RectF rcStringRect = {
		m_rcPrint.X + m_rcPrint.Width / 10.f,
		m_rcPrint.Y + m_rcPrint.Height / 10.f,
		m_rcPrint.Width - m_rcPrint.Width / 5.f,
		m_rcPrint.Height - m_rcPrint.Height / 5.f
	};

#ifdef _DEBUG
	SolidBrush magentaBrush(Color(255, 255, 0, 255));
	pGraphics->FillRectangle(&magentaBrush, rcStringRect);
#endif // _DEBUG

	CFontMgr::GetInstance()->DrawString(pGraphics, m_strBtn, FONT_TYPE::NORMAL, rcStringRect, Color{ 255, 255, 255, 255 });
}

void CButton::Release()
{
}

void CButton::UpdateRect()
{
	m_tRect = { static_cast<LONG>(m_rcPrint.X), 
				static_cast<LONG>(m_rcPrint.Y), 
				static_cast<LONG>(m_rcPrint.X + m_rcPrint.Width), 
				static_cast<LONG>(m_rcPrint.Y + m_rcPrint.Height) };

	m_tInfo.vSize = { m_rcPrint.Width, m_rcPrint.Height };
}

void CButton::Click()
{
	if (nullptr != m_onClick)
		m_onClick();
}
