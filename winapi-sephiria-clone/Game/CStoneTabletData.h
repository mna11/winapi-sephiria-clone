#pragma once

class CStoneTabletData
{
private:
	CStoneTabletData();
	~CStoneTabletData();
	CStoneTabletData(const CStoneTabletData& rhs) = delete;
	CStoneTabletData& operator=(CStoneTabletData& rTileMgr) = delete;

public:
	static CStoneTabletData* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CStoneTabletData;
		}

		return m_pInstance;
	}

	static void	DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Initialize();
	void Release();

private:
	void EmplaceStoneTabletInfo(int iID, wstring strImg, wstring strName, array<int, INVEN_SIZE> arrApplyLevel, int iLeaf);
	array<int, INVEN_SIZE> CreateApplyLevel(int iID);

private:
	void InitializeImg();
	void InitializeStoneTabletInfo();

public:
	const STONE_TABLET_INFO* FindStoneTabletInfo(int iID) const;
	ITEM_INFO				 FindItemInfo(int iID);

private:
	static CStoneTabletData* m_pInstance;

	map<int, STONE_TABLET_INFO> m_mapStoneTabletInfo;
};

