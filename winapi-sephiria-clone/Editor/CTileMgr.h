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
			m_pInstance->Initialize();
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
	void	Initialize();
	void	Update();
	void	LateUpdate();
	void	Render(Graphics*);
	void	Release();
public:
	bool	GetPreview() const		  { return m_bPreview; }
public:
	void	SetPreview() { m_bPreview = !m_bPreview; }

public:
	void	PickingTile(POINT pt, TILE tTile);
	void	SaveTile();
	void	LoadTile();

private:
	static CTileMgr*	m_pInstance;

	vector<CObj*>		m_vecTile[EnumToInt(TILE_LAYER::END)];
	bool				m_bPreview;
};

