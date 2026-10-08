#pragma once
#include "CScene.h"

class CObj;

class CStage :
    public CScene
{
public:
    CStage();
    ~CStage();

public:
	virtual void InitializeRooms()				PURE;
	virtual void SpawnMonster(int iRoomIdx)		PURE;
	virtual void HandleCollision()				PURE;
	void StartBattleRoom(int iRoomIdx);
	void ClearCurRoom();

public:
	void HotKey(); // 스테이지에서 사용하는 단축키

public:
	const vector<ROOM_INFO>& GetVecRooms() const { return m_vecRooms; }
	const int& GetBattleRoomIdx() const { return m_iCurBattleRoomIdx; }

protected:
	void HandleCollisionBattleRoom();

protected:
	void CreateBattleWallEffects();
	void RemoveBattleWallEffects();

protected:
	void EndBattle();

protected:
	vector<ROOM_INFO> m_vecRooms;
	vector<CObj*>	  m_vecBattleWallEffects;
	int				  m_iCurBattleRoomIdx;
	RECT			  m_rcStair;
};

