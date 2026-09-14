#pragma once
#include "CScene.h"
class CEditor :
    public CScene
{
public:
    CEditor();
    ~CEditor();
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;

public:
    void KeyInput();
public:
    void HandleRectInput();
    void HandleTileInput();
    void HandleLineInput();
    
private:
    EDIT_STATE m_eState;

    // TILE INfo
    TILE          m_tTile;
    unordered_map<TILE_TYPE, int> m_mapTileMax;
};

