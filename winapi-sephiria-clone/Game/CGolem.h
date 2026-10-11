#pragma once

#include "CMonster.h"
#include "CState.h"

class CErma;
class CGolemHead;
class CGolemHand;

enum class GOLEM_STATE
{
    INTRO,      // 시작
    COMBAT,     // 전투 중
    EXPOSED,    // 현재 페이즈 끝
    RECOVER,    // 다시 에르마가 돌아올 때
    DOWN,       // 끝
    END,        
};

class CGolem :
    public CMonster, public CState<GOLEM_STATE>
{
public:
    CGolem();
    ~CGolem();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void ApplyChange() override;
    void HitDamage(int iDamage, CObj* pObj = nullptr, HIT_SOURCE eHit = HIT_SOURCE::END, bool bCritical = false) override;

private:
    void UpdateTime() override;
    void Move() override;
    void Attack() override;

public:
    void InitializeParts();
    void SetErma(CErma* pErma);

public:
    VEC GetEnterPoint() const;
    int GetPhase() const { return m_iPhase; }

private:
    void StartMissile(int iType);

private:
    CErma*      m_pErma;
    CGolemHead* m_pHead;
    CGolemHand* m_pLeftHand;
    CGolemHand* m_pRightHand;

    double      m_dStateTime;
    
    // 전투 패턴 관련
    // 페이즈
    int         m_iPhase;
    // 슬램 
    double      m_dSlamCycleElapsed;    // 사이클 지나는 시간
    double      m_dSlamCycle;           // 사이클 주기
    double      m_dSlamElapsed;         // 슬램 지나는 시간 
    double      m_dSlamInterval;        // 슬램 주기
    int         m_iSlamRemain;          // 현재 사이클에서 남은 슬램 수
    int         m_iSlamCount;           // 사이클 내에서 진행할 슬램 수
    bool        m_bNextSlamLeft;        // 다음 슬램은 어느 손인가
    // 미사일
    double      m_dMissileElapsed;
    double      m_dMissileInterval;
    // 레이저
    double      m_dLaserElapsed;
    double      m_dLaserInterval;
};
