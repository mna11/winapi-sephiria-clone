#include "pch.h"
#include "CSword.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"

CSword::CSword()
	: m_vCellSize{ 9.f, 16.f }
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
	__super::UpdateRect();
	return NOEVENT;
}

void CSword::LateUpdate()
{
}

void CSword::Render(Graphics* pGraphics)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

#ifdef _DEBUG
	SolidBrush blackBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&blackBrush, (int)(m_tRect.left + vScroll.fX),
		(int)(m_tRect.top + vScroll.fY),
		(int)m_tInfo.vSize.fX,
		(int)m_tInfo.vSize.fY);
#endif // _DEBUG

	Image* pSwordImg = CImgMgr::GetInstance()->FindImg(L"Sword_Tier1");
	if (nullptr == pSwordImg)
		return;

	float fTargetAngle = m_pTarget->GetAngle();
	
	// 2사분면, 3사분면 - 여기서 분면 기준은 카테시안 좌표계
	if (fabsf(fTargetAngle) > PI * 0.5f)
	{
		fTargetAngle = fTargetAngle - PI;
	}
	fTargetAngle *= 180 / PI;

	Matrix matRot;
	matRot.RotateAt(fTargetAngle, { m_tInfo.vPoint.fX + vScroll.fX, m_tRect.bottom + vScroll.fY });
	pGraphics->SetTransform(&matRot); // DC 회전

	VEC vDrawSize = m_vCellSize * PIXEL_SCALE;
	RectF DestRect = { m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
					   m_tInfo.vPoint.fY - vDrawSize.fY * 0.5f + vScroll.fY,
					   vDrawSize.fX, vDrawSize.fY };

	pGraphics->DrawImage(
		pSwordImg, DestRect,
		0.f, 0.f, m_vCellSize.fX, m_vCellSize.fY, UnitPixel);

	pGraphics->ResetTransform();
}

void CSword::Release()
{
}
