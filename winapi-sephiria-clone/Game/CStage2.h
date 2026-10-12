#pragma once
#include "CStage.h"
class CStage2 :
    public CStage
{
public:
    CStage2();
    ~CStage2();
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
};

