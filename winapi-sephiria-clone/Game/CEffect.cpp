#include "pch.h"
#include "CEffect.h"

#include "CImgMgr.h"
#include "CTimeMgr.h"
#include "CCameraMgr.h"

CEffect::CEffect()
	: m_fAlpha(1.f)
{
}

CEffect::~CEffect()
{
}

void CEffect::Initialize()
{
	m_eRender = RENDERID::EFFECT;
	m_iRenderLayer = 1;
}

int CEffect::Update()
{
	if (m_bDead)
		return DEAD;

	UpdateFrame();
	__super::UpdateRect();

	return NOEVENT;
}

void CEffect::LateUpdate()
{
}

void CEffect::Render(Graphics* pGraphics)
{

	Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
	VEC vImgSize = m_tInfo.vSize * PIXEL_SCALE;

	ColorMatrix colorMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, m_fAlpha, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	ImageAttributes imageAtt;
	imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	Matrix matRot;
	float fDegree = m_fAngle * 180.f / PI;
	matRot.RotateAt(fDegree, { m_tInfo.vPoint.fX + vScroll.fX, m_tInfo.vPoint.fY + vScroll.fY });
	pGraphics->SetTransform(&matRot); // DC 회전

	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX,
				  m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY,
					vImgSize.fX,
					vImgSize.fY };

	pGraphics->DrawImage(
		pImg, rcDest,
		m_tInfo.vSize.fX * m_tFrame.iStart,
		m_tInfo.vSize.fY * m_tFrame.iMotion,
		m_tInfo.vSize.fX,
		m_tInfo.vSize.fY,
		UnitPixel,
		&imageAtt
	);

	pGraphics->ResetTransform();
}

void CEffect::Release()
{
}

void CEffect::UpdateFrame()
{
	if (m_tFrame.dFrameSpeed <= CTimeMgr::GetInstance()->GetTime(m_tFrame.iFrameTime))
	{
		++m_tFrame.iStart;

		QueryPerformanceCounter(&m_tFrame.iFrameTime);

		if (m_tFrame.iStart > m_tFrame.iEnd)
			m_bDead = true;
	}
}
