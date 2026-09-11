#pragma once

#include "CLine.h"

class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();
	CLineMgr(const CLineMgr& rhs) = delete;
	CLineMgr& operator=(CLineMgr& rLineMgr) = delete;
public:
	static CLineMgr* GetInstance()
	{
		if (!m_pInstance) {
			m_pInstance = new CLineMgr;
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
	void    Update() {}
	void    LateUpdate() {}
	void	Render(HDC hDC);
	void	Release();

	void	LoadLine();

private:
	static CLineMgr*	m_pInstance;
	list<CLine*>		m_LineList;
};