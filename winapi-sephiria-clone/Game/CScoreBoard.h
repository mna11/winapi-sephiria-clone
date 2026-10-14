#pragma once
#include "CScene.h"
class CScoreBoard :
    public CScene
{
public:
    CScoreBoard();
    ~CScoreBoard();
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;
    void Init_CreateObj() override;
private:
    CButton* m_pRetryButton;
};

