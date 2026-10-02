#include "pch.h"
#include "CLevelUp.h"

#include "CPlayer.h"

#include "CItemSelectUI.h"

#include "CImgMgr.h"
#include "CFontMgr.h"
#include "CUIMgr.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"

CLevelUp::CLevelUp()
{
}

CLevelUp::~CLevelUp()
{
	Release();
}

void CLevelUp::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.6f;

	m_tInfo = { WINCX >> 1, WINCY * 0.7f, 0.f, 0.f };

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 0;

	m_pFrameKey = L"LevelUp";
	SetFrame(0, 12, 0, 0.1);
}

int CLevelUp::Update()
{
	if (m_bDead)
		return DEAD;
	if (!m_bView)
		return NOEVENT;

	KeyInput();

	__super::UpdateFrame();
	return NOEVENT;
}

void CLevelUp::LateUpdate()
{
	if (!m_bView)
		return;
}

void CLevelUp::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

	VEC vCellSize{ 30.f, 30.f };
	VEC vImgSize = vCellSize * m_fUIScale;

	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f,
					vImgSize.fX,
					vImgSize.fY };

	pGraphics->DrawImage(
		pImg, rcDest,
		vCellSize.fX * m_tFrame.iStart,
		vCellSize.fY * m_tFrame.iMotion,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);

	// 안내 그리기
	pImg = CImgMgr::GetInstance()->FindImg(L"KeyUI");

	float fKeyImgWidth = 15.f * m_fUIScale;
	float fTextWidth = 30.f * m_fUIScale;
	float fGap = 3.f * m_fUIScale;
	float fAllWidth = fKeyImgWidth + fTextWidth + fGap;
	float fHeight = 10.f * m_fUIScale;

	wstring strLevelUp = L"레벨업";
	RectF rcLevelUp = { m_tInfo.vPoint.fX - fAllWidth * 0.5f, m_tInfo.vPoint.fY + vImgSize.fY * 0.6f,
						fAllWidth, fHeight };
	// rcLevelUp 안에 포함되는 위치
	RectF rcKeyImg = rcLevelUp;
	rcKeyImg.Width = fKeyImgWidth;



#ifdef _DEBUG
	SolidBrush BlackBrush(Color(255, 0, 0, 0));
	pGraphics->FillRectangle(&BlackBrush, rcLevelUp);
	SolidBrush WhiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&WhiteBrush, rcKeyImg);
#endif // _DEBUG
	vCellSize = { 13.f, 9.f };
	pGraphics->DrawImage(
		pImg, rcKeyImg,
		3 * vCellSize.fX,
		2 * vCellSize.fY,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);
	RectF rcBgStr = rcLevelUp;
	rcBgStr.X += 2.f;
	rcBgStr.Y += 2.f;
	CFontMgr::GetInstance()->DrawString(pGraphics, strLevelUp, FONT_TYPE::NORMAL, rcBgStr, Color{ 255, 0, 0, 0 }, 28.f, StringAlignmentFar);
	CFontMgr::GetInstance()->DrawString(pGraphics, strLevelUp, FONT_TYPE::NORMAL, rcLevelUp, Color{ 255, 255, 255, 255 }, 28.f, StringAlignmentFar);
}

void CLevelUp::Release()
{
}

void CLevelUp::KeyInput()
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	if (KEY_DOWN('R'))
	{
		Hide();
		CUIMgr::GetInstance()->ShowUI(UIID::ITEM_SELECT);
		static_cast<CItemSelectUI*>(CUIMgr::GetInstance()->GetUI(UIID::ITEM_SELECT))->RequestChange(ITEM_SELECT_UI_STATE::SELECT);
		pPlayer->SetCanLevelUp(false);
	}
}