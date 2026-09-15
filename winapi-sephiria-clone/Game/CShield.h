#pragma once
#include "CObj.h"
class CShield :
    public CObj
{
public:
    CShield();
    ~CShield();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

private:
    VEC m_vCellSize;
};

