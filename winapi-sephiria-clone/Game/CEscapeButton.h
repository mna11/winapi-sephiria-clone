#pragma once
#include "CUI.h"
class CEscapeButton :
    public CUI
{
public:
    CEscapeButton();
    ~CEscapeButton();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    const bool& GetMouseCollide() { return m_bCol; }

private:
    bool m_bCol;
};

