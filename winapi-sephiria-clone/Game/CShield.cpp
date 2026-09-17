#include "pch.h"
#include "CShield.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"

CShield::CShield()
    : m_vCellSize{ 9.f, 10.f },
	m_pWeaponState(nullptr)
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
	if (nullptr == m_pTarget)
		return NOEVENT;

	float fTargetAngle = m_pTarget->GetAngle();
	VEC vTargetPoint = m_pTarget->GetInfo().vPoint;
	VEC vOffset = m_pTarget->GetInfo().vSize * 0.3f;

	// 4사분면 - 여기서 분면 기준은 카테시안 좌표계 / 카테시안 좌표계 기준 마우스를 4사분면에 놓았을 때
	if (fTargetAngle >= 0.f && fTargetAngle < PI * 0.5f)
		this->SetPos(static_cast<float>(m_pTarget->GetRect().left), vTargetPoint.fY + vOffset.fY);
	// 3사분면 
	else if (fTargetAngle >= PI * 0.5f && fTargetAngle < PI)
		this->SetPos(static_cast<float>(m_pTarget->GetRect().right), vTargetPoint.fY + vOffset.fY);
	// 2사분면
	else if (fTargetAngle >= -PI && fTargetAngle < -PI * 0.5f)
		this->SetPos(static_cast<float>(m_pTarget->GetRect().right) - vOffset.fX, vTargetPoint.fY + vOffset.fY);
	// 1사분면
	else
		this->SetPos(static_cast<float>(m_pTarget->GetRect().left) + vOffset.fX, vTargetPoint.fY + vOffset.fY);

	__super::UpdateRect();
    return NOEVENT;
}

void CShield::LateUpdate()
{

}

void CShield::Render(Graphics* pGraphics)
{
	VEC    vScroll = CCameraMgr::GetInstance()->GetScroll();
	Image* pShieldImg = CImgMgr::GetInstance()->FindImg(L"Shield_Tier1");
	if (nullptr == pShieldImg)
		return;

	switch (*m_pWeaponState)
	{
	case SWORD_AND_SHIELD_STATE::IDLE:
	case SWORD_AND_SHIELD_STATE::ATTACK:
		HandleIdleRender(pGraphics, pShieldImg, vScroll);
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

void CShield::Release()
{
}

#pragma region HandleRender
void CShield::HandleIdleRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
	VEC vDrawSize = m_vCellSize * PIXEL_SCALE;
	RectF DestRect = { m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
					   m_tInfo.vPoint.fY - vDrawSize.fY * 0.5f + vScroll.fY,
					   vDrawSize.fX, vDrawSize.fY };

	pGraphics->DrawImage(
		pImg, DestRect,
		0.f, 0.f, m_vCellSize.fX, m_vCellSize.fY, UnitPixel);
}

void CShield::HandleAttackRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
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

void CShield::HandleAttack1Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}

void CShield::HandleAttack2Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}

void CShield::HandleAttack3Render(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}

void CShield::HandleShieldRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
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

void CShield::HandleCleaveRender(Graphics* pGraphics, Image* pImg, VEC& vScroll)
{
}

#pragma endregion HandleRender
