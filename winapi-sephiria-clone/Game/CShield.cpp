#include "pch.h"
#include "CShield.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"

CShield::CShield()
    : m_vCellSize{ 9.f, 10.f }
{
}

CShield::~CShield()
{
    Release();
}

void CShield::Initialize()
{
    m_tInfo = { 0.f, 0.f, 10.f, 10.f};
    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 4; 
}

int CShield::Update()
{
    return NOEVENT;
}

void CShield::LateUpdate()
{

}

void CShield::Render(Graphics* pGraphics)
{
	Image* pShieldImg = CImgMgr::GetInstance()->FindImg(L"Shield_Tier1");
	if (nullptr == pShieldImg)
		return;

	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

	VEC vDrawSize = m_vCellSize * PIXEL_SCALE;

	RectF DestRect = { m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
					   m_tInfo.vPoint.fY - vDrawSize.fY * 0.5f + vScroll.fY,
					   vDrawSize.fX, vDrawSize.fY };

	pGraphics->DrawImage(
		pShieldImg, DestRect,
		0.f, 0.f, m_vCellSize.fX, m_vCellSize.fY, UnitPixel);
}

void CShield::Release()
{
}
