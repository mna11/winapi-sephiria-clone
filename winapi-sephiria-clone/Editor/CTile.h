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
    void Render(Graphics*) override;
    void Render(Graphics* pGraphics, Image* pTileImg, VEC& vScroll) override; // 순수 가상함수는 아님
    void Render(Graphics* pGraphics, Image* pTileImg, VEC& vScroll, ImageAttributes& ImgAttr) override; // override (순수 가상함수는 아님)
    void Release() override;

public:
    const TILE& GetTile() const { return m_tTile; }
public:
    void SetTile(TILE tTile) { m_tTile = tTile; __super::UpdateRect(); }
   
private:
    TILE            m_tTile;
};