#include "pch.h"
#include "CSword.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"

CSword::CSword()
	: m_vCellSize{ 9.f, 16.f },
	m_pWeaponState(nullptr), m_pAtk(nullptr)
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
	m_iRenderLayer = 1;
}

int CSword::Update()
{
	if (nullptr == m_pTarget)
		return NOEVENT;

	// 마우스 위치에 따른 위치 세팅

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
	Image* pSwordImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pSwordImg)
		return;

	switch (*m_pWeaponState)
	{
	case SWORD_AND_SHIELD_STATE::IDLE:
		HandleIdleRender(pGraphics, pSwordImg, vScroll);
		break;
	case SWORD_AND_SHIELD_STATE::ATTACK:
		HandleAttackRender(pGraphics, pSwordImg, vScroll);
		break;
	case SWORD_AND_SHIELD_STATE::DEFENSE:
		HandleShieldRender(pGraphics, pSwordImg, vScroll);
		break;
	case SWORD_AND_SHIELD_STATE::CLEAVE:
		HandleCleaveRender(pGraphics, pSwordImg, vScroll);
		break;
	default:
		break;
	}
}

void CSword::Release()
{
}

void CSword::DrawSword(Graphics* pGraphics, Image* pImg, VEC& vScroll, float fTargetAngle)
{
	Matrix matRot;
	matRot.RotateAt(fTargetAngle, { m_tInfo.vPoint.fX + vScroll.fX, m_tInfo.vPoint.fY + vScroll.fY });
	pGraphics->SetTransform(&matRot); // DC 회전

	// 회전된 DC에다가 그리기
	// 그릴 때, 현재 m_tInfo가 검 bottom center가 되도록 그림 -> 검 손잡이 기준 회전하는 것처럼 보이게 구현
	VEC vDrawSize = m_vCellSize * PIXEL_SCALE;
	RectF DestRect = { m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
					   m_tRect.top - vDrawSize.fY * 0.5f + vScroll.fY,
					   vDrawSize.fX, vDrawSize.fY };

	pGraphics->DrawImage(
		pImg, DestRect,
		0.f, 0.f, m_vCellSize.fX, m_vCellSize.fY, UnitPixel);

	pGraphics->ResetTransform();
}