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
	static void CollisionWall(list<CObj*>& DstList);
	static void CollisionPlayerAttack(list<CObj*>& DstList, list<CObj*>& SrcList);
	static void CollisionPlayerDefense(list<CObj*>& DstList, list<CObj*>& SrcList);

	static void CollisionMonsterAttack(list<CObj*>& DstList, list<CObj*>& SrcList);

	static bool CollisionMouse(VEC vMousePoint, RECT& rc);

	static void CollisionBulletWall(list<CObj*>& bullets);
	static void CollisionBulletObj(list<CObj*>& DstList, list<CObj*>& SrcList);


	template<typename T>
	static int GetCollisionSlotIndex(VEC vMousePoint, vector<T*>& vecSlots)
	{
		POINT ptMouse{ static_cast<int>(vMousePoint.fX), static_cast<int>(vMousePoint.fY) };

		for (int i = 0; i < static_cast<int>(vecSlots.size()); ++i)
		{
			if (PtInRect(&vecSlots[i]->GetRect(), ptMouse))
				return i;
		}
		return -1;
	}
};

