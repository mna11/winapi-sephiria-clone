#include "pch.h"
#include "CSephirite.h"

#include "CPlayer.h"

#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CUIMgr.h"
#include "CFontMgr.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"

CSephirite::CSephirite()
	: m_bCol(false)
{
	ZeroMemory(&m_tInteractRect, sizeof(RECT));
}

CSephirite::~CSephirite()
{
	Release();
}

void CSephirite::Initialize()
{
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Sephirite/Sephirite.png", L"Sephirite");

	// 기초 정보 초기화
	m_tInfo = { 0.f, 0.f, 12.f * PIXEL_SCALE, 15.f * PIXEL_SCALE };
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 0;

	m_pFrameKey = L"Sephirite";
	SetFrame(0, 19, 0, 0.1);
}

int CSephirite::Update()
{
	if (m_bDead)
		return DEAD;

	HandleInteraction();

	UpdateInteractRect();

	__super::UpdateRect();
	__super::UpdateFrame();
	return NOEVENT;
}

void CSephirite::LateUpdate()
{
}

void CSephirite::Render(Graphics* pGraphics)
{
	Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

#ifdef _DEBUG
	// 상호작용 렉트
	SolidBrush blackBrush(Color(255, 0, 0, 0));
	pGraphics->FillRectangle(&blackBrush, (int)(m_tInteractRect.left + vScroll.fX),
		(int)(m_tInteractRect.top + vScroll.fY),
		(int)(m_tInteractRect.right - m_tInteractRect.left),
		(int)(m_tInteractRect.bottom - m_tInteractRect.top)
	);
	// 충돌 렉트
	SolidBrush whiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&whiteBrush, (int)(m_tRect.left + vScroll.fX),
		(int)(m_tRect.top + vScroll.fY),
		(int)m_tInfo.vSize.fX,
		(int)m_tInfo.vSize.fY);
#endif // _DEBUG

	VEC vCellSize = m_tInfo.vSize / PIXEL_SCALE;
	VEC vImgSize = m_tInfo.vSize;
	RectF rcDest = { m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX,
					 m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY,
					 vImgSize.fX, vImgSize.fY };

	pGraphics->DrawImage(pImg, rcDest,
		vCellSize.fX * m_tFrame.iStart,
		vCellSize.fY * m_tFrame.iMotion,
		vCellSize.fX, vCellSize.fY, UnitPixel);


	if (m_bCol)
	{
		// 안내 그리기
		pImg = CImgMgr::GetInstance()->FindImg(L"KeyUI");

		float fKeyImgWidth = 15.f * PIXEL_SCALE;
		float fTextWidth = 10.f * PIXEL_SCALE;
		float fGap = 3.f * PIXEL_SCALE;
		float fAllWidth = fKeyImgWidth + fTextWidth + fGap;
		float fHeight = 10.f * PIXEL_SCALE;

		wstring strGet = L"줍기";
		RectF rcGet = { m_tInfo.vPoint.fX - fAllWidth * 0.5f + vScroll.fX, m_tInfo.vPoint.fY - vImgSize.fY * 0.7f - fHeight + vScroll.fY,
							fAllWidth, fHeight };
		// rcGet 안에 포함되는 위치
		RectF rcGetImg = rcGet;
		rcGetImg.Width = fKeyImgWidth;

	#ifdef _DEBUG
		SolidBrush BlackBrush(Color(255, 0, 0, 0));
		pGraphics->FillRectangle(&BlackBrush, rcGet);
		SolidBrush WhiteBrush(Color(255, 255, 255, 255));
		pGraphics->FillRectangle(&WhiteBrush, rcGetImg);
	#endif // _DEBUG
		vCellSize = { 13.f, 9.f };
		pGraphics->DrawImage(
			pImg, rcGetImg,
			3 * vCellSize.fX,
			3 * vCellSize.fY,
			vCellSize.fX,
			vCellSize.fY,
			UnitPixel
		);
		RectF rcBgStr = rcGet;
		rcBgStr.X += 2.f;
		rcBgStr.Y += 2.f;
		CFontMgr::GetInstance()->DrawString(pGraphics, strGet, FONT_TYPE::NORMAL, rcBgStr, Color{ 255, 0, 0, 0 }, 28.f, StringAlignmentFar);
		CFontMgr::GetInstance()->DrawString(pGraphics, strGet, FONT_TYPE::NORMAL, rcGet, Color{ 255, 255, 255, 255 }, 28.f, StringAlignmentFar);
	}
}

void CSephirite::Release()
{
}

void CSephirite::UpdateInteractRect()
{
	VEC vSize{ 100.f, 100.f };
	SetRect(&m_tInteractRect, m_tInfo.vPoint.fX - vSize.fX, m_tInfo.vPoint.fY - vSize.fY,
		m_tInfo.vPoint.fX + vSize.fX, m_tInfo.vPoint.fY + vSize.fY);
}

void CSephirite::HandleInteraction()
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	if (CCollisionMgr::CollisionRect(pPlayer->GetRect(), m_tInteractRect))
	{
		if (KEY_DOWN('F'))
		{
			CUIMgr::GetInstance()->ShowUI(UIID::ITEM_SELECT);
			m_bDead = true;
		}
		m_bCol = true;
	}
	else
	{
		m_bCol = false;
	}
}