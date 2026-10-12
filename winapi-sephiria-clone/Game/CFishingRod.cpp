#include "pch.h"
#include "CFishingRod.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"

CFishingRod::CFishingRod()
{
}

CFishingRod::~CFishingRod()
{
	Release();
}

void CFishingRod::Initialize()
{
	m_tInfo = { 0.f, 0.f, 20.f * PIXEL_SCALE, 20.f * PIXEL_SCALE };
	m_iID = 0;

	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 1; 

	m_eWeaponType = WEAPON_TYPE::FISHING_ROD;
	m_iID = 3;
}

int CFishingRod::Update()
{
	if (m_bDead)
		return DEAD;

	UpdatePos();
	__super::UpdateRect();
	return NOEVENT;
}

void CFishingRod::LateUpdate()
{
}

void CFishingRod::Render(Graphics* pGraphics)
{
	Image* pImg = CImgMgr::GetInstance()->FindImg(L"Fishing_Rod_Rod");
	if (nullptr == pImg)
		return;

	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

	float fTargetAngle = m_pTarget->GetAngle();
	fTargetAngle *= 180 / PI;
	bool bFaceLeft = fabsf(fTargetAngle) > 90.f;
	if (bFaceLeft)
		fTargetAngle += 180.f;

	VEC vPivot = VEC{ m_tInfo.vPoint.fX, m_tInfo.vPoint.fY } + vScroll;

	Matrix matTransform;
	matTransform.Translate(m_tInfo.vSize.fX * 0.5f, -m_tInfo.vSize.fY * 0.5f, MatrixOrderAppend);
	matTransform.Scale(bFaceLeft ? -1.f : 1.f, 1.f, MatrixOrderAppend);
	matTransform.Rotate(fTargetAngle, MatrixOrderAppend);
	matTransform.Translate(vPivot.fX, vPivot.fY, MatrixOrderAppend);
	pGraphics->SetTransform(&matTransform); // DC 회전

	VEC vCellSize = { 20.f, 20.f };
	VEC vDrawSize = VEC{ 16.f, 16.f } * PIXEL_SCALE;
	RectF DestRect = { -vDrawSize.fX * 0.5f, -vDrawSize.fY * 0.5f, vDrawSize.fX, vDrawSize.fY };

#ifdef _DEBUG
	SolidBrush blackBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&blackBrush, DestRect);
#endif // _DEBUG

	pGraphics->DrawImage(
		pImg, DestRect,
		0.f, 0.f, vCellSize.fX, vCellSize.fY, UnitPixel);

	pGraphics->ResetTransform();
}

void CFishingRod::Release()
{
}

void CFishingRod::UpdatePos()
{
	// 마우스 위치에 따른 위치 세팅

	if (nullptr == m_pTarget)
		return;

	float fTargetAngle = m_pTarget->GetAngle();
	VEC vTargetPoint = m_pTarget->GetInfo().vPoint;
	VEC vOffset = m_tInfo.vSize * 0.2;

	// 4사분면
	if (fTargetAngle >= 0.f && fTargetAngle < PI * 0.5f)
		this->SetPos(static_cast<float>(m_pTarget->GetRect().right) - vOffset.fX * 1.25f, vTargetPoint.fY + vOffset.fY * 0.75f);
	// 3사분면 
	else if (fTargetAngle >= PI * 0.5f && fTargetAngle < PI)
		this->SetPos(static_cast<float>(m_pTarget->GetRect().left) + vOffset.fX * 1.25f, vTargetPoint.fY + vOffset.fY * 0.75f);
	// 2사분면
	else if (fTargetAngle >= -PI && fTargetAngle < -PI * 0.5f)
		this->SetPos(static_cast<float>(m_pTarget->GetRect().left) + vOffset.fX, vTargetPoint.fY + vOffset.fY);
	// 1사분면
	else
		this->SetPos(static_cast<float>(m_pTarget->GetRect().right) - vOffset.fX, vTargetPoint.fY + vOffset.fY);
}
