#include "pch.h"
#include "CInventory.h"
#include "CItem.h"
#include "CPlayer.h"

#include "CAbstractFactory.h"
#include "CImgMgr.h"
#include "CUIMgr.h"
#include "CInventoryUI.h"
#include "CInventorySlotUI.h"

CInventory::CInventory()
	: m_iInvenSize(24), m_pOwner(nullptr)
{
	m_vecItems.resize(m_iInvenSize, nullptr);
}

CInventory::~CInventory()
{
	Release();
}

void CInventory::Initialize()
{
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Balisong.png", L"Balisong");

	// 테스트용
	for (int i = 0; i < m_vecItems.size(); ++i)
	{
		if (i < 15)
		{
			InsertItem(i, 0);
		}
	}
}

void CInventory::Release()
{
	for_each(m_vecItems.begin(), m_vecItems.end(), SafeDelete<CItem*>);
	m_vecItems.clear();
	m_vecItems.shrink_to_fit();
}


void CInventory::InsertItem(int iIdx, int iID)
{
	if (nullptr == m_pOwner)
		return;

	if (nullptr == m_vecItems[iIdx])
	{
		m_vecItems[iIdx] = CAbstractFactory<CItem>::CreateItem(iID);
		m_pOwner->AddStat(m_vecItems[iIdx]->GetStat());
	}
	else
	{
		// 이미 있는 공간에 아이템을 넣은 경우, 앞에서부터 순회해 빈 공간을 찾아 넣음
		for (int i = 0; i < m_vecItems.size(); ++i)
		{
			if (nullptr == m_vecItems[i])
			{
				m_vecItems[i] = CAbstractFactory<CItem>::CreateItem(iID);
				m_pOwner->AddStat(m_vecItems[i]->GetStat());
			}
		}
	}

}

void CInventory::EraseItem(int iIdx)
{
	SafeDelete<CItem*>(m_vecItems[iIdx]);
	m_vecItems[iIdx] = nullptr;
}

void CInventory::MoveItem(int iStartIdx, int iEndIdx)
{
	std::swap(m_vecItems[iStartIdx], m_vecItems[iEndIdx]);
}
