#include "pch.h"
#include "CInventoryUI.h"

#include "CMouse.h"

#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CInventory.h"
#include "CInventorySlotUI.h"
#include "CCollisionMgr.h"
#include "CAbstractFactory.h"
#include "CKeyMgr.h"

CInventoryUI::CInventoryUI()
	: m_pInventory(nullptr), m_iStartSlot(-1), m_iMouseHoverSlot(-1), m_bDrag(false)
{
}

CInventoryUI::~CInventoryUI()
{
	Release();
}

void CInventoryUI::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;
	
	// 이 UI는 중점 좌표보다는 오히려 왼쪽 상단 위치로 두는게 slot 배치시 코드가 더 깔끔할거 같아서
	// m_tInfo.vPoint가 중점이 아니라 LT임
	m_tInfo = { 600.f, (WINCY >> 1) - 169.f * 0.5f * m_fUIScale, 0.f, 0.f };

	// 스프라이트 넣기 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Inventory/InventoryBase.png", L"Inventory_Base");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Inventory/InventorySlot_Blank.png", L"Inventory_Slot_Blank");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Inventory/InventorySlot_Item.png", L"Inventory_Slot_Item");

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 4;    
}

int CInventoryUI::Update()
{
	if (!m_bView)
		return NOEVENT;

	for (auto& slot : m_vecItemSlot)
		slot->Update();

	// 마우스 포인터와 충돌하는 슬롯이 없는 경우에는 -1을 반환해줌
	m_iMouseHoverSlot = CCollisionMgr::GetCollisionSlotIndex(m_pMouse->GetInfo().vPoint, m_vecItemSlot);

	// Collide 체크를 해줌
	// 사족 - 어차피 ItemSlot도 마우스 객체를 참조하고 있으니깐 객체 위치로 내부에서 판단하는게 좋지 않나요?
	//      - 일리가 있어서 고민했으나, 어차피 충돌하는 슬롯 인덱스를 InventoryUI가 알아야 해서 충돌 검사를 두번하게 되는 느낌이었음
	//		- 그래서 여기서 제어하게 함
	for (int i = 0; i < m_vecItemSlot.size(); ++i)
	{
		m_vecItemSlot[i]->SetCollide(i == m_iMouseHoverSlot);
	}

	// 드래그 시작
	if (KEY_DOWN(VK_LBUTTON) && m_iMouseHoverSlot != -1)
	{
		CItem* pDragItem = m_pInventory->GetItem(m_iMouseHoverSlot);

		// 빈 슬롯이 아닌 경우에만 시작
		if (pDragItem != nullptr)
		{
			// 마우스가 현재 드래그 중인 아이템을 참조하게 해줌 - 렌더용
			m_pMouse->SetDragItem(pDragItem);

			m_iStartSlot = m_iMouseHoverSlot;
			m_bDrag = true;

			// 원래 슬롯은 안보이게 함
			m_vecItemSlot[m_iStartSlot]->SetItem(nullptr);
		}
	}

	// 드래그 끝
	if (KEY_UP(VK_LBUTTON) && m_bDrag)
	{
		// 이동해야되는 인덱스 정리
		int iStartIdx = m_iStartSlot;
		int iEndIdx = m_iMouseHoverSlot;

		// 마우스가 놓은 곳이 현재와 다른 슬롯이라면!
		if (iEndIdx != -1 && iEndIdx != iStartIdx)
		{
			// 아이템 이동
			m_pInventory->MoveItem(iStartIdx, iEndIdx);
		}

		// SyncInventorySlot이 LateUpdate에서 Slot 동기화 해줘서 별도로 할 것 없음

		// 초기화
		m_pMouse->SetDragItem(nullptr);
		m_iStartSlot = -1;
		m_bDrag = false;
	}

	return NOEVENT;
}

void CInventoryUI::LateUpdate()
{
	if (!m_bView)
		return;

	SyncInventorySlot();
}

void CInventoryUI::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	// 배경 그리기
	Image* pInventoryBaseImg = CImgMgr::GetInstance()->FindImg(L"Inventory_Base");
	if (nullptr == pInventoryBaseImg)
		return;

	VEC vCellSize = { 244.f, 169.f };
	VEC vImgSize = vCellSize * m_fUIScale;
	RectF rcDest = { m_tInfo.vPoint.fX, m_tInfo.vPoint.fY, vImgSize.fX, vImgSize.fY};
	pGraphics->DrawImage(pInventoryBaseImg, rcDest, 0, 0, vCellSize.fX, vCellSize.fY, UnitPixel);

	// 슬롯 & 레벨 그리기
	for (auto& slot : m_vecItemSlot)
		slot->Render(pGraphics);
}

void CInventoryUI::Release()
{
	for_each(m_vecItemSlot.begin(), m_vecItemSlot.end(), SafeDelete<CInventorySlotUI*>);
	m_vecItemSlot.clear();
	m_vecItemSlot.shrink_to_fit();
}

void CInventoryUI::SetInventorySize(int iSize)
{
	Release();

	float fSlotSize = 32.f;
	VEC vGap = { 2.f, 1.5f };
	VEC vStep = VEC{ fSlotSize + vGap.fX, fSlotSize + vGap.fY } * m_fUIScale;

	// 첫 슬롯 위치
	const VEC vOffset{ (21.f + fSlotSize * 0.5f) * m_fUIScale,
					   (23.f + fSlotSize * 0.5f) * m_fUIScale };

	m_vecItemSlot.resize(iSize, nullptr);

	for (int i = 0; i < iSize; ++i)
	{
		const VEC vPoint{
			m_tInfo.vPoint.fX + vOffset.fX + vStep.fX * (i % INVEN_COL),
			m_tInfo.vPoint.fY + vOffset.fY + vStep.fY * (i / INVEN_COL)
		};
		m_vecItemSlot[i] = static_cast<CInventorySlotUI*>(CAbstractFactory<CInventorySlotUI>::CreateUI(vPoint.fX,vPoint.fY,m_pMouse));
	}
}

void CInventoryUI::SetInventory(CInventory* pInventory)
{
	m_pInventory = pInventory;
	SetInventorySize(m_pInventory->GetInventorySize());
}

void CInventoryUI::SyncInventorySlot()
{
	// 인벤토리 슬롯 싱크 맞추기
	for (int i = 0; i < m_vecItemSlot.size(); ++i)
	{
		m_vecItemSlot[i]->SetItem(m_pInventory->GetItem(i));
	}
}

