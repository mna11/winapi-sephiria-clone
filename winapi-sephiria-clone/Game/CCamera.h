#pragma once
#include "CObj.h"
class CCamera :
    public CObj
{
public:
    // NORMAL은 플레이어 따라다니는 상태
    // FIXED는 특정 위치에 고정된 상태
    // MOVE_TO는 특정 위치로 가는 중인 상태
    enum class STATE { NORMAL, FIXED, MOVE_TO, END };

public:
    CCamera();
    ~CCamera();
public:
    void Initialize() override;
    int  Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void SetState(STATE eState) { m_eCurState = eState; }
    void SetTargetPoint(VEC vTargetPoint) { m_vTargetPoint = vTargetPoint; }

public: // 상태별 Update에서 수행할 함수
    void HandleBehavior(); 
    void HandleNormal();
    void HandleFixed();
    void HandleMoveTo();

public:
    void ChangeState();
    void Shaking(int iStrength, double dTime);

private:
    STATE m_eCurState;
    STATE m_ePreState; 

    VEC   m_vTargetPoint;
    VEC   m_vOffset;

    bool   m_bShaking;
    uniform_int_distribution<int> m_disShaking;
    double m_dShakingRunningTime;
};

