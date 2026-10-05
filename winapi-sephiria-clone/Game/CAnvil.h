#pragma once
#include "CObj.h"
class CAnvil :
    public CObj
{
public:
    CAnvil();
    ~CAnvil();
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
    
    bool m_bUse;
};

