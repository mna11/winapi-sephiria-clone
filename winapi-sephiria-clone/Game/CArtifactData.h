#pragma once

class CArtifactData
{
private:
	CArtifactData();
	~CArtifactData();
	CArtifactData(const CArtifactData& rhs) = delete;
	CArtifactData& operator=(CArtifactData& rTileMgr) = delete;

public:
	static CArtifactData* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CArtifactData;
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
	void EmplaceArtifactInfo(int iID, wstring strImg, wstring strCategoryImg, wstring strName,
		wstring strCategoryName, vector<wstring> vecStrDescription, vector<tagStat> vecStat, 
		ARTIFACT_CATEGORY eCategory, int iLeaf);
	vector<STAT> CreateStatVec(int iID);
	vector<wstring> CreateStrDescriptionVec(int iID);

	void InitializeImg();
	void InitializeArtifactInfo(); 

public:
	const ARTIFACT_INFO* FindArtifactInfo(int iID) const;
	ITEM_INFO			 FindItemInfo(int iID);

private:
	static CArtifactData* m_pInstance;
	
	map<int, ARTIFACT_INFO> m_mapArtifactInfo;
};

