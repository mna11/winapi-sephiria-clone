#include "pch.h"
#include "CCollisionMgr.h"

#include "CObj.h"
#include "CPlayer.h"
#include "CStage.h"
#include "CWeaponController.h"
#include "CWeapon.h"

#include "CEffectMgr.h"
#include "CTileMgr.h"
#include "CObjMgr.h"
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

            bool bIsWall = (pTile->GetTile().eTileOption == TILE_OPTION::WALL);
            bool bIsBlockedInteraction = (pTile->GetTile().eTileOption == TILE_OPTION::INTERACTION && CTileMgr::GetInstance()->GetInteractionBlocked());

            if (!bIsWall && !bIsBlockedInteraction)
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

void CCollisionMgr::CollisionPlayerAttack(list<CObj*>& DstList, list<CObj*>& SrcList)
{
    RECT rc{};

    for (auto& Dst : DstList)
    {
        const vector<RECT>& vecWeaponRect = static_cast<CPlayer*>(Dst)->GetWeaponController()->GetWeapon()->GetAtkRectVec();
       
        for (auto& Src : SrcList)
        {
            for (auto& ColRect : vecWeaponRect)
            {
                if (IntersectRect(&rc, &ColRect, &Src->GetRect()))
                {
                    Src->SetDamage(Dst->GetStat().iAtk, Dst);
                }
            }
        }
    }
}

void CCollisionMgr::CollisionPlayerDefense(list<CObj*>& DstList, list<CObj*>& SrcList)
{
    RECT rc{};

    for (auto& Dst : DstList)
    {
        const vector<RECT>& vecWeaponRect = static_cast<CPlayer*>(Dst)->GetWeaponController()->GetWeapon()->GetDefRectVec();

        for (auto& Src : SrcList)
        {
            for (auto& ColRect : vecWeaponRect)
            {
                if (IntersectRect(&rc, &ColRect, &Src->GetRect()))
                {
                    Src->SetDead(true);
                }
            }
        }
    }
}

void CCollisionMgr::CollisionMonsterAttack(list<CObj*>& DstList, list<CObj*>& SrcList)
{
    RECT rc{};

    for (auto& Dst : DstList) // 플레이어
    {
        for (auto& Src : SrcList) // 몬스터
        {
            const vector<RECT>& vecAtkRect = Src->GetAtkRectVec();
            for (auto& ColRect : vecAtkRect)
            {
                if (IntersectRect(&rc, &ColRect, &Dst->GetRect()))
                {
                    Dst->SetDamage(Src->GetStat().iAtk, Src);
                }
            }
        }
    }
}

void CCollisionMgr::CollisionRoom(CObj* pPlayer, CStage* pStage)
{
    RECT rc{};

    int iCurBattleRoomIdx = pStage->GetBattleRoomIdx();

    // 현재 싸우고 있는 상태가 아니라면
    if (-1 == iCurBattleRoomIdx)
    {
        vector<ROOM_INFO> vecRooms = pStage->GetVecRooms();

        // 방을 순회한다.
        for (int i = 0; i < vecRooms.size(); ++i)
        {
            ROOM_INFO& room = vecRooms[i];

            // 이미 클리어 한 방이라면 다른 방 순회
            if (room.eState == ROOM_STATE::CLEAR)
                continue;

            // 만약 준비중인 방과 트리거가 충돌이 된다면
            if (IntersectRect(&rc, &pPlayer->GetRect(), &room.rcTrigger))
            {
                // 현재 방을 전투 상태로 변환
                pStage->StartBattleRoom(i);
                CEffectMgr::GetInstance()->CreateEffect(L"Exclamation_Mark", { WINCX >> 1, 200.f }, EFTMGR_IMAGE | EFTMGR_FIXED | EFTMGR_NO_SCROLL);
                break;
            }
        }
    }
    else
    {
        // 전투 중인 상황인데, 현재 몬스터를 다 죽였다면 방을 클리어 상태로 변경한다.
        if (CObjMgr::GetInstance()->ObjEmpty(OBJID::MONSTER))
            pStage->ClearCurRoom();
    }
}