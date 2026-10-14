#include "pch.h"
#include "CKeyGuide.h"

#include "CUIMgr.h"
#include "CImgMgr.h"
#include "CCameraMgr.h"
#include "CFontMgr.h"

CKeyGuide::CKeyGuide()
	: m_bFollow(false)
{
}

CKeyGuide::~CKeyGuide()
{
	Release();
}

void CKeyGuide::Initialize()
{
	m_tInfo = { 0.f, 0.f, 0.f, 0.f };

	m_eRender = RENDERID::UI;
	m_iRenderLayer = 0;
}

int CKeyGuide::Update()
{
	if (m_bDead)
		return DEAD;

	if (!m_bView)
		return NOEVENT;

	return NOEVENT;
}

void CKeyGuide::LateUpdate()
{
	if (!m_bView)
		return;

	if (m_bFollow)
	{
		if (nullptr == m_pTarget)
			return; 

		VEC vDifference = m_pTarget->GetInfo().vPoint - m_pTarget->GetPrePoint();
		m_tInfo.vPoint += vDifference;
	}
}

void CKeyGuide::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;
	
	// 안내 그리기
	Image* pImg = CImgMgr::GetInstance()->FindImg(L"KeyUI");
	if (nullptr == pImg)
		return; 

	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
	float fWidth = m_tInfo.vSize.fX * PIXEL_SCALE;
	float fHeight = m_tInfo.vSize.fY * PIXEL_SCALE;

	float fKeyImgWidth = fWidth * 0.5f;
	float fTextWidth = fWidth * 0.4f;
	float fGap = fWidth * 0.1f;

	RectF DestRect = { m_tInfo.vPoint.fX - fWidth * 0.5f + m_vOffset.fX + vScroll.fX, m_tInfo.vPoint.fY - fHeight * 0.5f + m_vOffset.fY + vScroll.fY, fWidth, fHeight};
	RectF DestRectKey = DestRect;
	DestRectKey.Width = fKeyImgWidth;

#ifdef _DEBUG
	SolidBrush BlackBrush(Color(255, 0, 0, 0));
	pGraphics->FillRectangle(&BlackBrush, DestRect);
	SolidBrush WhiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&WhiteBrush, DestRectKey);
#endif // _DEBUG

	VEC vCellSize = { 13.f, 9.f };
	pGraphics->DrawImage(
		pImg, DestRectKey,
		m_vKeyImgPoint.fX * vCellSize.fX,
		m_vKeyImgPoint.fY * vCellSize.fY,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);

	RectF DestBgStr = DestRect;
	DestBgStr.X += 2.f;
	DestBgStr.Y += 2.f;
	CFontMgr::GetInstance()->DrawString(pGraphics, m_strGuide, FONT_TYPE::NORMAL, DestBgStr, Color{ 255, 0, 0, 0 }, 28.f, StringAlignmentFar);
	CFontMgr::GetInstance()->DrawString(pGraphics, m_strGuide, FONT_TYPE::NORMAL, DestRect, Color{ 255, 255, 255, 255 }, 28.f, StringAlignmentFar);
}

void CKeyGuide::Release()
{
}

void CKeyGuide::Show()
{
	m_bView = true;

	m_strGuide.clear();
	m_vKeyImgPoint = {};
	m_vOffset = {};
}

void CKeyGuide::Hide()
{
	m_bView = false;

	m_strGuide.clear();
	m_vKeyImgPoint = {};
	m_vOffset = {};
}

void CKeyGuide::SetKeyGuideInfo(int vKey, wstring strGuide, VEC vOffset, VEC vSize)
{
	SetGuideKey(vKey);
	SetGuideString(strGuide);
	SetOffset(vOffset);
	SetSize(vSize);
}

void CKeyGuide::SetGuideKey(int vKey)
{
	switch (vKey)
	{
	case 'F':
		m_vKeyImgPoint = { 3.f, 3.f };
		break;
	case 'R':
		m_vKeyImgPoint = { 3.f, 2.f };
		break;
	case VK_SPACE:
		m_vKeyImgPoint = { 9.f, 6.f };
		break;
	case VK_LBUTTON:
		m_vKeyImgPoint = { 10.f, 6.f };
		break;
	default:
		break;
	}
}

void CKeyGuide::ShowGuide(CObj* pObj)
{
	if (!pObj)
		return;

	m_pTarget = pObj;
	SetPos(m_pTarget->GetInfo().vPoint);

	Show();
}

void CKeyGuide::HideGuide(CObj* pObj)
{
	if (m_pTarget == pObj)
	{
		m_pTarget = nullptr;
		Hide();
	}

}
