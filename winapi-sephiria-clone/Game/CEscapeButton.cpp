#include "pch.h"
#include "CEscapeButton.h"

#include "CImgMgr.h"
#include "CMouse.h"

CEscapeButton::CEscapeButton()
	: m_bCol(false)
{
}

CEscapeButton::~CEscapeButton()
{
	Release();
}

void CEscapeButton::Initialize()
{
	m_tInfo = { WINCX - 50.f, 35.f, 50.f, 50.f };

	m_eRender = RENDERID::UI;
	m_iRenderLayer = 3;
	m_fUIScale = PIXEL_SCALE * 0.5f;
}

int CEscapeButton::Update()
{
	if (!m_bView)
		return NOEVENT;

	__super::UpdateRect();
	return NOEVENT;
}

void CEscapeButton::LateUpdate()
{
	if (!m_bView)
		return;

	POINT ptMouse{ static_cast<int>(m_pMouse->GetInfo().vPoint.fX), static_cast<int>(m_pMouse->GetInfo().vPoint.fY) };
	m_bCol = PtInRect(&m_tRect, ptMouse);
}

void CEscapeButton::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

#ifdef _DEBUG
	SolidBrush whiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&whiteBrush, (int)(m_tRect.left),
		(int)(m_tRect.top),
		(int)m_tInfo.vSize.fX,
		(int)m_tInfo.vSize.fY);
#endif // _DEBUG

	Image* pImg(nullptr);
	VEC vCellSize{ 21.f, 24.f };
	VEC vImgSize = vCellSize * m_fUIScale;

	pImg = CImgMgr::GetInstance()->FindImg(L"EscapeButton");
	if (nullptr == pImg)
		return;

	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f,
					vImgSize.fX,
					vImgSize.fY };

	pGraphics->DrawImage(
		pImg, rcDest,
		(m_bCol ? vCellSize.fX : 0),
		0,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);

	pImg = CImgMgr::GetInstance()->FindImg(L"EscapeString");
	vCellSize = {11.f, 9.f};
	vImgSize = vCellSize * m_fUIScale;
	rcDest = { m_tInfo.vPoint.fX - vImgSize.fX * 0.5f,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + 40.f,
					vImgSize.fX,
					vImgSize.fY };

	pGraphics->DrawImage(
		pImg, rcDest,
		0, 0,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);

}

void CEscapeButton::Release()
{
}
