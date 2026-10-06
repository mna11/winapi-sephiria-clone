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
    void ApplyChange() override;
};