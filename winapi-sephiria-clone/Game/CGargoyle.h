#pragma once
#include "CMonster.h"
#include "CState.h"

enum class GARGOYLE_STATE
{
    SUMMON,
    IDLE, 
    WALK,
    ATK,
    FLY_READY,
    FLY_CYCLE,
    FLY_DOWN,
    FLY_END,
    AIR,
    DOWN,
    END
};

enum class GARGOYLE_ATK_RECT
{
    ATK,     // 일반 공격
    FLY_ATK, // 공중 공격
    END
};

class CGargoyle :
    public CMonster, public CState<GARGOYLE_STATE>
{
public:
    CGargoyle();
    ~CGargoyle();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
public:
    void SetDamage(int iDamage, CObj* pObj) override;
    void ApplyChange() override;

private:
    void UpdateTime();
    void Move();
    void Attack();

private:
    // 여러 시간 변수
    double  m_dStateTime;           // 현재 상태에서 걸린 시간 - 이를 각 상태 시간과 비교할거임
    double  m_dSummonTime;          // 소환하는데 걸리는 시간
    double  m_dAtkTime;             // 총 공격하는데 걸리는 시간
    double  m_dAtkCollisionTime;    // 공격할 때, 공격 활성화되는 시작 시간
    double  m_dDownTime;           // 죽는 시간

    VEC     m_vAtkDir;              // 일반 공격 시 방향

    bool    m_bEffectCreate;        // 이펙트 생성 여부

#ifdef _DEBUG
private:
    void    PrintInfo();
    double  m_dPrintInterval = 1.;
#endif // DEBUG 
};

