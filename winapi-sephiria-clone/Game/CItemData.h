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
		wstring strCategoryName, vector<wstring> vecStrDescription, vector<tagStat> vecStat, 
		ITEM_CATEGORY eCategory, int iLeaf);
	vector<STAT> CreateStatVec(int iID);
	vector<wstring> CreateStrDescriptionVec(int iID);

	void InitializeImg();
	void InitializeItemInfo(); 

public:
	const ITEM_INFO* FindItemInfo(int iID) const;

private:
	static CItemData* m_pInstance;
	
	map<int, ITEM_INFO> m_mapItemInfo;
};

