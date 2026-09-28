#pragma once

class CObj;

class CEffectMgr
{
private:
	CEffectMgr();
	~CEffectMgr();
	CEffectMgr(const CEffectMgr& rhs) = delete;
	CEffectMgr& operator=(CEffectMgr& rTileMgr) = delete;

public:
	static CEffectMgr* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CEffectMgr;
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

public:
	CObj* CreateEffect(const TCHAR* pFrameKey, VEC vPoint, int iOption, 
		float fFactor = 0.f, double dFrameSpeed = 0., CObj* pObj = nullptr, VEC vDir = { 0.f, 0.f }, wstring wstr = L"", Color tColor = {0, 0, 0, 0});

private:
	static CEffectMgr* m_pInstance;
};


