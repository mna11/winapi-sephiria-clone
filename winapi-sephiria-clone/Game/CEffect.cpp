#include "pch.h"
#include "CEffect.h"

#include "CImgMgr.h"
#include "CTimeMgr.h"
#include "CCameraMgr.h"
#include "CFontMgr.h"

CEffect::CEffect()
	: m_fAlpha(1.f)
{
	ZeroMemory(&m_vDir, sizeof(VEC));
	m_strPhrase.clear();
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
	// CEffectMgr::GetInstance()->CreateFollowEffect()로 만들어지면 타겟이 이동한 만큼 이동해줌
	if (m_pTarget != nullptr)
	{
		VEC vPrePoint = m_pTarget->GetPrePoint();
		VEC vCurPoint = m_pTarget->GetInfo().vPoint;

		m_tInfo.vPoint += vCurPoint - vPrePoint;
	}

	// CEffectMgr::GetInstance()->CreateMoveEffect()로 만들어지면 해당 방향 벡터 방향으로 이동함
	if (m_vDir.fX != 0.f && m_vDir.fY != 0.f)
	{
		m_tInfo.vPoint += m_vDir * m_fSpeed * DT;
	}
}

void CEffect::Render(Graphics* pGraphics)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

	if (!m_strPhrase.empty())
	{
		RectF rc{m_tInfo.vPoint.fX + vScroll.fX, m_tInfo.vPoint.fY + vScroll.fY, 200.f, 200.f };
		CFontMgr::GetInstance()->DrawString(pGraphics, m_strPhrase, FONT_TYPE::PIXEL_BOLD, rc, Color{ 255, 0, 0, 0 }, 35.f, StringAlignmentNear, StringAlignmentNear);
		CFontMgr::GetInstance()->DrawString(pGraphics, m_strPhrase, FONT_TYPE::PIXEL_BIG, rc, m_tColor, 30.f, StringAlignmentNear, StringAlignmentNear);
	}
	else
	{
		Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
		if (nullptr == pImg)
			return;

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
}

void CEffect::Release()
{
}

void CEffect::UpdateFrame()
{
	m_tFrame.dFrameElapsedTime += DT;
	if (m_tFrame.dFrameSpeed <= m_tFrame.dFrameElapsedTime)
	{
		++m_tFrame.iStart;
		m_tFrame.dFrameElapsedTime -= m_tFrame.dFrameSpeed;

		if (m_tFrame.iStart > m_tFrame.iEnd)
			m_bDead = true;
	}
}