#pragma once

class CPotionData
{
private:
	CPotionData();
	~CPotionData();
	CPotionData(const CPotionData& rhs) = delete;
	CPotionData& operator=(CPotionData& rTileMgr) = delete;

public:
	static CPotionData* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CPotionData;
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
	void EmplaceArtifactInfo(int iID, wstring strImg, wstring strName, vector<wstring> vecStrDescription, STAT tStat, int iMaxHave, int iLeaf);
	STAT CreateStat(int iID);
	vector<wstring> CreateStrDescriptionVec(int iID);

	void InitializeImg();
	void InitializePotionInfo();

public:
	const POTION_INFO*	 FindPotionInfo(int iID) const;
	ITEM_INFO			 FindItemInfo(int iID);
	const int& GetPotionNum() const { return m_mapPotionInfo.size(); }

private:
	static CPotionData* m_pInstance;

	map<int, POTION_INFO> m_mapPotionInfo;
};

