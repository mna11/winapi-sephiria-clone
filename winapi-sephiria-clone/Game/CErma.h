#pragma once

#include "CState.h"
#include "CMonster.h"

enum class ERMA_STATE {
    IDLE,
    WALK,
    AIR,
    STUN,
    STONE, // 스톤이 되어가는 상태
    LIGHT, // 빛나는 돌!
    END
};

class CErma :
    public CMonster, public CState<ERMA_STATE>
{
public:
    CErma();
    ~CErma();
public:
    void Initialize() override;
    int  Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void UpdateFrame() override;

public:
    void ApplyChange() override;
    void SetDamage(int iDamage, CObj* pObj) override;

public:
    void UpdateTime();
    void Move();
};

