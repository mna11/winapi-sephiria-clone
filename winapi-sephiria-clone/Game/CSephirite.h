#pragma once
#include "CObj.h"
class CSephirite :
    public CObj
{
public:
    CSephirite();
    ~CSephirite();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void HandleInteraction();
    void UpdateInteractRect();

private:
    RECT m_tInteractRect;
    bool m_bCol;
};

