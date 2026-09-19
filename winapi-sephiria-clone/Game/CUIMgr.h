#pragma once

class CUI;

class CUIMgr
{
private:
	CUIMgr();
	~CUIMgr();
	CUIMgr(const CUIMgr& rhs) = delete;
	CUIMgr& operator=(CUIMgr& rTileMgr) = delete;

public:
	static CUIMgr* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CUIMgr;
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
	void ShowUI(UIID eID);
	void HideUI(UIID eID);
	void ToggleUI(UIID eID);

private:
	CUI* CreateUI(UIID eID);

private:
	static CUIMgr* m_pInstance;
	map<UIID, CUI*> m_mapUI;
};

