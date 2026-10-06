#pragma once


class CWeaponData
{
private:
	CWeaponData();
	~CWeaponData();
	CWeaponData(const CWeaponData& rhs) = delete;
	CWeaponData& operator=(CWeaponData& rTileMgr) = delete;

public:
	static CWeaponData* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CWeaponData;
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
	void EmplaceWeaponInfo(int iID, wstring strIconImg, wstring strName, wstring strDescription, WEAPON_TYPE eWeaponType);

	void InitializeImg();
	void InitializeItemInfo();

public:
	int	GetMapSize() { return m_mapWeaponInfo.size(); }

public:
	const WEAPON_INFO* FindWeaponInfo(int iID) const;

private:
	static CWeaponData* m_pInstance;

	map<int, WEAPON_INFO> m_mapWeaponInfo;
};

