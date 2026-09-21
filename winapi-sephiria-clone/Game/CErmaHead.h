#pragma once

#include "CMonster.h"

class CBossErma;

class CErmaHead : public CMonster
{
public:
    CErmaHead();
    ~CErmaHead();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void SetOwner(CBossErma* pOwner) { m_pOwner = pOwner; }
    void RequestRemove() { m_bRemoveRequested = true; }

private:
    CBossErma* m_pOwner;
    bool m_bRemoveRequested;
};
