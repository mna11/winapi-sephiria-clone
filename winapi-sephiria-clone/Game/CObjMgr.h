#pragma once

#include "CObj.h"

class CObjMgr
{
private:
	CObjMgr();
	CObjMgr(const CObj& rhs) = delete;
	CObjMgr& operator=(CObjMgr& rObjMgr) = delete;
	~CObjMgr();

public:
	static CObjMgr* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;

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
	void AddObject(OBJID eID, CObj* pObj);
	int  Update();
	void LateUpdate();
	void Render(Graphics* pGraphics);
	void Release();

public:
	CObj* GetPlayer() { 
		if (m_ObjList[toUType(OBJID::PLAYER)].empty())
			return nullptr;

		return m_ObjList[toUType(OBJID::PLAYER)].front(); 
	}

public:
	void	DeleteID(OBJID eID);
	bool	ObjEmpty(OBJID eID) { return m_ObjList[toUType(eID)].empty(); }

private:
	static CObjMgr* m_pInstance;

	list<CObj*>		m_ObjList[toUType(OBJID::END)];
	list<CObj*>		m_RenderList[toUType(RENDERID::END)];
};