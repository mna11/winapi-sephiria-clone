#include "pch.h"
#include "CShopTableSlot.h"

#include "CImgMgr.h"
#include "CItemData.h"
#include "CFontMgr.h"

CShopTableSlot::CShopTableSlot()
	: m_iItemID(-1), m_bCol(false)
{
}

CShopTableSlot::~CShopTableSlot()
{
	Release();
}

void CShopTableSlot::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;
	m_tInfo = { 0.f, 0.f, 144.f * m_fUIScale, 42.f * m_fUIScale };

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 2;
}

int CShopTableSlot::Update()
{
	__super::UpdateRect();
    return NOEVENT;
}

void CShopTableSlot::LateUpdate()
{
}

void CShopTableSlot::Render(Graphics* pGraphics)
{
	Image* pImg(nullptr);
	VEC vCellSize{ 144.f, 42.f };
	VEC vImgSize = vCellSize * m_fUIScale;

	m_pFrameKey = L"ShopListSlot";
	pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f,
					vImgSize.fX,
					vImgSize.fY };

	pGraphics->DrawImage(
		pImg, rcDest,
		(m_bCol ? vCellSize.fX : 0),
		0,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);


	if (-1 == m_iItemID)
		return;

	// 아이템 그리기
	const ITEM_INFO* pItemInfo = CItemData::GetInstance()->FindItemInfo(m_iItemID);
	VEC vOffset{};
	VEC vRectSize{};
	

	// 아이템 프레임 씌우기
	m_pFrameKey = L"Inventory_Slot_Item";
	pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;
	vOffset = { 6.f, 5.f };
	VEC vItemFrameCellSize = { 32.f, 32.f };
	VEC vItemFrameImgSize = vItemFrameCellSize * m_fUIScale;
	rcDest = { m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vOffset.fX,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vOffset.fY,
					vItemFrameImgSize.fX + 15.f,
					vItemFrameImgSize.fY + 15.f };

	pGraphics->DrawImage(
		pImg, rcDest,
		0,
		0,
		vItemFrameCellSize.fX,
		vItemFrameCellSize.fY,
		UnitPixel
	);


	// 아이템 이미지 그리기 - 나중에 살짝 작게 조정하자
	Image* pItemImg = CImgMgr::GetInstance()->FindImg(pItemInfo->strImg.c_str());

	pGraphics->DrawImage(
		pItemImg, rcDest,
		0,
		0,
		vItemFrameCellSize.fX,
		vItemFrameCellSize.fY,
		UnitPixel
	);

	// 텍스트 그리기

	// 아이템 이름 텍스트 그리기
	vOffset = VEC{ -30.f, -17.f } * m_fUIScale;
	vRectSize = VEC{ 95.f, 18.f } *m_fUIScale;
	rcDest = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush GreenBrush(Color{ 255, 0, 128, 0 });
	pGraphics->FillRectangle(&GreenBrush, rcDest);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, pItemInfo->strName, FONT_TYPE::NORMAL, rcDest, Color { 255, 255, 255, 255 }, 24.f, StringAlignmentNear, StringAlignmentNear);

	// 아이템 가격 텍스트 그리기
	vOffset = VEC{ 15.f, 2.f } * m_fUIScale;
	vRectSize = VEC{ 40.f, 15.f } * m_fUIScale;
	rcDest = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush RedBrush(Color{ 255, 128, 0, 0 });
	pGraphics->FillRectangle(&RedBrush, rcDest);
#endif // _DEBUG
	RectF rcBackStringDest = rcDest;
	rcBackStringDest.X += 2.5f;
	rcBackStringDest.Y += 2.5f;
	CFontMgr::GetInstance()->DrawString(pGraphics, to_wstring(pItemInfo->iLeaf), FONT_TYPE::PIXEL_BIG, rcBackStringDest, Color{ 255, 0, 0, 0 }, 22.f, StringAlignmentFar, StringAlignmentFar);
	CFontMgr::GetInstance()->DrawString(pGraphics, to_wstring(pItemInfo->iLeaf), FONT_TYPE::PIXEL_BIG, rcDest, Color{ 255, 238, 207, 116 }, 22.f, StringAlignmentFar, StringAlignmentFar);

	// 리프 아이콘 그리기
	pImg = CImgMgr::GetInstance()->FindImg(L"Leaf");
	if (nullptr == pImg)
		return;

	vOffset = VEC{ 55.f, 6.f } * m_fUIScale;
	vCellSize = { 10.f, 10.f };
	vImgSize = vCellSize * m_fUIScale;
	rcDest = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vImgSize.fX, vImgSize.fY };
#ifdef _DEBUG
	pGraphics->FillRectangle(&RedBrush, rcDest);
#endif // _DEBUG
	pGraphics->DrawImage(
		pImg, rcDest,
		0,
		0,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);
}

void CShopTableSlot::Release()
{
}
