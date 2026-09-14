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
    const TILE& GetTile() const { return m_tTile; }
public:
    void SetTile(TILE tTile) { m_tTile = tTile; }

private:
    TILE            m_tTile;
    ColorMatrix     m_tColorMatrix[EnumToInt(TILE_OPTION::END)];
};