#pragma once

class CObj;
class CStage;
class CInventorySlotUI;

class CCollisionMgr
{
public:
	static void CollisionRect(list<CObj*>& DstList, list<CObj*>& SrcList);
	static void CollisionWall(list<CObj*>& DstList, TILE_LAYER eLayer);
	static void CollisionPlayerAttack(list<CObj*>& DstList, list<CObj*>& SrcList);
	static void CollisionPlayerDefense(list<CObj*>& DstList, list<CObj*>& SrcList);

	static void CollisionMonsterAttack(list<CObj*>& DstList, list<CObj*>& SrcList);
	static void CollisionRoom(CObj* pPlayer, CStage* pScene);
	static void CollisionStair(CObj* pPlayer, RECT& pStair, SCENEID eSceneID);
	static int GetCollisionSlotIndex(vector<CInventorySlotUI*>& vecSlots);
};

