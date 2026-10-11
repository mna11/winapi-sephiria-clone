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

private:
    void StartMissile(int iType);

private:
    CErma* m_pErma;
    CGolemHead* m_pHead;
    CGolemHand* m_pLeftHand;
    CGolemHand* m_pRightHand;

    double m_dStateTime;

};
