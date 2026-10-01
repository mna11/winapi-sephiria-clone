#include "pch.h"
#include "CShopTable.h"

#include "CMouse.h"
#include "CShopTableSlot.h"

#include "CAbstractFactory.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CUIMgr.h"
#include "CCollisionMgr.h"

CShopTable::CShopTable()
	: m_iItemSlotSize(4), m_iStartSlot(-1), m_iMouseHoverSlot(-1), m_bDrag(false)
{
	m_vecItemSlot.reserve(m_iItemSlotSize);
}

CShopTable::~CShopTable()
{
	Release();
}

void CShopTable::Initialize()
{
	m_tInfo = { 50.f, 14.f, 0.f, 0.f};

	m_eRender = RENDERID::UI;
	m_iRenderLayer = 3;
	m_fUIScale = PIXEL_SCALE * 0.5f;

	SetSellingItem();
}

int CShopTable::Update()
{
	if (!m_bView)
		return NOEVENT;

	for (auto& slot : m_vecItemSlot)
		slot->Update();

	// 마우스 포인터와 충돌하는 슬롯이 없는 경우에는 -1을 반환해줌
	m_iMouseHoverSlot = CCollisionMgr::GetCollisionSlotIndex<CShopTableSlot>(m_pMouse->GetInfo().vPoint, m_vecItemSlot);

	for (int i = 0; i < m_vecItemSlot.size(); ++i)
	{
		bool bCol = (i == m_iMouseHoverSlot && -1 != m_vecItemSlot[i]->GetItemID());
		m_vecItemSlot[i]->SetCollide(bCol);
	}

	// 마우스 호버 아이템 세팅
	//if (-1 != m_iMouseHoverSlot)
	//{
	//	//m_pMouse->SetHoverItem(m_pInventory->GetItem(m_iMouseHoverSlot));
	//	CUIMgr::GetInstance()->ShowUI(UIID::ITEM_TOOLTIP);
	//	CUIMgr::GetInstance()->SetPos(UIID::ITEM_TOOLTIP, VEC{ 210.f, 200.f });
	//}
	//else
	//{
	//	CUIMgr::GetInstance()->HideUI(UIID::ITEM_TOOLTIP);
	//}

	return NOEVENT;
}

void CShopTable::LateUpdate()
{
	if (!m_bView)
		return;

	for (auto& slot : m_vecItemSlot)
		slot->LateUpdate();
}

void CShopTable::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	// 배경 그리기
	Image* pInventoryBaseImg = CImgMgr::GetInstance()->FindImg(L"ShopList");
	if (nullptr == pInventoryBaseImg)
		return;

	VEC vCellSize = { 196.f, 277.f };
	VEC vImgSize = vCellSize * m_fUIScale;
	RectF rcDest = { m_tInfo.vPoint.fX, m_tInfo.vPoint.fY, vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(pInventoryBaseImg, rcDest, 0, 0, vCellSize.fX, vCellSize.fY, UnitPixel);

	for (auto& slot : m_vecItemSlot)
		slot->Render(pGraphics);
}

void CShopTable::Release()
{
}

void CShopTable::SetSellingItem()
{
	VEC vPoint = m_tInfo.vPoint + VEC{ 23.f + 144.f * 0.5f, 20.f + 42.f * 0.5f} * m_fUIScale;
	float fGap = 41.f * m_fUIScale;

	for (int i = 0; i < m_iItemSlotSize; ++i)
	{
		m_vecItemSlot.push_back(static_cast<CShopTableSlot*>(CAbstractFactory<CShopTableSlot>::CreateUI(vPoint.fX, vPoint.fY + fGap * i, m_pMouse)));
		m_vecItemSlot.back()->SetItemID(uniform_int_distribution<int>(0, 8)(g_engine));
	}
}
