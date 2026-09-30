#pragma once


class CItemData
{
private:
	CItemData();
	~CItemData();
	CItemData(const CItemData& rhs) = delete;
	CItemData& operator=(CItemData& rTileMgr) = delete;

public:
	static CItemData* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CItemData;
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
	void EmplaceItemInfo(int iID, wstring strImg, wstring strCategoryImg, wstring strName,
		wstring strCategoryName, wstring strDescription1, wstring strDescription2, 
		wstring strDescription3, vector<tagStat> vecStat, ITEM_CATEGORY eCategory);
	vector<STAT> CreateStatVec(int iID);

public:
	const ITEM_INFO* FindItemInfo(int iID) const;

private:
	static CItemData* m_pInstance;
	
	map<int, ITEM_INFO> m_mapItemInfo;
};

