#pragma once

#include "Define.h"

class CCameraMgr
{
private:
	CCameraMgr();
	~CCameraMgr();
	CCameraMgr(const CCameraMgr& rhs) = delete;
	CCameraMgr& operator=(CCameraMgr& rScrollMgr) = delete;

public:
	static CCameraMgr* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new CCameraMgr;

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
	float	GetScrollX() { return m_fScrollX; }
	float	GetScrollY() { return m_fScrollY; }
public:
	void	AddScrollX(float fX) { m_fScrollX += fX; }
	void	AddScrollY(float fY) { m_fScrollY += fY; }

public:
	void		Scroll_Lock();

private:
	static CCameraMgr* m_pInstance;

	float			m_fScrollX;
	float			m_fScrollY;
};