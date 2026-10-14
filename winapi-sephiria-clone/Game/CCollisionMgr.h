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
			// ForgeUI에서는 일단 무기 데이터 개수만큼 전부 할당해뒀기 때문에 문제가 발생할 여지가 있음
			// 여기는 수정 필요 - ForgeUI 자체가 수정할 필요 있음
			// 무기 ID별로 슬롯 매칭을 해뒀는데, 무기 타입별 띄우는거에서 나오는 불일치 문제
			if (nullptr == vecSlots[i])
				return -1;

			if (PtInRect(&vecSlots[i]->GetRect(), ptMouse))
				return i;
		}
		return -1;
	}
};

