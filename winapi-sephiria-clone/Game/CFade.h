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

public:
    void SetFadeType(FADE_TYPE eType) { m_eFadeType = eType; }

private:
    FADE_TYPE m_eFadeType; 
    
    // RECTANGLE¿ë
    double  m_dFadeElapsed;
    double  m_dFadeDuration;
    int     m_iAlpha;
};

