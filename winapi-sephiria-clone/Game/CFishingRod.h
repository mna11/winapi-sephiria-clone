#pragma once
#include "CWeapon.h"

// 낚시시 렌더링 용 공격 기능이 존재하지 않는다.

class CFishingRod :
    public CWeapon
{
public:
    CFishingRod();
    ~CFishingRod();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
public:
    void Attack() override {};
    void SpecialAttack() override {};
private:
    void UpdatePos();
};

