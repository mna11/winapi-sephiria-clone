#pragma once
#include "CSword.h"

// 충돌 관련해서는 Sword And Shield
// 초기화(Initialize), 위치(Update)는 CSword에서 관리
// 하위 Sword에서는 렌더만 신경쓰면 된다.

class CNormalSword :
    public CSword
{
public:
    CNormalSword();
    ~CNormalSword();
public:
    void HandleIdleRender(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
    void HandleAttackRender(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
    void HandleAttack1Render(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
    void HandleAttack2Render(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
    void HandleAttack3Render(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
    void HandleShieldRender(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
    void HandleCleaveRender(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
};

