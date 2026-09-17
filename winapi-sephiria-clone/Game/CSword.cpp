#include "pch.h"
#include "CSword.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"

CSword::CSword()
	: m_vCellSize{ 9.f, 16.f },
	m_pWeaponState(nullptr), m_pAtk(nullptr), m_bAtkStart(false)
{
}

CSword::~CSword()
{
	Release();
}

void CSword::Initialize()
{
	m_tInfo = { 0.f, 0.f, 25.f, 50.f };
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 2;
}

int CSword::Update()
{
	if (nullptr == m_pTarget)
		return NOEVENT;

	float fTargetAngle = m_pTarget->GetAngle();
	VEC vTargetPoint = m_pTarget->GetInfo().vPoint;
	VEC vOffset = m_pTarget->GetInfo().vSize * 0.2;

	// 4사분면 - 여기서 분면 기준은 카테시안 좌표계 / 카테시안 좌표계 기준 마우스를 4사분면에 놓았을 때
	if (fTargetAngle >= 0.f && fTargetAngle < PI * 0.5f)
		this->SetPos(static_cast<float>(m_pTarget->GetRect().right), vTargetPoint.fY + vOffset.fY);
	// 3사분면 
	else if (fTargetAngle >= PI * 0.5f && fTargetAngle < PI)
		this->SetPos(static_cast<float>(m_pTarget->GetRect().left), vTargetPoint.fY + vOffset.fY);
	// 2사분면
	else if (fTargetAngle >= -PI && fTargetAngle < -PI * 0.5f)
		this->SetPos(static_cast<float>(m_pTarget->GetRect().left) - vOffset.fX, vTargetPoint.fY + vOffset.fY);
	// 1사분면
	else
		this->SetPos(static_cast<float>(m_pTarget->GetRect().right) + vOffset.fX, vTargetPoint.fY + vOffset.fY);

	__super::UpdateRect();
	return NOEVENT;
}

void CSword::LateUpdate()
{
}

void CSword::Render(Graphics* pGraphics)
{
	VEC    vScroll = CCameraMgr::GetInstance()->GetScroll();
	Image* pShieldImg = CImgMgr::GetInstance()->FindImg(L"Sword_Tier1");
	if (nullptr == pShieldImg)
		return;

	switch (*m_pWeaponState)
	{
	case SWORD_AND_SHIELD_STATE::IDLE:
		HandleIdleRender(pGraphics, pShieldImg, vScroll);
		break;
	case SWORD_AND_SHIELD_STATE::ATTACK:
		HandleAttackRender(pGraphics, pShieldImg, vScroll);
		break;
	case SWORD_AND_SHIELD_STATE::DEFENSE:
		HandleShieldRender(pGraphics, pShieldImg, vScroll);
		break;
	case SWORD_AND_SHIELD_STATE::CLEAVE:
		HandleCleaveRender(pGraphics, pShieldImg, vScroll);
		break;
	default:
		break;
	}
}

void CSword::Release()
{
}

#pragma region HandleRender
void CSword::HandleIdleRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
#ifdef _DEBUG
	SolidBrush blackBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&blackBrush, (int)(m_tRect.left + vScroll.fX),
		(int)(m_tRect.top + vScroll.fY),
		(int)m_tInfo.vSize.fX,
		(int)m_tInfo.vSize.fY);
#endif // _DEBUG

	float fTargetAngle = m_pTarget->GetAngle();

	// 2사분면, 3사분면 - 여기서 분면 기준은 카테시안 좌표계
	if (fabsf(fTargetAngle) > PI * 0.5f)
	{
		fTargetAngle = fTargetAngle - PI;
	}
	fTargetAngle *= 180 / PI;

	Matrix matRot;
	matRot.RotateAt(fTargetAngle, { m_tInfo.vPoint.fX + vScroll.fX, m_tInfo.vPoint.fY + vScroll.fY });
	pGraphics->SetTransform(&matRot); // DC 회전

	// 회전된 DC에다가 그리기
	VEC vDrawSize = m_vCellSize * PIXEL_SCALE;
	RectF DestRect = { m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
					   m_tRect.top - vDrawSize.fY * 0.5f + vScroll.fY,
					   vDrawSize.fX, vDrawSize.fY };

	pGraphics->DrawImage(
		pImg, DestRect,
		0.f, 0.f, m_vCellSize.fX, m_vCellSize.fY, UnitPixel);

	pGraphics->ResetTransform();
}

void CSword::HandleAttackRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
#ifdef _DEBUG
	SolidBrush blackBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&blackBrush, (int)(m_tRect.left + vScroll.fX),
		(int)(m_tRect.top + vScroll.fY),
		(int)m_tInfo.vSize.fX,
		(int)m_tInfo.vSize.fY);
#endif // _DEBUG

	// 빙글빙글 안돌게!
	if (m_pAtk->dElapseTime > m_pAtk->dMaxTime)
	{
		HandleIdleRender(pGraphics, pImg, vScroll);
		return;
	}

	switch (m_pAtk->iLevel)
	{
	case 0:
		HandleAttack1Render(pGraphics, pImg, vScroll);
		break;
	case 1:
		HandleAttack2Render(pGraphics, pImg, vScroll);
		break;
	case 2:
		HandleAttack3Render(pGraphics, pImg, vScroll);
		break;
	}
}

void CSword::HandleAttack1Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	float fTargetAngle = m_fAngle;
	if (fabsf(fTargetAngle) > PI * 0.5f)
	{
		fTargetAngle = fTargetAngle - PI;
		fTargetAngle -= PI * m_pAtk->dElapseTime / m_pAtk->dMaxTime;
	}
	else
	{
		fTargetAngle += PI * m_pAtk->dElapseTime / m_pAtk->dMaxTime;
	}
	fTargetAngle *= 180.f / PI;

	Matrix matRot;
	matRot.RotateAt(fTargetAngle, { m_tInfo.vPoint.fX + vScroll.fX, m_tInfo.vPoint.fY + vScroll.fY });
	pGraphics->SetTransform(&matRot); // DC 회전

	// 회전된 DC에다가 그리기
	VEC vDrawSize = m_vCellSize * PIXEL_SCALE;
	RectF DestRect = { m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
					   m_tRect.top - vDrawSize.fY * 0.5f + vScroll.fY,
					   vDrawSize.fX, vDrawSize.fY };

	pGraphics->DrawImage(
		pImg, DestRect,
		0.f, 0.f, m_vCellSize.fX, m_vCellSize.fY, UnitPixel);

	pGraphics->ResetTransform();
}


void CSword::HandleAttack2Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	float fTargetAngle = m_fAngle;
	if (fabsf(fTargetAngle) > PI * 0.5f)
	{
		fTargetAngle = fTargetAngle - PI;
		fTargetAngle -= PI * m_pAtk->dElapseTime / m_pAtk->dMaxTime;
	}
	else
	{
		fTargetAngle += PI * m_pAtk->dElapseTime / m_pAtk->dMaxTime;
	}

	fTargetAngle *= 180.f / PI;

	Matrix matRot;
	matRot.RotateAt(fTargetAngle, { m_tInfo.vPoint.fX + vScroll.fX, m_tInfo.vPoint.fY + vScroll.fY });
	pGraphics->SetTransform(&matRot); // DC 회전

	// 회전된 DC에다가 그리기
	VEC vDrawSize = m_vCellSize * PIXEL_SCALE;
	RectF DestRect = { m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
					   m_tRect.top - vDrawSize.fY * 0.5f + vScroll.fY,
					   vDrawSize.fX, vDrawSize.fY };

	pGraphics->DrawImage(
		pImg, DestRect,
		0.f, 0.f, m_vCellSize.fX, m_vCellSize.fY, UnitPixel);

	pGraphics->ResetTransform();
}

void CSword::HandleAttack3Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	float fTargetAngle = m_fAngle;
	if (fabsf(fTargetAngle) > PI * 0.5f)
	{
		fTargetAngle = fTargetAngle - PI;
		fTargetAngle -= PI * (1 - m_pAtk->dElapseTime / m_pAtk->dMaxTime);
	}
	else
	{
		fTargetAngle += PI * (1 - m_pAtk->dElapseTime / m_pAtk->dMaxTime);
	}

	fTargetAngle *= 180.f / PI;

	Matrix matRot;
	matRot.RotateAt(fTargetAngle, { m_tInfo.vPoint.fX + vScroll.fX, m_tInfo.vPoint.fY + vScroll.fY });
	pGraphics->SetTransform(&matRot); // DC 회전

	// 회전된 DC에다가 그리기
	VEC vDrawSize = m_vCellSize * PIXEL_SCALE;
	RectF DestRect = { m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
					   m_tRect.top - vDrawSize.fY * 0.5f + vScroll.fY,
					   vDrawSize.fX, vDrawSize.fY };

	pGraphics->DrawImage(
		pImg, DestRect,
		0.f, 0.f, m_vCellSize.fX, m_vCellSize.fY, UnitPixel);

	pGraphics->ResetTransform();

}

void CSword::HandleShieldRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}

void CSword::HandleCleaveRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}
#pragma endregion HandleRender