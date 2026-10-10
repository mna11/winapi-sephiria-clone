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
	m_vecLevels.resize(m_iInvenSize, 0);
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

		if (i < 5)
		{
			InsertItem(i, i, ITEM_TYPE::STONE_TABLET);
		}
	}
}

void CInventory::Update()
{
	UpdateLevel();
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

void CInventory::UpdateLevel()
{
	// 레벨을 일단 다 0으로 초기화
	m_vecLevels.assign(INVEN_SIZE, 0);

	// 레벨 업데이트
	for (int iIdx = 0; iIdx < m_vecItems.size(); ++iIdx)
	{
		CItem* pItem = m_vecItems[iIdx];

		// 빈칸이거나, 석판이 아니라면 패스
		if (nullptr == pItem || ITEM_TYPE::STONE_TABLET != pItem->GetItemType())
			continue;

		// 석판
		const STONE_TABLET_INFO* pStoneTabletInfo = static_cast<CStoneTablet*>(pItem)->GetStoneTabletInfo();
		int iAngle = static_cast<int>(pItem->GetAngle() / (PI * 0.5f));

		for (int i = 0; i < pStoneTabletInfo->arrRelativePos[iAngle].size(); ++i)
		{
			pair<int, int> prItemCord = make_pair<int, int>(iIdx % INVEN_COL, iIdx / INVEN_COL);
			pair<int, int> prRelativeCord = pStoneTabletInfo->arrRelativePos[iAngle][i];
			pair<int, int> prResultCord = { prItemCord.first + prRelativeCord.first, prItemCord.second + prRelativeCord.second };

			// 인벤토리에서 벗어난 위치는 패스
			if (prResultCord.first < 0 || prResultCord.first >= INVEN_COL)
				continue;
			if (prResultCord.second < 0 || prResultCord.second >= INVEN_SIZE / INVEN_COL)
				continue;

			// 업데이트할 인벤토리 인덱스
			int iUpdateIdx = prResultCord.first + prResultCord.second * INVEN_COL;

			// 해당 위치에 레벨 증감량을 더해주기
			m_vecLevels[iUpdateIdx] += pStoneTabletInfo->vecApplyLevel[i];
		}
	}

	// 레벨에 맞게 아이템 스탯 변경
	for (int iIdx = 0; iIdx < m_vecItems.size(); ++iIdx)
	{
		CItem* pItem = m_vecItems[iIdx];

		// 아이템이 없거니, 아티팩트가 아니라면 패스
		if (nullptr == pItem || ITEM_TYPE::ARTIFACT != pItem->GetItemType())
			continue; 

		static_cast<CArtifact*>(pItem)->ChangeLevel(m_vecLevels[iIdx]);
	}
}
