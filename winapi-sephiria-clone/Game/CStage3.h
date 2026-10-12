#pragma once
#include "CStage.h"
class CStage3 :
    public CStage
{
public:
    CStage3();
    ~CStage3();
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;
public:
    void Init_CreateObj() override;
public:
    void InitializeRooms() override;
    void SpawnMonster(int iRoomIdx) override;

private:
    void HandleCollisionFishingSpot();
private:
    RECT m_rcFishing;
    int  m_iPlayerWeaponID;
};

