#pragma once

class CObj;
class CPlayer;
class CStage;
class CInventorySlotUI;

class CCollisionMgr
{
public:
	static bool CollisionRect(const RECT& DstRect, const RECT& SrcRect);
	static void CollisionRect(list<CObj*>& DstList, list<CObj*>& SrcList);
	static void CollisionWall(list<CObj*>& DstList, TILE_LAYER eLayer);
	static void CollisionPlayerAttack(list<CObj*>& DstList, list<CObj*>& SrcList);
	static void CollisionPlayerDefense(list<CObj*>& DstList, list<CObj*>& SrcList);

	static void CollisionMonsterAttack(list<CObj*>& DstList, list<CObj*>& SrcList);
	static int GetCollisionSlotIndex(VEC vMousePoint, vector<CInventorySlotUI*>& vecSlots);
};

