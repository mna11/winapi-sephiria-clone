#include "pch.h"
#include "CStage.h"

#include "CPlayer.h"
#include "CErma.h"
#include "CFluffy.h"
#include "CMonster.h"
#include "CTile.h"

#include "CAbstractFactory.h"
#include "CEffectMgr.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"
#include "CUIMgr.h"


CStage::CStage()
	: m_iCurBattleRoomIdx(-1)
{
}

CStage::~CStage()
{
}

void CStage::StartBattleRoom(int iRoomIdx)
{
	ROOM_INFO& room = m_vecRooms[iRoomIdx];
	room.eState = ROOM_STATE::BATTLE;
	m_iCurBattleRoomIdx = iRoomIdx;
	CTileMgr::GetInstance()->SetInteractionBlocked(true);
    CreateBattleWallEffects();
    SpawnMonster(iRoomIdx);
}

void CStage::ClearCurRoom()
{
	ROOM_INFO& room = m_vecRooms[m_iCurBattleRoomIdx];
	room.eState = ROOM_STATE::CLEAR;
	m_iCurBattleRoomIdx = -1;
	CTileMgr::GetInstance()->SetInteractionBlocked(false);

    RemoveBattleWallEffects();
}

void CStage::CreateBattleWallEffects()
{
    for (int i = 0; i < toUType(TILE_LAYER::END); ++i)
    {
        TILE_LAYER eLayer =
            static_cast<TILE_LAYER>(i);

        const vector<CObj*>& vecTiles =
            CTileMgr::GetInstance()->GetTile(eLayer);

        for (CObj* pObj : vecTiles)
        {
            CTile* pTile =
                static_cast<CTile*>(pObj);

            if (pTile->GetTile().eTileOption !=
                TILE_OPTION::INTERACTION)
            {
                continue;
            }

            CObj* pEffect =
                CEffectMgr::GetInstance()->CreateEffect(
                    L"Battle_Wall",
                    pTile->GetInfo().vPoint,
                    EFTMGR_IMAGE | EFTMGR_FIXED
                );

            m_vecBattleWallEffects.push_back(pEffect);
        }
    }
}

void CStage::RemoveBattleWallEffects()
{
    for (CObj* pEffect : m_vecBattleWallEffects)
    {
        if (pEffect != nullptr)
            pEffect->SetDead(true);
    }

    m_vecBattleWallEffects.clear();
}