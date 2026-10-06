#include "pch.h"
#include "CNormalShield.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"

CNormalShield::CNormalShield()
{
	m_vCellSize = { 9.f, 10.f };
	m_pFrameKey = L"Normal_Sword_Shield_Shield";
}

CNormalShield::~CNormalShield()
{
	Release();
}

void CNormalShield::HandleIdleRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	VEC vDrawSize = m_vCellSize * PIXEL_SCALE;
	RectF DestRect = { m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
					   m_tInfo.vPoint.fY - vDrawSize.fY * 0.5f + vScroll.fY,
					   vDrawSize.fX, vDrawSize.fY };

	pGraphics->DrawImage(
		pImg, DestRect,
		0.f, 0.f, m_vCellSize.fX, m_vCellSize.fY, UnitPixel);
}

void CNormalShield::HandleAttackRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	switch (m_pAtk->iLevel)
	{
	case 1:
		HandleAttack1Render(pGraphics, pImg, vScroll);
		break;
	case 2:
		HandleAttack2Render(pGraphics, pImg, vScroll);
		break;
	case 3:
		HandleAttack3Render(pGraphics, pImg, vScroll);
		break;
	}
}

void CNormalShield::HandleAttack1Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}

void CNormalShield::HandleAttack2Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}

void CNormalShield::HandleAttack3Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}

void CNormalShield::HandleShieldRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	float fTargetAngle = m_pTarget->GetAngle();
	float fDistance = 20.f;
	VEC vOffset{ fDistance * cosf(fTargetAngle), fDistance * sinf(fTargetAngle) };
	VEC vRenderPoint = m_tInfo.vPoint + vOffset;

	VEC vDrawSize = m_vCellSize * PIXEL_SCALE;
	RectF DestRect = { vRenderPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
					   vRenderPoint.fY - vDrawSize.fY * 0.5f + vScroll.fY,
					   vDrawSize.fX, vDrawSize.fY };

	pGraphics->DrawImage(
		pImg, DestRect,
		0.f, 0.f, m_vCellSize.fX, m_vCellSize.fY, UnitPixel);
}

void CNormalShield::HandleCleaveRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}
