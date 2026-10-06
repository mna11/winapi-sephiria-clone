#pragma once
#include "CShield.h"
class CNormalShield :
    public CShield
{
public:
    CNormalShield();
    ~CNormalShield();
private:
    void HandleIdleRender(Graphics* pGraphics, Image* pImg, VEC& vScroll);
    void HandleAttackRender(Graphics* pGraphics, Image* pImg, VEC& vScroll);
    void HandleAttack1Render(Graphics* pGraphics, Image* pImg, VEC& vScroll);
    void HandleAttack2Render(Graphics* pGraphics, Image* pImg, VEC& vScroll);
    void HandleAttack3Render(Graphics* pGraphics, Image* pImg, VEC& vScroll);
    void HandleShieldRender(Graphics* pGraphics, Image* pImg, VEC& vScroll);
    void HandleCleaveRender(Graphics* pGraphics, Image* pImg, VEC& vScroll);
};

