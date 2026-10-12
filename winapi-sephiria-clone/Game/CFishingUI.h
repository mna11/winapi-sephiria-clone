#pragma once
#include "CUI.h"

class CFishingRodUI;

class CFishingUI 
    : public CUI
{
public:
    CFishingUI();
    ~CFishingUI();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;

public:
    void Show() override;
    void Hide() override;
    void Toggle() override;

private:
    CFishingRodUI* m_pRodUI;
};