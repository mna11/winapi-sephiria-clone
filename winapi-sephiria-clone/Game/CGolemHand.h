#pragma once
#include "CMonster.h"
#include "CState.h"

class CGolem;

enum class GOLEM_HAND_STATE
{
    IDLE,
    SLAM_READY,
    SLAM,
    RETURN,

    LASER_READY,    // 레이저 위치로 손 이동
    LASER_CREATE,   // 레이저 생성 
    LASER,          // 레이저 주와왕
    LASER_END,      // 레이저 소멸

    END
};

enum class GOLEM_HAND_ATK_RECT
{
    SLAM,
    LASER,
    END
};

class CGolemHand :
    public CMonster, public CState<GOLEM_HAND_STATE>
{
public:
    CGolemHand();
    ~CGolemHand();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void RenderLaser(Graphics*);
    void Release() override;

private:
    void UpdateTime() override;
    void Move() override;
    void Attack() override;
    void Down() override;

private:
    void ApplyChange() override;
    void UpdateFrame() override;
    void UpdateLaserFrame();
    void SetLaserFrame(int iStart, int iEnd, int iMotion, double dFrameSpeed);

public:
    void HitDamage(int iDamage, CObj* pObj = nullptr, HIT_SOURCE eHit = HIT_SOURCE::END, bool bCritical = false) override;

public:
    void SetOwner(CGolem* pGolem) { m_pOwner = pGolem; }
    void SetDirection(bool bLeft);
    void SetIdlePoint(VEC vPoint) { m_vIdlePoint = vPoint; }

public:
    void StartSlam();
    void StartLaser(VEC vStartPoint, VEC vEndPoint);

private:
    double  m_dStateTime;
    double  m_dSlamReadyTime;
    double  m_dSlamTime;

    // 레이저
    double  m_dLaserReadyTime;
    double  m_dLaserCreateTime;
    double  m_dLaserTime;
    double  m_dLaserEndTime;

    CGolem* m_pOwner;

    bool    m_bLeft;
    const TCHAR* m_pLaserFrameKey;

    VEC     m_vIdlePoint; 

    // SLAM
    VEC     m_vSlamReadyStartPoint;
    VEC     m_vSlamStartPoint;
    VEC     m_vSlamPoint;   // Slam시 바닥 타격 위치

    // 레이저
    FRAME   m_tLaserFrame;
    VEC     m_vLaserReadyStart;
    VEC     m_vLaserStart;
    VEC     m_vLaserEnd;
};

