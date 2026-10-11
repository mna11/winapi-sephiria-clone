#pragma once
#include "CUI.h"

class CObj;

class CSpeechBubble :
    public CUI
{
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
public:
    void Show() override; 
    void Hide() override;

private:
    CObj* m_pSpeaker;
};

