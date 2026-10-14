#pragma once
#include "CUI.h"
#include "CState.h"


class CPlayer;
class CFishingRodUI;
class CFishingPlayingUI;

enum class FISHING_STATE
{
    READY,      // 아직 LMB도 클릭 안한 상태
    BAIT,       // LMB 클릭하여 미끼를 던진 상태
    PLAYING,    // 미끼를 물고기가 물어서 플레이하는 상태
    CAUGHT,     // 플레이 결과 - 물고기 잡음
    MISS,       // 플레이 결과 - 물고기 놓침
    END
};

class CFishingUI 
    : public CUI, public CState<FISHING_STATE>
{
public:
    CFishingUI();
    ~CFishingUI();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;

public:
    void Show() override;
    void Hide() override;
    void Toggle() override;

public:
    void UpdateTime();
    void ApplyChange() override;

private:
    CPlayer*            m_pPlayer;      // 게임 플레이어
    CFishingRodUI*      m_pRodUI;       // 낚시대 UI
    CFishingPlayingUI*  m_pPlayingUI;   // 게임 플레이 UI

    double m_dCatchElapseTime;  // 물고기 잡기 시도 후 걸린 시간
    double m_dCatchInterval;    // 물고기 잡기 시도 인터벌
    float m_fCatchProb;         // 물고기 잡기 확률

    double m_dNotColElapseTime; // NotColTime
    double m_dNotColJudTime;    // NotCol판단
};