#pragma once
#include "CMonster.h"
#include "CState.h"

class CErmaPhase;

enum class BOSS_ERMA_STATE
{
    WAIT,    // 플레이어 오기 전
    INTRO,   // 플레이어 오고 인트로씬
    IDLE,    // 가만히 있을 때
    ATTACK,  // 손으로 찍기
    LASER,   // 레이저 발사하기
    MISSILE, // 미사일 발사하기
    END
};

class CBossErma :
    public CMonster, public CState<BOSS_ERMA_STATE>
{
public:
    CBossErma();
    ~CBossErma();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void ApplyChange() override;

public:
    void SetDamage(int iDamage, CObj* pObj) override;

public:
    const CObj* GetHead() const { return m_pHead; }
    const CObj* GetBody() const { return m_pBody; }
    const CObj* GetRightHand() const { return m_pRightHand; }
    const CObj* GetLeftHand() const { return m_pLeftHand; }
    const CObj* GetErma() const { return m_pErma; }


private:
    void UpdateTime();
    void ChangePhase(CErmaPhase* pNextPhase);

private:
    CObj* m_pHead;      // 골렘 머리
    CObj* m_pBody;      // 골렘 바디
    CObj* m_pRightHand; // 골렘 오른손
    CObj* m_pLeftHand;  // 골렘 왼손
    CObj* m_pErma;      // 미친 과학자 에르마

    CErmaPhase* m_pPhase;       // 페이즈
    double m_dStateElapseTime;  // 현재 스테이트에서 걸린 시간
};

