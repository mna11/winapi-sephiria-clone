#pragma once
#include "CMonster.h"

class CDummy :
    public CMonster
{
public:
    CDummy();
    ~CDummy();

public:
    void Initialize()                   override;
    int  Update()                       override;
    void LateUpdate()                   override;
    void Render(Graphics* pGraphics)    override;
    void Release()                      override;

private:
    void UpdateTime()   override;
    void Move()         override {};
    void Attack()       override {};
    void Down()         override {};
};