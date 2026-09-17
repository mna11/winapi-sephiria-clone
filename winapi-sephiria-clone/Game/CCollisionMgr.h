#pragma once

class CObj;

class CCollisionMgr
{
public:
	static void CollisionRect(list<CObj*>& DstList, list<CObj*>& SrcList);
	static void CollisionWall(list<CObj*>& DstList, TILE_LAYER eLayer);
};

