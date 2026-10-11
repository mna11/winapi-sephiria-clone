#pragma once
#include "CUI.h"

class CLetterBox 
    : public CUI
{
public:
    CLetterBox();
    ~CLetterBox();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;

public:
    void Show() override;
    void Hide() override;

private:
    float m_fHeight;
    float m_fTargetHeight;
};