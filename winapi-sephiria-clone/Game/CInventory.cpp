#include "pch.h"
#include "CInventory.h"
#include "CItem.h"
#include "CPlayer.h"

#include "CArtifact.h"
#include "CStoneTablet.h"

#include "CAbstractFactory.h"
#include "CImgMgr.h"
#include "CUIMgr.h"
#include "CInventoryUI.h"
#include "CInventorySlotUI.h"
#include "CSoundMgr.h"

CInventory::CInventory()
	: m_iInvenSize(INVEN_SIZE), m_pOwner(nullptr)
{
	m_vecItems.resize(m_iInvenSize, nullptr);
}

CInventory::~CInventory()
{
	Release();
}

void CInventory::Initialize()
{
	// 테스트용
	for (int i = 0; i < m_vecItems.size(); ++i)
	{
		if (i < 9)
		{
			InsertItem(i, i, ITEM_TYPE::ARTIFACT);
		}
	}
}

void CInventory::Release()
{
	for_each(m_vecItems.begin(), m_vecItems.end(), SafeDelete<CItem*>);
	m_vecItems.clear();
	m_vecItems.shrink_to_fit();
}


bool CInventory::InsertItem(int iIdx, int iID, ITEM_TYPE eItemType)
{
	if (nullptr == m_pOwner)
		return false;

	if (nullptr == m_vecItems[iIdx])
	{
		switch (eItemType)
		{
		case ITEM_TYPE::ARTIFACT:
			m_vecItems[iIdx] = CAbstractFactory<CArtifact>::CreateItem(iID, m_pOwner, eItemType);
			m_pOwner->AddStat(m_vecItems[iIdx]->GetStat());
			break;
		case ITEM_TYPE::STONE_TABLET:
			m_vecItems[iIdx] = CAbstractFactory<CStoneTablet>::CreateItem(iID, m_pOwner, eItemType);
			break;
		default:
			return false;
			break;
		}
		
		return true;
	}
	else
	{
		// 이미 있는 공간에 아이템을 넣은 경우, 앞에서부터 순회해 빈 공간을 찾아 넣음
		for (int i = 0; i < m_vecItems.size(); ++i)
		{
			if (nullptr == m_vecItems[i])
			{
				switch (eItemType)
				{
				case ITEM_TYPE::ARTIFACT:
					m_vecItems[i] = CAbstractFactory<CArtifact>::CreateItem(iID, m_pOwner, eItemType);
					m_pOwner->AddStat(m_vecItems[i]->GetStat());
					break;
				case ITEM_TYPE::STONE_TABLET:
					m_vecItems[i] = CAbstractFactory<CStoneTablet>::CreateItem(iID, m_pOwner, eItemType);
					break;
				default:
					return false;
					break;
				}

				return true;
			}
		}
	}

	// 인벤토리가 꽉 찬 경우
	return false; 
}

void CInventory::EraseItem(int iIdx)
{
	if (nullptr != m_vecItems[iIdx])
		m_pOwner->AddStat(m_vecItems[iIdx]->GetStat() * -1);
	SafeDelete<CItem*>(m_vecItems[iIdx]);
	m_vecItems[iIdx] = nullptr;
}

void CInventory::MoveItem(int iStartIdx, int iEndIdx)
{
	std::swap(m_vecItems[iStartIdx], m_vecItems[iEndIdx]);
}
