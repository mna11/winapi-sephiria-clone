#include "pch.h"
#include "CObjMgr.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::AddObject(OBJID eID, CObj* pObj)
{
	if (eID >= OBJID::END || nullptr == pObj)
		return;

	m_ObjList[EnumToInt(eID)].push_back(pObj);
}

int CObjMgr::Update()
{
	for (size_t i = 0; i < EnumToInt(OBJID::END); ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{
			int iResult = (*iter)->Update();

			if (DEAD == iResult)
			{
				SafeDelete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}



	return 0;
}

void CObjMgr::LateUpdate()
{
	for (size_t i = 0; i < EnumToInt(OBJID::END); ++i)
	{
		for (auto& pObj : m_ObjList[i])
		{
			pObj->LateUpdate();

			if (m_ObjList[i].empty())
				break;

			RENDERID  eID = pObj->GetRenderID();
			m_RenderList[EnumToInt(eID)].push_back(pObj);
		}
	}
	//CCollisionMgr::CollisionRectEx(m_ObjList[PLAYER], m_ObjList[MONSTER]);
	//CCollisionMgr::CollisionCircle(m_ObjList[BULLET], m_ObjList[MONSTER]);
}

void CObjMgr::Render(Graphics* pGraphics)
{

	for (size_t i = 0; i < EnumToInt(RENDERID::END); ++i)
	{
		m_RenderList[i].sort([](CObj* pDst, CObj* pSrc)->bool
			{
				return pDst->GetInfo().fY > pSrc->GetInfo().fY;
			});

		for (auto& pObj : m_RenderList[i])
		{
			pObj->Render(pGraphics);
		}

		m_RenderList[i].clear();
	}

}

void CObjMgr::Release()
{
	for (size_t i = 0; i < EnumToInt(OBJID::END); ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), SafeDelete<CObj*>);
		m_ObjList[i].clear();
	}
}

void CObjMgr::DeleteID(OBJID eID)
{
	for_each(m_ObjList[EnumToInt(eID)].begin(), m_ObjList[EnumToInt(eID)].end(), SafeDelete<CObj*>);
	m_ObjList[EnumToInt(eID)].clear();
}
