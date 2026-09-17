#include "pch.h"
#include "CCollisionMgr.h"

#include "CObj.h"

#include "CTileMgr.h"
#include "CTile.h"

void CCollisionMgr::CollisionRect(list<CObj*>& DstList, list<CObj*>& SrcList)
{
	RECT rc{};

	for (auto& Dst : DstList)
	{
		for (auto& Src : SrcList)
		{
			if (IntersectRect(&rc, &Dst->GetRect(), &Src->GetRect()))
			{
				Dst->SetDead(true);
				Src->SetDead(true);
			}
		}
	}
}

void CCollisionMgr::CollisionWall(list<CObj*>& DstList, TILE_LAYER eLayer)
{
	auto& vTiles = CTileMgr::GetInstance()->GetTile(eLayer);
	
    for (CObj* pObj : DstList)
    {
        for (CObj* pTileObj : vTiles)
        {
            CTile* pTile = static_cast<CTile*>(pTileObj);

            if (pTile->GetTile().eTileOption != TILE_OPTION::WALL)
                continue;

            // 대쉬 처리  - 개선 필요
            /*const VEC& tObjPrePoint = pObj->GetPrePoint();
            VEC vDir = (pObj->GetInfo().vPoint - tObjPrePoint).Normalize();

            VEC vToTile = pTile->GetInfo().vPoint - tObjPrePoint;
            VEC vToCurPoint = pObj->GetInfo().vPoint - tObjPrePoint;
            if (vToTile.Norm() < vToCurPoint.Norm())
            {
                pObj->AddPos(vDir * (vToCurPoint.Norm() - vToTile.Norm() + 10.f) * -1);
            }*/


            const INFO& tObjInfo = pObj->GetInfo();
            const INFO& tTileInfo = pTile->GetInfo();

            // Obj와 타일의 거리 측정
            VEC vDistance = tObjInfo.vPoint - tTileInfo.vPoint;
            // 충돌 판정 거리
            VEC vColDistance = (tObjInfo.vSize + tTileInfo.vSize) * 0.5f;
            // 겹친 거리? 
            VEC vOverlap = { vColDistance.fX - fabsf(vDistance.fX) , vColDistance.fY - fabsf(vDistance.fY) };


            // X와 Y축이 모두 겹쳐야 실제 충돌
            if (vOverlap.fX <= 0.f || vOverlap.fY <= 0.f)
                continue;

            // 더 적게 겹친 축만 밀어낼꺼임 - 둘 다 밀어내니깐 안되는건 아닌데 뭔가 뭔가였음
            if (vOverlap.fX < vOverlap.fY)
            {
                // fDistanceX는 플레이어 - 타일이었으니깐
                // 0보다 작으면 플레이어는 왼쪽에 있으므로 뺴줘야함
                float fPushX = (vDistance.fX < 0.f) ? -vOverlap.fX : vOverlap.fX;
                pObj->AddPos(fPushX, 0.f);
            }
            else
            {
                float fPushY = (vDistance.fY < 0.f) ? -vOverlap.fY : vOverlap.fY;
                pObj->AddPos(0.f, fPushY);
            }
        }

        pObj->RefreshRect();
    }
}
