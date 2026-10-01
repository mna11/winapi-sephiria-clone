#include "pch.h"
#include "CMouse.h"

#include "CItem.h"

#include "CUIMgr.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CItemData.h"

CMouse::CMouse()
	: CState(MOUSE_STATE::END, MOUSE_STATE::COMBAT),
	m_dStateTime(0.), m_dClickTime(0.)
{
	m_tHoverReferItem = { -1, ITEM_SOURCE::END };
	m_tDragReferItem = { -1, ITEM_SOURCE::END };
}

CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	m_tInfo = { 0.f, 0.f, 0.f, 0.f };
	m_eRender = RENDERID::MOUSE;
	m_iRenderLayer = 5;

	// 스프라이트 넣기
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Cursor/Cursor_UI.png", L"Cursor_UI");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Cursor/Cursor_Combat.png", L"Cursor_Combat");

	// 반투명 이미지 Attr 정의 
	ColorMatrix colorMatrix = {	1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.7f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	m_imgAttrTranslucent.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);


	m_dClickTime = 0.5;
}

int CMouse::Update()
{
	UpdateTime();

	ApplyChange();

	UpdatePoint();

	__super::UpdateFrame();
	return NOEVENT;
}

void CMouse::LateUpdate()
{
	UpdateClick();
}

void CMouse::Render(Graphics* pGraphics)
{
	Image* pImg(nullptr);
	VEC vCellSize{};
	VEC vImgSize{};
	RectF DestRect{};

	if (-1 != m_tDragReferItem.iID)
	{
		const ITEM_INFO* pItemInfo = CItemData::GetInstance()->FindItemInfo(m_tDragReferItem.iID);
		pImg = CImgMgr::GetInstance()->FindImg(pItemInfo->strImg.c_str());
		vCellSize = { 32.f, 32.f };
		vImgSize = vCellSize * PIXEL_SCALE * 0.5f;

		RectF rcDest{
				m_tInfo.vPoint.fX - vImgSize.fX * 0.5f,
				m_tInfo.vPoint.fY - vImgSize.fY * 0.5f,
				vImgSize.fX,
				vImgSize.fY
		};

		pGraphics->DrawImage(
			pImg,
			rcDest,
			0,
			0,
			vCellSize.fX,
			vCellSize.fY,
			UnitPixel,
			&m_imgAttrTranslucent
		);
	}

	// 마우스 커서 종류별 렌더링
	pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	switch (m_eCurState)
	{
	case MOUSE_STATE::COMBAT:
		vCellSize = { 27.f, 27.f };
		vImgSize = vCellSize * PIXEL_SCALE * 0.5f;
		DestRect = { m_tInfo.vPoint.fX - vImgSize.fX * 0.5f, m_tInfo.vPoint.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };
		pGraphics->DrawImage(
			pImg, DestRect,
			0, 0,
			vCellSize.fX, vCellSize.fY,
			UnitPixel
		);
		break;
	case MOUSE_STATE::UI_IDLE:
		vCellSize = { 18.f, 18.f };
		vImgSize = vCellSize * PIXEL_SCALE * 0.5f;
		DestRect = { m_tInfo.vPoint.fX, m_tInfo.vPoint.fY, vImgSize.fX, vImgSize.fY };
		pGraphics->DrawImage(
			pImg, DestRect,
			0, 0,
			vCellSize.fX, vCellSize.fY,
			UnitPixel
		);
		break;
	case MOUSE_STATE::UI_CLICK_DOWN:
	{
		vCellSize = { 18.f, 18.f };
		vImgSize = vCellSize * PIXEL_SCALE * 0.5f;
		DestRect = { m_tInfo.vPoint.fX, m_tInfo.vPoint.fY, vImgSize.fX, vImgSize.fY };
		// 아틀라스 이미지에서 두번째 이미지가 클릭시 이미지임
		pGraphics->DrawImage(
			pImg, DestRect,
			vCellSize.fX * 1, vCellSize.fY * m_tFrame.iMotion,
			vCellSize.fX, vCellSize.fY,
			UnitPixel
		);
		break;
	}
	case MOUSE_STATE::UI_CLICK_UP:
	{
		vCellSize = { 18.f, 18.f };
		vImgSize = vCellSize * PIXEL_SCALE * 0.5f;
		DestRect = { m_tInfo.vPoint.fX, m_tInfo.vPoint.fY, vImgSize.fX, vImgSize.fY };
		pGraphics->DrawImage(
			pImg, DestRect,
			vCellSize.fX * m_tFrame.iStart, vCellSize.fY * m_tFrame.iMotion,
			vCellSize.fX, vCellSize.fY,
			UnitPixel
		);
		break;
	}
	default:
		break;
	}
}

void CMouse::Release()
{

}

void CMouse::UpdateTime()
{
	m_dStateTime += DT;
	
	if (m_eCurState == MOUSE_STATE::UI_CLICK_UP && m_dStateTime > m_dClickTime)
		m_eNextState = MOUSE_STATE::UI_IDLE;
}

void CMouse::UpdatePoint()
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	m_tInfo.vPoint = { (float)ptMouse.x, (float)ptMouse.y };
}

void CMouse::UpdateClick()
{
	if (KEY_DOWN(VK_LBUTTON) && m_eCurState == MOUSE_STATE::UI_IDLE)
	{
		m_eNextState = MOUSE_STATE::UI_CLICK_DOWN;
	}

	if (KEY_UP(VK_LBUTTON) && m_eCurState == MOUSE_STATE::UI_CLICK_DOWN)
	{
		m_eNextState = MOUSE_STATE::UI_CLICK_UP;
	}
}

void CMouse::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case MOUSE_STATE::COMBAT:
			m_pFrameKey = L"Cursor_Combat";
			break;
		case MOUSE_STATE::UI_IDLE:
			m_pFrameKey = L"Cursor_UI";
			break;
		case MOUSE_STATE::UI_CLICK_DOWN:
			m_pFrameKey = L"Cursor_UI";
			break;
		case MOUSE_STATE::UI_CLICK_UP:
			m_pFrameKey = L"Cursor_UI";
			SetFrame(1, 6, 0, m_dClickTime / 6.);
			break;
		default:
			break;
		}

		m_eCurState = m_eNextState;
		m_dStateTime = 0.;
	}
}
