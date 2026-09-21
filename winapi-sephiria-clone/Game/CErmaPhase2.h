#pragma once
#include "CErmaPhase.h"

class CBossErma;

class CErmaPhase2 :
    public CErmaPhase
{
public:
    explicit CErmaPhase2(CBossErma* pBoss);
    ~CErmaPhase2();

public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Release() override;
};

