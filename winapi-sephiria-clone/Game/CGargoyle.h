#pragma once
#include "CMonster.h"
#include "CState.h"

enum class GARGOYLE_STATE
{
    SUMMON,
    IDLE, 
    WALK,
    ATK,
    FLY_ATK,
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

    double  m_dAtkTime;             // 일반 공격하는데 걸리는 총 시간
    double  m_dAtkCollisionTime;    // 공격할 때, 공격 활성화되는 시작 시간
    double  m_dDownTime;            // 죽는 시간

    double  m_dFlyAtkTime;          // 공중 공격하는데 걸리는 총 시간
    double  m_dFlyAtkCycleTime;     // 공중 공격시 가고일 날아오르는 시작 시간
    double  m_dFlyAtkDownTime;      // 공중 공격시 가고일 떨어지는 시작 시간
    double  m_dFlyAtkEndTime;       // 공중 공격시 가고일 떨어지고 난 뒤 시간
    double  m_dFlyAtkCollisionTime; // 공중 공격할 때, 공격 활성화되는 시작 시간

    VEC     m_vAtkDir;              // 일반 공격 시 방향
    VEC     m_vFlyAtkPoint;         // 공중 공격 시 목표 위치
    VEC     m_vFlyAtkStartPoint;    // 공중 공격 시 시작 위치

    bool    m_bEffectCreate;        // 이펙트 생성 여부


    // 공격 기준 - 플레이어와의 거리
    float m_fAtkDistance;
    // 공격 기준2 
    float m_fFlyAtkDistance;


#ifdef _DEBUG
private:
    void    PrintInfo();
    double  m_dPrintInterval = 1.;
#endif // DEBUG 
};

