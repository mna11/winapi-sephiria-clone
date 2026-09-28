#pragma once
#include "CStage.h"

class CBossStage :
    public CStage
{
public:
    CBossStage();
    ~CBossStage();
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

