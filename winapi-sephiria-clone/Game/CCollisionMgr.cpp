#include "pch.h"
#include "CCollisionMgr.h"

#include "CObj.h"
#include "CPlayer.h"
#include "CStage.h"
#include "CWeaponController.h"
#include "CWeapon.h"
#include "CBullet.h"

#include "CEffectMgr.h"
#include "CInventorySlotUI.h"
#include "CTileMgr.h"
#include "CObjMgr.h"
#include "CSceneMgr.h"
#include "CTile.h"
#include "CKeyMgr.h"
#include "CSoundMgr.h"

bool CCollisionMgr::CollisionRect(const RECT& DstRect, const RECT& SrcRect)
{
    RECT rc{};
    return IntersectRect(&rc, &DstRect, &SrcRect);
}

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

void CCollisionMgr::CollisionWall(list<CObj*>& DstList)
{
	auto& arrVecTiles = CTileMgr::GetInstance()->GetTiles();
	
    for (CObj* pObj : DstList)
    {
        if (nullptr == pObj || !pObj->GetWallCollisionEnable())
            continue;

        for (auto& vecTiles : arrVecTiles)
        {
            for (CObj* pTileObj : vecTiles)
            {
                CTile* pTile = static_cast<CTile*>(pTileObj);

                bool bIsWall = (pTile->GetTile().eTileOption == TILE_OPTION::WALL || pTile->GetTile().eTileOption == TILE_OPTION::AIR);
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

                pObj->OnWallCollision();

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
        }
        pObj->RefreshRect();
    }
}

void CCollisionMgr::CollisionPlayerAttack(list<CObj*>& DstList, list<CObj*>& SrcList)
{
    RECT rc{};

    for (auto& Dst : DstList)
    {
        CPlayer* pPlayer = static_cast<CPlayer*>(Dst);
        const vector<RECT>& vecWeaponRect = pPlayer->GetWeaponController()->GetWeapon()->GetAtkRectVec();

        for (auto& Src : SrcList)
        {
            for (auto& ColRect : vecWeaponRect)
            {
                if (IntersectRect(&rc, &ColRect, &Src->GetRect()))
                {
                    int iDamage = Dst->GetStat().iPhysicalAtk;
                    bool bCritical = (uniform_real_distribution<float>(0.f, 1.f)(g_engine) < pPlayer->GetStat().fCriticalChange);
                    if (bCritical)
                        iDamage =  static_cast<int>(iDamage * (1.f + pPlayer->GetStat().fCriticalDamage));

                    Src->HitDamage(iDamage, Dst, HIT_SOURCE::SLASH, bCritical);
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
        CPlayer* pPlayer = static_cast<CPlayer*>(Dst);
        const vector<RECT>& vecWeaponRect = pPlayer->GetWeaponController()->GetWeapon()->GetDefRectVec();

        for (auto& Src : SrcList)
        {
            for (auto& ColRect : vecWeaponRect)
            {
                if (IntersectRect(&rc, &ColRect, &Src->GetRect()))
                {
                    CEffectMgr::GetInstance()->CreateEffect(L"", pPlayer->GetInfo().vPoint, EFTMGR_STRING | EFTMGR_MOVE, 100.f, 0.5f, nullptr, VEC{1.f, -1.f}.Normalize(), L"BLOCK", Color{255, 182, 190, 200});
                    CSoundMgr::GetInstance()->PlaySound(L"ShieldBlock.wav", CHANNEL_GROUPID::SFX, 1.f);
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
                    Dst->HitDamage(Src->GetStat().iPhysicalAtk, Src, HIT_SOURCE::SLASH);
                }
            }
        }
    }
}

bool CCollisionMgr::CollisionMouse(VEC vMousePoint, RECT& rc)
{
    POINT ptMouse{ static_cast<int>(vMousePoint.fX), static_cast<int>(vMousePoint.fY) };
    return PtInRect(&rc, ptMouse);
}

void CCollisionMgr::CollisionBulletWall(list<CObj*>& bullets)
{
    auto& arrVecTiles = CTileMgr::GetInstance()->GetTiles();
    bool bInteractionBlocked = CTileMgr::GetInstance()->GetInteractionBlocked();

    for (CObj* pObj : bullets)
    {
        if (nullptr == pObj || pObj->GetDead())
            continue;

        CBullet* pBullet = static_cast<CBullet*>(pObj);
        pBullet->SetWallCollision(false);

        bool bHandled = false;

        for (auto& tiles : arrVecTiles)
        {
            for (CObj* pTileObj : tiles)
            {
                if (nullptr == pTileObj)
                    continue;

                CTile* pTile = static_cast<CTile*>(pTileObj);
                bool bWall = pTile->GetTile().eTileOption == TILE_OPTION::WALL;
                bool bBlocked = pTile->GetTile().eTileOption == TILE_OPTION::INTERACTION && bInteractionBlocked;

                if (!bWall && !bBlocked)
                    continue;

                if (!CollisionRect(pBullet->GetRect(), pTile->GetRect()))
                    continue;

                VEC vDistance = pBullet->GetInfo().vPoint - pTile->GetInfo().vPoint;
                VEC vColDistance = (pBullet->GetInfo().vSize + pTile->GetInfo().vSize) * 0.5f;
                VEC vOverlap{ vColDistance.fX - fabsf(vDistance.fX), vColDistance.fY - fabsf(vDistance.fY) };

                if (vOverlap.fX <= 0.f || vOverlap.fY <= 0.f)
                    continue;

                VEC vNormal{};

                // x축으로 충돌했음
                if (vOverlap.fX < vOverlap.fY)
                {
                    vNormal = { vDistance.fX < 0.f ? -1.f : 1.f, 0.f };
                    pBullet->AddPos(vNormal.fX * (vOverlap.fX + 1.f), 0.f);
                }
                else
                {
                    vNormal = { 0.f, vDistance.fY < 0.f ? -1.f : 1.f };
                    pBullet->AddPos(0.f, vNormal.fY * (vOverlap.fY + 1.f));
                }

                pBullet->RefreshRect();
                pBullet->SetWallNormal(vNormal);
                pBullet->SetWallCollision(true);

                bHandled = true;
                break;
            }

            if (bHandled)
                break;
        }
    }
}

void CCollisionMgr::CollisionBulletObj(list<CObj*>& DstList, list<CObj*>& SrcList)
{
    for (CObj* pDst : DstList)
    {
        if (pDst == nullptr || pDst->GetDead())
            continue;

        for (CObj* pSrc : SrcList)
        {
            if (pSrc == nullptr || pSrc->GetDead())
                continue;

            if (!CollisionRect(pDst->GetRect(), pSrc->GetRect()))
                continue;

            CBullet* pBullet = static_cast<CBullet*>(pDst);

            pBullet->OnHitObj(pSrc);
            break;
        }
    }
}