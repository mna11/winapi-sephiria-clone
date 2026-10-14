#pragma once
#include "CSwordAndShield.h"
class CReturningGuardian :
    public CSwordAndShield
{
public:
    CReturningGuardian();
    ~CReturningGuardian();
public:
    void Initialize() override;
    void Render(Graphics*) override;
    void ApplyChange() override;
    void CreateEffect() override;
};

