#pragma once
#include "CObj.h"
#include "CState.h"

class CWeaponController;

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
    void ApplyChange() override;

public:
    void Move();
    void Rotate();
    void Attack();

public:
    CWeaponController* m_pWeaponController;

#ifdef _DEBUG
private:
    void    PrintInfo();
    double  m_dPrintInterval = 3.;
#endif // DEBUG 
};

