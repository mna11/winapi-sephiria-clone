#pragma once
#include "CStage.h"
class CStageTest :
    public CStage
{
public:
    CStageTest();
    ~CStageTest();
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;
    void Init_CreateObj() override;
    void InitializeRooms() override;
    void SpawnMonster(int iRoomIdx) override;
};

