#pragma once
#include "CUI.h"

class CButton;

class CStageChangePlayerIcon :
    public CUI
{
public:
    CStageChangePlayerIcon();
    ~CStageChangePlayerIcon();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
public:
    void SetAnchorButton(CButton* pButton) { m_pAnchorBtn = pButton; }

private:
    CButton* m_pAnchorBtn;
};

