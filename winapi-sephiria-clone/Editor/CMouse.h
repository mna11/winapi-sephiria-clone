#pragma once
#include "CObj.h"

class CTile;

class CMouse :
    public CObj
{
public:
    CMouse();
    ~CMouse();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void SetEditState(EDIT_STATE eEditState)  { m_eEditState = eEditState; }
    void SetTile(TILE* pTile)                 { m_pTile = pTile; }

private:
    //void HandleEditLineRender();
    void HandleEditTileRender(Graphics*);
    //void HandleEditRectRender();

private:
    EDIT_STATE  m_eEditState;
    TILE*       m_pTile;
};

