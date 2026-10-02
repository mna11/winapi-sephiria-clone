#pragma once
#include "CUI.h"
class CLevelUp :
    public CUI
{
public:
    CLevelUp();
    ~CLevelUp();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
private:
    void KeyInput();
};

