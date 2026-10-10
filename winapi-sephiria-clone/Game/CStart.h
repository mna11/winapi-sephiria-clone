#pragma once
#include "CScene.h"
#include "CState.h"

class CButton;
class CMouse;

enum class START_SCENE_STATE
{
    HORAY,
    ANIMATION,
    START,
    END
};


class CStart :
    public CScene, public CState<START_SCENE_STATE>
{
public:
    CStart();
    ~CStart();
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;

private:
    void Init_CreateObj() override;

private:
    void ApplyChange() override;

private:
    void UpdateFrame();
    void SetFrame(int iStart, int iEnd, int iMotion, double dFrameSpeed);

private:
    void HandleHorayRender(Graphics* pGraphics);
    void HandleAnimationRender(Graphics* pGraphics);
    void HandleStartRender(Graphics* pGraphics);

private:
    CMouse* m_pMouse;

    CButton* m_pStartBtn;
    CButton* m_pExitBtn;

    FRAME m_tTreeFrame;

    double m_dStateTime;
    double m_dHorayTime;
};

