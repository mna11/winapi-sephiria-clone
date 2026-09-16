#pragma once
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
	void	CreateEffect(const TCHAR*, VEC vPoint, float fFactor);

private:
	static CEffectMgr* m_pInstance;
};