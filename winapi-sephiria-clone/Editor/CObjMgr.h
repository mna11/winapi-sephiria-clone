#pragma once

#include "CObj.h"
#include "CPlayer.h"
#include "CMouse.h"


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
	CPlayer* GetPlayer() const { 
		if (m_ObjList[EnumToInt(OBJID::PLAYER)].empty())
			return nullptr;

		return static_cast<CPlayer*>(m_ObjList[EnumToInt(OBJID::PLAYER)].front()); 
	}

	CMouse* GetMouse() const {
		if (m_ObjList[EnumToInt(OBJID::MOUSE)].empty())
			return nullptr;

		return static_cast<CMouse*>(m_ObjList[EnumToInt(OBJID::MOUSE)].front());
	}

public:
	void	DeleteID(OBJID eID);


private:
	static CObjMgr* m_pInstance;

	list<CObj*>		m_ObjList[EnumToInt(OBJID::END)];
	list<CObj*>		m_RenderList[EnumToInt(RENDERID::END)];
};