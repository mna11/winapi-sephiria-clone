#pragma once
#include "CUI.h"
#include "CState.h"

enum class FISHING_ROD_STATE
{
    PREPARE_CAST,
    CASTING,
    IDLE_OUT_WATER,
    IDLE_IN_WATER,
    HOOK_SET,
    REELING,
    CAUGHT,
    END
};

class CFishingRodUI :
    public CUI,
    public CState<FISHING_ROD_STATE>
{
public:
    CFishingRodUI();
    ~CFishingRodUI();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;

public:
    void UpdateTime();
    void UpdateFrame() override;


public:
    void Show() override;
public:
    void ApplyChange() override;

public:
    bool IsAnimationFinished() const { return m_bAnimationFinished; }

private:
    bool m_bLoop;
    bool m_bAnimationFinished;
};