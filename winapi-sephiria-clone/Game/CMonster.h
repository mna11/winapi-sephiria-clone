#pragma once
#include "CObj.h"
class CMonster :
    public CObj
{
public:
    CMonster();
    ~CMonster();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
};

