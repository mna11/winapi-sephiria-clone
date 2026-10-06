#pragma once
#include "CObj.h"
#include "CState.h"

class CWeaponController;
class CInventory;

// State 상속을 위해 클래스 내부 선언이 아닌 밖에서 함
// Define에 안한 이유는 헷갈릴까봐
// 특정 Obj 클래스에 대한 상태 enum은 각 헤더에서 하겠다.
enum class PLAYER_STATE {
    IDLE,
    WALK,
    ATTACK,
    HEAVY_ATTACK,
    WHIRLWIND_READY,
    WHIRLWIND_CYCLE,
    AIR,
    DOWN,
    END
};

class CPlayer :
    public CObj, public CState<PLAYER_STATE>
{
public:
    CPlayer();
    ~CPlayer();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    CWeaponController*    GetWeaponController() { return m_pWeaponController; }
    const double&               GetDashRecoveryInterval()   const   { return m_dDashRecorveyInterval; }
    const double&               GetDashRecoveryElapse()     const   { return m_dDashRecoveryElapseTime; }
    const int&                  GetLeaf()                   const { return m_iLeaf; }
    CInventory*                 GetInventory()              const { return m_pInventory; }
    const int&                  GetMaxExp()                 const { return m_iMaxExp; }
    const int&                  GetExp()                    const { return m_iExp; }
    const int&                  GetLevelUp()                const { return m_iLevelUp; }
    const int&                  GetDice()                   const { return m_iDice; }

public:
    void                        SetPlayerBehaviorEnable(bool bEnable) { m_bPlayerBehaviorEnable = bEnable; }
    void                        SetLeaf(int iLeaf) { m_iLeaf = iLeaf; }

public:
    void                        AddLeaf(int iAmount) { m_iLeaf += iAmount; }
    void                        AddExp(int iAmount);
    void                        AddLevelUp(int iAmount) { m_iLevelUp += iAmount; }
    void                        AddDice(int iAmount); 

public:
    void HitDamage(int iDamage, CObj* pObj = nullptr, HIT_SOURCE eHit = HIT_SOURCE::END) override;

public:
    void ApplyChange() override;

public:
    void UpdateTime();
    void Move();
    void Dash();
    void Rotate();
    void Attack();

public:
    void CreateEffect();

private:
    CWeaponController*  m_pWeaponController;
    CInventory*         m_pInventory;

    // 대시 회복 시간
    double              m_dDashRecorveyInterval;
    double              m_dDashRecoveryElapseTime;

    // 달리기 속도
    float               m_fNormalSpeed;
    float               m_fRunSpeed;

    // 먼지 이펙트 발생 간격
    double              m_dDustInterval;
    double              m_dDustElapseTime;

    // 플레이어 행동 가능 - UI 때문에 만듬
    bool                m_bPlayerBehaviorEnable;
    
    // 돈
    int                 m_iLeaf;


    // 경험치
    int                 m_iExp;
    const int           m_iMaxExp;
    int                 m_iLevelUp; // 레벨업 횟수

    // 주사위
    int                 m_iDice;
#ifdef _DEBUG
private:
    void    PrintInfo();
    double  m_dPrintInterval = 3.;
#endif // DEBUG 
};

