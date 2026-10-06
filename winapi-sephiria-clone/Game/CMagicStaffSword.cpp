#include "pch.h"
#include "CMagicStaffSword.h"

CMagicStaffSword::CMagicStaffSword()
{
	m_vCellSize = { 7.f, 18.f };
	m_pFrameKey = L"Magic_Wand_Staff";
}

CMagicStaffSword::~CMagicStaffSword()
{
}

void CMagicStaffSword::HandleIdleRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	float fTargetAngle = m_pTarget->GetAngle();

	// 2사분면, 3사분면 - 여기서 분면 기준은 카테시안 좌표계
	if (fabsf(fTargetAngle) > PI * 0.5f)
	{
		fTargetAngle = fTargetAngle - PI;
	}
	fTargetAngle *= 180 / PI;

	DrawSword(pGraphics, pImg, vScroll, fTargetAngle);
}

void CMagicStaffSword::HandleAttackRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
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

void CMagicStaffSword::HandleAttack1Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	// PI만큼 아래로 회전시킬 건데, 현재 공격 클릭 후 걸린 시간 비율에 따라 다르게 한다.
	float fTargetAngle = m_fAngle;
	float fFactor = (m_pAtk->dElapseTime > m_pAtk->dMaxTime) ? 1.f : m_pAtk->dElapseTime / m_pAtk->dMaxTime;

	if (fabsf(fTargetAngle) > PI * 0.5f)
	{
		fTargetAngle = fTargetAngle - PI;
		fTargetAngle -= PI * fFactor;
	}
	else
	{
		fTargetAngle += PI * fFactor;
	}
	fTargetAngle *= 180.f / PI;

	DrawSword(pGraphics, pImg, vScroll, fTargetAngle);
}


void CMagicStaffSword::HandleAttack2Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	float fTargetAngle = m_fAngle;
	float fFactor = (m_pAtk->dElapseTime > m_pAtk->dMaxTime) ? 1.f : m_pAtk->dElapseTime / m_pAtk->dMaxTime;
	if (fabsf(fTargetAngle) > PI * 0.5f)
	{
		fTargetAngle = fTargetAngle - PI;
		fTargetAngle -= PI * (1 - fFactor);
	}
	else
	{
		fTargetAngle += PI * (1 - fFactor);
	}

	fTargetAngle *= 180.f / PI;

	DrawSword(pGraphics, pImg, vScroll, fTargetAngle);
}

void CMagicStaffSword::HandleAttack3Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	HandleAttack1Render(pGraphics, pImg, vScroll);
}

void CMagicStaffSword::HandleShieldRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	DrawSword(pGraphics, pImg, vScroll, 0.f);
}

void CMagicStaffSword::HandleCleaveRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}