#pragma once
#include "CObj.h"
class CItem :
    public CObj
{
public:
    CItem();
    ~CItem();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
};

