#pragma once

class CObj;
class CStage;

class CCollisionMgr
{
public:
	static void CollisionRect(list<CObj*>& DstList, list<CObj*>& SrcList);
	static void CollisionWall(list<CObj*>& DstList, TILE_LAYER eLayer);
	static void CollisionPlayerAttack(list<CObj*>& DstList, list<CObj*>& SrcList);
	static void CollisionPlayerDefense(list<CObj*>& DstList, list<CObj*>& SrcList);

	static void CollisionMonsterAttack(list<CObj*>& DstList, list<CObj*>& SrcList);
	static void CollisionRoom(CObj* pPlayer, CStage* pScene);
};

