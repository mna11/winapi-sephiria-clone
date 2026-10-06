#include "pch.h"
#include "CMagicStaffShield.h"

CMagicStaffShield::CMagicStaffShield()
{
	m_vCellSize = { 10.f, 10.f };
	m_pFrameKey = L"Magic_Wand_Shield";
}

CMagicStaffShield::~CMagicStaffShield()
{
}

void CMagicStaffShield::HandleIdleRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	VEC vDrawSize = m_vCellSize * PIXEL_SCALE;
	RectF DestRect = { m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
					   m_tInfo.vPoint.fY - vDrawSize.fY * 0.5f + vScroll.fY,
					   vDrawSize.fX, vDrawSize.fY };

	pGraphics->DrawImage(
		pImg, DestRect,
		0.f, 0.f, m_vCellSize.fX, m_vCellSize.fY, UnitPixel);
}

void CMagicStaffShield::HandleAttackRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
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

void CMagicStaffShield::HandleAttack1Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}

void CMagicStaffShield::HandleAttack2Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}

void CMagicStaffShield::HandleAttack3Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}

void CMagicStaffShield::HandleShieldRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
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

void CMagicStaffShield::HandleCleaveRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}

