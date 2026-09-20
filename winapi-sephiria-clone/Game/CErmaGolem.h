#pragma once
#include "CMonster.h"
#include "CState.h"

class CErmaGolem :
    public CMonster
{
public:
    CErmaGolem();
    ~CErmaGolem();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
};

