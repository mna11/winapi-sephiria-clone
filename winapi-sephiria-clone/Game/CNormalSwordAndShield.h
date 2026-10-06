#pragma once

#include "CSwordAndShield.h"

class CNormalSwordAndShield :
    public CSwordAndShield
{
public:
    CNormalSwordAndShield();
    ~CNormalSwordAndShield();
public:
    void Initialize() override;
    void Render(Graphics*) override;
public:
    void CreateEffect() override;

    // CSwordAndShield을(를) 통해 상속됨
    void CleaveUpdate() override;
};