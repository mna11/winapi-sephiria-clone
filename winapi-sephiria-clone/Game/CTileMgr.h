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
		{
			m_pInstance = new CTileMgr;
			m_pInstance->m_bInteractionBlocked = false;
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
	void SetInteractionBlocked(bool bBlocked) { m_bInteractionBlocked = bBlocked; }
	bool GetInteractionBlocked() const		  { return m_bInteractionBlocked; }

public:
	void	Initialize();
	void	Update();
	void	LateUpdate();
	void	Render(Graphics*);
	void	Release();

public:
	const vector<CObj*>& GetTile(TILE_LAYER eLayer) const { return m_vecTile[toUType(eLayer)]; }

public:
	void	LoadTile(SCENEID eSceneID);

private:
	static CTileMgr* m_pInstance;

	vector<CObj*>		m_vecTile[toUType(TILE_LAYER::END)];
	bool				m_bInteractionBlocked;					// Interaction 블록 이동 불가 플래그
};

