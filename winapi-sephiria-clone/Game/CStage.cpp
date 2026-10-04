#include "pch.h"
#include "CStage.h"

#include "CPlayer.h"
#include "CState.h"
#include "CErma.h"
#include "CFluffy.h"
#include "CMonster.h"
#include "CTile.h"

#include "CItemSelectUI.h"

#include "CAbstractFactory.h"
#include "CEffectMgr.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"
#include "CUIMgr.h"
#include "CCollisionMgr.h"


CStage::CStage()
	: m_iCurBattleRoomIdx(-1)
{
    ZeroMemory(&m_rcStair, sizeof(RECT));
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

void CStage::HotKey()
{
    CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
    if (nullptr == pPlayer)
        return;

    // 아이템 셀렉트일 때는 맘대로 열고 닫기 못함
    if (KEY_DOWN('U') && !CUIMgr::GetInstance()->GetUI(UIID::ITEM_SELECT)->GetView())
    {
        CUIMgr::GetInstance()->ToggleUI(UIID::INVENTORY);
        CUIMgr::GetInstance()->HideUI(UIID::ITEM_TOOLTIP);
    }

    // 이번 방에서 Exp가 다 찼고, 방을 클리어하면 띄운다.
    if (pPlayer->GetCanLevelUp() && CObjMgr::GetInstance()->ObjEmpty(OBJID::MONSTER))
    {
        CUIMgr::GetInstance()->ShowUI(UIID::LEVEL_UP);
    }
}

void CStage::HandleCollisionBattleRoom()
{
    CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
    if (nullptr == pPlayer)
        return;

    // 현재 싸우고 있는 상태가 아니라면
    if (-1 == m_iCurBattleRoomIdx)
    {
        // 방을 순회한다.
        for (int i = 0; i < m_vecRooms.size(); ++i)
        {
            ROOM_INFO& room = m_vecRooms[i];

            // 이미 클리어 한 방이라면 다른 방 순회
            if (room.eState == ROOM_STATE::CLEAR)
                continue;

            // 만약 준비중인 방과 트리거가 충돌이 된다면
            if (CCollisionMgr::CollisionRect(pPlayer->GetRect(), room.rcTrigger))
            {
                // 현재 방을 전투 상태로 변환
                StartBattleRoom(i);
                CEffectMgr::GetInstance()->CreateEffect(L"Exclamation_Mark", { WINCX >> 1, 200.f }, EFTMGR_IMAGE | EFTMGR_FIXED | EFTMGR_NO_SCROLL);
                break;
            }
        }
    }
    else
    {
        // 전투 중인 상황인데, 현재 몬스터를 다 죽였다면 방을 클리어 상태로 변경한다.
        if (CObjMgr::GetInstance()->ObjEmpty(OBJID::MONSTER))
            ClearCurRoom();
    }
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

void CStage::EndBattle()
{
    // 전투 중이 아니라면
    if (-1 == m_iCurBattleRoomIdx)
    {
        // 드랍 아이템들이 자동으로 플레이어한테 다가가도록 함
        auto DropList = CObjMgr::GetInstance()->GetObjList(OBJID::DROP);
        for (auto& drop : DropList)
            static_cast<CDrop*>(drop)->SetBattleEnd(true);
    }
}
