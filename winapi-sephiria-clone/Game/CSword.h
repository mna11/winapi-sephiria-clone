#pragma once
#include "CObj.h"
class CSword :
    public CObj
{
public:
    CSword();
    ~CSword();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

private:
    VEC m_vCellSize;
};

