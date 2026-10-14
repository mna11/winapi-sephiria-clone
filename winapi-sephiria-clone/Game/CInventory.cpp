#include "pch.h"
#include "CInventory.h"
#include "CItem.h"
#include "CPlayer.h"

#include "CArtifact.h"
#include "CPotion.h"
#include "CStoneTablet.h"

#include "CAbstractFactory.h"
#include "CEffectMgr.h"
#include "CImgMgr.h"
#include "CUIMgr.h"
#include "CInventoryUI.h"
#include "CInventorySlotUI.h"
#include "CSoundMgr.h"

CInventory::CInventory()
	: m_iInvenSize(INVEN_SIZE), m_pOwner(nullptr), m_iUsePotionSlot(-1)
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

		if (i < 3)
		{
			InsertItem(i, i, ITEM_TYPE::POTION);
		}
	}

	// 테스트용 
	m_iUsePotionSlot = 2;
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

	// 포션은 중첩이 있어서 먼저 검사 후 넣음
	if (eItemType == ITEM_TYPE::POTION)
	{
		int iAlreadyItemIdx = FindItem(iID, eItemType);

		if (iAlreadyItemIdx != -1)
		{
			CPotion* pPotion = static_cast<CPotion*>(m_vecItems[iAlreadyItemIdx]);
			const POTION_INFO* pPotionInfo = pPotion->GetPotionInfo();
			if (nullptr == pPotionInfo || pPotion->GetHave() >= pPotionInfo->iMaxHave)
				return false;

			pPotion->AddHave(1);
			return true;
		}
	}


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
		case ITEM_TYPE::POTION:
			m_vecItems[iIdx] = CAbstractFactory<CPotion>::CreateItem(iID, m_pOwner, eItemType);
			static_cast<CPotion*>(m_vecItems[iIdx])->SetHave(1);
			if (-1 == m_iUsePotionSlot)
				m_iUsePotionSlot = iIdx;
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
				case ITEM_TYPE::POTION:
					m_vecItems[i] = CAbstractFactory<CPotion>::CreateItem(iID, m_pOwner, eItemType);
					static_cast<CPotion*>(m_vecItems[i])->SetHave(1);
					if (-1 == m_iUsePotionSlot)
						m_iUsePotionSlot = i;
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
	if (nullptr != m_vecItems[iIdx] && m_vecItems[iIdx]->GetItemType() == ITEM_TYPE::ARTIFACT)
		m_pOwner->AddStat(m_vecItems[iIdx]->GetStat() * -1);
	SafeDelete<CItem*>(m_vecItems[iIdx]);
	m_vecItems[iIdx] = nullptr;
}

void CInventory::MoveItem(int iStartIdx, int iEndIdx)
{
	// 만약 현재 세팅된 포션과 관련이 있을 시 바꿔주기
	if (m_iUsePotionSlot == iStartIdx) m_iUsePotionSlot = iEndIdx;
	else if (m_iUsePotionSlot == iEndIdx) m_iUsePotionSlot = iStartIdx;

	std::swap(m_vecItems[iStartIdx], m_vecItems[iEndIdx]);
}

int CInventory::FindItem(int iID, ITEM_TYPE eItemType)
{
	auto iter = find_if(m_vecItems.begin(), m_vecItems.end(), [iID, eItemType]( const CItem* pItem ) {
		return (nullptr != pItem && pItem->GetItemID() == iID && pItem->GetItemType() == eItemType);
	});

	if (iter == m_vecItems.end())
		return -1;
	else
		return static_cast<int>(iter - m_vecItems.begin());
}

bool CInventory::UsePotion()
{
	// 세팅이 안된 상태라면 false
	if (-1 == m_iUsePotionSlot)
		return false; 

	CItem* pItem = m_vecItems[m_iUsePotionSlot];
	if (nullptr == pItem || ITEM_TYPE::POTION != pItem->GetItemType())
		return false;
	
	CPotion* pPotion = static_cast<CPotion*>(pItem);
	const POTION_INFO* pPotionInfo = pPotion->GetPotionInfo();
	if (nullptr == pPotionInfo)
		return false;
	
	// 스탯을 더해주고 보유량을 하나 까준다.
	m_pOwner->AddStat(pPotionInfo->tStat);
	pPotion->AddHave(-1);

	// 보유량이 0 이하가 되면 인벤토리에서 삭제하고 Slot을 -1로 초기화해준다.
	if (pPotion->GetHave() <= 0)
	{
		EraseItem(m_iUsePotionSlot);
		ChangePotion(-1, true);	// 현재 인덱스 이후로 포션 찾음
	}
	VEC vOffset { 0.f, m_pOwner->GetInfo().vSize.fY * -0.5f };
	CEffectMgr::GetInstance()->CreateEffect(L"Potion_Use", m_pOwner->GetInfo().vPoint + vOffset, 0, 0.f, 0.1);
	return true;
}

void CInventory::ChangePotion(int iWheelScroll, bool bUse)
{
	if (iWheelScroll == 0)
		return;

	int iInvenSize = m_vecItems.size();
	int iDir = (iWheelScroll > 0) ? -1 : 1;
	int iSlot = m_iUsePotionSlot;

	for (int i = 0; i < iInvenSize; ++i)
	{
		iSlot = (iSlot + iDir + iInvenSize) % iInvenSize;

		CItem* pItem = m_vecItems[iSlot];
		if (pItem == nullptr || pItem->GetItemType() != ITEM_TYPE::POTION)
			continue;

		if (iSlot != m_iUsePotionSlot)
		{
			m_iUsePotionSlot = iSlot;
			
			// 사용으로 바뀔때는 소리 X
			if (!bUse)
				CSoundMgr::GetInstance()->PlaySound(L"ChangePotion.wav", CHANNEL_GROUPID::SFX, 1.f);
		}
		return;
	}

	// 현재 인벤토리에 포션류가 없음
	m_iUsePotionSlot = -1;
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
