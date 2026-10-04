#pragma once
#include "CObj.h"

// 몬스터 처치 또는 항아리같은거 깨면 경험치 || 돈이 나오는데 그 때의 경험치와 돈이다.

class CDrop :
    public CObj
{
public:
    CDrop();
    ~CDrop();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
    
public:
    void SetAmount(int iAmount) { m_iAmount = iAmount; }
    void SetBattleEnd(bool bBattleEnd) { m_bBattleEnd = bBattleEnd; }
    void SetDropType(DROP_TYPE eDropType);

public:
    const int& GetAmount() const { return m_iAmount; }
    const DROP_TYPE& GetDropType() const { return m_eDropType; }

public:
    void UpdateInteractRect();

private:
    void Magnet();

private:
    DROP_TYPE  m_eDropType; // 경험치인지 리프인지

    int     m_iAmount;          // 경험치량 || 리프량
    RECT    m_tInteractRect;   // 만약 타겟이 여기에 닿으면 타겟쪽으로 PickUp들이 빨려감
    bool    m_bBattleEnd;   // 전투 종료 플래그
};

