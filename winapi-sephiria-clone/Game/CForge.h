#pragma once
#include "CScene.h"

class CButton;
class CMsgBox;

class CForge :
    public CScene
{
public:
    CForge();
    ~CForge();
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;
public:
    void Init_CreateObj() override;
public:
    void UpdateTime();

public:
    void TryChangeWeapon(int iID);
    void ChangeWeapon(int iID);

private:
    int     m_iFrame;
    double  m_dFrameTime;

    CButton*    m_pEscapeButton;
    CMsgBox*    m_pMsgBoxUI;
    RectF       m_rcMsgBox;
};

