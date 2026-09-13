#pragma once
#include "CObj.h"
class CCameraTarget :
    public CObj
{
public:
    CCameraTarget();
    ~CCameraTarget();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
};

