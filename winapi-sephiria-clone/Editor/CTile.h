#pragma once

#include "CObj.h"

class CTile :
    public CObj
{
public:
    CTile();
    virtual ~CTile();

public:
    void Initialize() override;
    int  Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;

public:
    const TILE& GetTile() const     { return m_tTile; }
    const bool& GetIsDraw() const { return m_bIsDraw; }
public:
    void SetTile(TILE tTile)        { m_tTile = tTile; }
    void SetIsDraw(bool bIsDraw)    { m_bIsDraw = bIsDraw; }

private:
    TILE            m_tTile;
    bool            m_bIsDraw;
};