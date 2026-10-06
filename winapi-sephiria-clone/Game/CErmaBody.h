#pragma once

#include "CMonster.h"

class CBossErma;

class CErmaBody : public CMonster
{
public:
    CErmaBody();
    ~CErmaBody();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
    void HitDamage(int iDamage, CObj* pObj = nullptr, HIT_SOURCE eHit = HIT_SOURCE::END) override;

public:
    void SetOwner(CBossErma* pOwner) { m_pOwner = pOwner; }
    void RequestRemove() { m_bRemoveRequested = true; }

private:
    CBossErma* m_pOwner;
    bool m_bRemoveRequested;
};
