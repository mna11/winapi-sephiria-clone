#pragma once
#include "CSwordAndShield.h"
class CMagicStaffSwordAndShield :
    public CSwordAndShield
{
public:
    CMagicStaffSwordAndShield();
    ~CMagicStaffSwordAndShield();
public:
    void Initialize() override;
    void Render(Graphics*) override;
    void CreateEffect() override;
    void ApplyChange() override;
};

