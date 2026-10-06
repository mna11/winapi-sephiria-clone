#include "pch.h"
#include "CObjMgr.h"

#include "CCameraMgr.h"
#include "CCollisionMgr.h"
#include "CUIMgr.h"

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

	m_ObjList[toUType(eID)].push_back(pObj);
}

int CObjMgr::Update()
{
	for (size_t i = 0; i < toUType(OBJID::END); ++i)
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

int CObjMgr::UpdateOnly(initializer_list<OBJID> ids)
{
	for (auto id : ids)
	{
		for (auto iter = m_ObjList[toUType(id)].begin();
			iter != m_ObjList[toUType(id)].end(); )
		{
			int iResult = (*iter)->Update();

			if (DEAD == iResult)
			{
				SafeDelete<CObj*>(*iter);
				iter = m_ObjList[toUType(id)].erase(iter);
			}
			else
				++iter;
		}
	}

	return 0;
}

void CObjMgr::LateUpdate()
{
	// 벽 충돌
	CCollisionMgr::CollisionWall(m_ObjList[toUType(OBJID::PLAYER)]);
	CCollisionMgr::CollisionWall(m_ObjList[toUType(OBJID::MONSTER)]);
	// 총알 벽 충돌
	CCollisionMgr::CollisionBulletWall(m_ObjList[toUType(OBJID::PLAYER_BULLET)]);
	CCollisionMgr::CollisionBulletWall(m_ObjList[toUType(OBJID::MONSTER_BULLET)]);

	for (size_t i = 0; i < toUType(OBJID::END); ++i)
	{
		for (auto& pObj : m_ObjList[i])
		{
			pObj->LateUpdate();

			if (m_ObjList[i].empty())
				break;

			RENDERID  eID = pObj->GetRenderID();
			m_RenderList[toUType(eID)].push_back(pObj);
		}
	}

	// 플레이어 공격, 방어 충돌
	CCollisionMgr::CollisionPlayerAttack(m_ObjList[toUType(OBJID::PLAYER)], m_ObjList[toUType(OBJID::MONSTER)]);
	CCollisionMgr::CollisionPlayerDefense(m_ObjList[toUType(OBJID::PLAYER)], m_ObjList[toUType(OBJID::MONSTER_BULLET)]);
	// 플레이어 총알 충돌
	CCollisionMgr::CollisionBulletObj(m_ObjList[toUType(OBJID::PLAYER_BULLET)], m_ObjList[toUType(OBJID::MONSTER)]);

	// 몬스터 공격 충돌
	CCollisionMgr::CollisionMonsterAttack(m_ObjList[toUType(OBJID::PLAYER)], m_ObjList[toUType(OBJID::MONSTER)]);
	// 몬스터 총알 충돌
	CCollisionMgr::CollisionBulletObj(m_ObjList[toUType(OBJID::MONSTER_BULLET)], m_ObjList[toUType(OBJID::PLAYER)]);
}

void CObjMgr::LateUpdateOnly(initializer_list<OBJID> ids)
{
	for (auto id : ids)
	{
		for (auto& pObj : m_ObjList[toUType(id)])
		{
			pObj->LateUpdate();

			if (m_ObjList[toUType(id)].empty())
				break;

			RENDERID  eID = pObj->GetRenderID();
			m_RenderList[toUType(eID)].push_back(pObj);
		}
	}
}

void CObjMgr::Render(Graphics* pGraphics)
{

	for (size_t i = 0; i < toUType(RENDERID::END); ++i)
	{
		m_RenderList[i].sort([](CObj* pDst, CObj* pSrc)->bool
			{
				return pDst->GetRenderLayer() < pSrc->GetRenderLayer();
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
	for (size_t i = 0; i < toUType(OBJID::END); ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), SafeDelete<CObj*>);
		m_ObjList[i].clear();
	}
}

void CObjMgr::DeleteID(OBJID eID)
{
	for_each(m_ObjList[toUType(eID)].begin(), m_ObjList[toUType(eID)].end(), SafeDelete<CObj*>);
	m_ObjList[toUType(eID)].clear();
}
