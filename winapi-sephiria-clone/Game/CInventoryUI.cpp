#include "pch.h"
#include "CInventoryUI.h"

#include "CMouse.h"

#include "CShop.h"

#include "CUIMgr.h"
#include "CItemData.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CInventory.h"
#include "CInventorySlotUI.h"
#include "CCollisionMgr.h"
#include "CSceneMgr.h"
#include "CAbstractFactory.h"
#include "CKeyMgr.h"

CInventoryUI::CInventoryUI()
	: m_pInventory(nullptr), m_iStartSlot(-1), m_iMouseHoverSlot(-1)
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

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 3;    
}

int CInventoryUI::Update()
{
	if (!m_bView)
		return NOEVENT;

	if (m_bDead)
		return DEAD;

	for (auto& slot : m_vecItemSlot)
		slot->Update();

	// 마우스 포인터와 충돌하는 슬롯이 없는 경우에는 -1을 반환해줌
	m_iMouseHoverSlot = CCollisionMgr::GetCollisionSlotIndex<CInventorySlotUI>(m_pMouse->GetInfo().vPoint, m_vecItemSlot);

	// Collide 체크를 해줌
	// 사족 - 어차피 ItemSlot도 마우스 객체를 참조하고 있으니깐 객체 위치로 내부에서 판단하는게 좋지 않나요?
	//      - 일리가 있어서 고민했으나, 어차피 충돌하는 슬롯 인덱스를 InventoryUI가 알아야 해서 충돌 검사를 두번하게 되는 느낌이었음
	//		- 그래서 여기서 제어하게 함
	for (int i = 0; i < m_vecItemSlot.size(); ++i)
	{
		m_vecItemSlot[i]->SetCollide(i == m_iMouseHoverSlot);
	}

	// 인벤토리 업데이트하고 -> 상점 테이블 업데이트하는 이 순서가 현재 종속적인 상태
	// 만약 순서가 뒤바뀐다면 로직 작동안함
	// 
	// 마우스 호버 아이템 세팅
	if (m_pInventory->IsExistItem(m_iMouseHoverSlot))
	{
		m_pMouse->SetHoverReferItem({ m_pInventory->GetItem(m_iMouseHoverSlot)->GetItemInfo()->iID, ITEM_SOURCE::INVENTORY });
		CUIMgr::GetInstance()->ShowUI(UIID::ITEM_TOOLTIP);
		CUIMgr::GetInstance()->SetPos(UIID::ITEM_TOOLTIP, VEC{ 210.f, 200.f });
	}
	else
	{
		// 아이템이 없는 칸 혹은 아예 밖
		CUIMgr::GetInstance()->HideUI(UIID::ITEM_TOOLTIP);
	}


	// 드래그 시작
	if (KEY_DOWN(VK_LBUTTON) && m_pInventory->IsExistItem(m_iMouseHoverSlot))
	{
		// 마우스가 현재 드래그 중인 아이템의 아이디를 참조하게 해줌 - 렌더용
		m_pMouse->SetDragReferItem({ m_pInventory->GetItem(m_iMouseHoverSlot)->GetItemInfo()->iID, ITEM_SOURCE::INVENTORY });
		m_iStartSlot = m_iMouseHoverSlot;

		// 원래 슬롯은 안보이게 함
		m_vecItemSlot[m_iStartSlot]->SetItem(nullptr);
	}

	if (KEY_DOWN(VK_RBUTTON) && m_pInventory->IsExistItem(m_iMouseHoverSlot))
	{
		if (SCENEID::SHOP == CSceneMgr::GetInstance()->GetCurrentSceneID())
		{
			// 판매 시도
			static_cast<CShop*>(CSceneMgr::GetInstance()->GetCurrentScene())->TrySellItem(m_iMouseHoverSlot);
		}
	}

	if (-1 != m_pMouse->GetDragReferItem().iID)
	{
		// 인벤토리에서 인벤토리로 드래그
		if (ITEM_SOURCE::INVENTORY == m_pMouse->GetDragReferItem().eItemSource)
		{
			if (KEY_UP(VK_LBUTTON))
			{
				// 이동해야되는 인덱스 정리
				int iStartIdx = m_iStartSlot;
				int iEndIdx = m_iMouseHoverSlot;

				// 드래그 플래그가 필요없는 이유 -> 같은 슬롯이면 안되고 / 슬롯이 아니면 안되니깐
				// 마우스가 놓은 곳이 현재와 다른 슬롯이라면!
				if (iEndIdx != -1 && iEndIdx != iStartIdx)
				{
					// 아이템 이동
					m_pInventory->MoveItem(iStartIdx, iEndIdx);
				}

				// SyncInventorySlot이 LateUpdate에서 Slot 동기화 해줘서 별도로 할 것 없음

				// 초기화
				m_pMouse->SetDragReferItem({ -1, ITEM_SOURCE::END });
				m_iStartSlot = -1;
			}
		}
		// 상점에서 인벤토리로 드래그
		else if (ITEM_SOURCE::SHOP == m_pMouse->GetDragReferItem().eItemSource)
		{
			if (KEY_UP(VK_LBUTTON))
			{
				int iIdx = m_iMouseHoverSlot;

				if (iIdx != -1 && SCENEID::SHOP == CSceneMgr::GetInstance()->GetCurrentSceneID())
				{
					// 구매 시도 - 아이템 삽입
					static_cast<CShop*>(CSceneMgr::GetInstance()->GetCurrentScene())->TryBuyItem(iIdx, m_pMouse->GetDragReferItem().iID);
				}

				// SyncInventorySlot이 LateUpdate에서 Slot 동기화 해줘서 별도로 할 것 없음

				// 초기화
				m_pMouse->SetDragReferItem({ -1, ITEM_SOURCE::END });
				m_iStartSlot = -1;
			}
		}
		// 레벨업 보상에서 인벤토리로 드래그

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

