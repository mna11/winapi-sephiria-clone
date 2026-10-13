#pragma once
#include "CObj.h"

class CKeyGuide;

class CBaba :
    public CObj
{
public:
    CBaba();
    ~CBaba();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void HandleInteraction();

private:
    void UpdateInteractRect();

private:
    RECT m_tInteractRect;
    bool m_bCol;

    CKeyGuide* m_pKeyGuide;
};

