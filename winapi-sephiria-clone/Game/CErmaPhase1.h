#pragma once
#include "CErmaPhase.h"

class CBossErma;

class CErmaPhase1 :
    public CErmaPhase
{
public:
    explicit CErmaPhase1(CBossErma* pBoss);
    ~CErmaPhase1();

public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Release() override;
};

