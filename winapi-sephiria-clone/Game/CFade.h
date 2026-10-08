#pragma once
#include "CUI.h"
class CFade :
    public CUI
{
public:
    CFade();
    ~CFade();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
public:
    void Show() override;
    void UpdateFrame() override;
};

