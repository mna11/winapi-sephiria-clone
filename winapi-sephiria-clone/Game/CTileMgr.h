#pragma once

class CObj;

class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();
	CTileMgr(const CTileMgr& rhs) = delete;
	CTileMgr& operator=(CTileMgr& rTileMgr) = delete;

public:
	static CTileMgr* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new CTileMgr;

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
	void	Initialize();
	void	Update();
	void	LateUpdate();
	void	Render(Graphics*);
	void	Release();

public:
	void	LoadTile();

private:
	static CTileMgr* m_pInstance;

	vector<CObj*>		m_vecTile[toUType(TILE_LAYER::END)];
};

