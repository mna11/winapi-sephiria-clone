#pragma once
#include "CObj.h"
class CBullet :
    public CObj
{
public:
    CBullet();
    ~CBullet();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void Move();
};

