#pragma once
#include "CObj.h"

#include "CSwordAndShield.h"

class CShield :
    public CObj
{
public:
    CShield();
    ~CShield();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void SetState(SWORD_AND_SHIELD_STATE* pWeaponState) { m_pWeaponState = pWeaponState; }
    void SetAtkInfo(ATK_INFO* pAtk)                     { m_pAtk = pAtk; }

private:
    void HandleIdleRender   (Graphics* pGraphics, Image* pImg, VEC& vScroll);
    void HandleAttackRender (Graphics* pGraphics, Image* pImg, VEC& vScroll);
    void HandleAttack1Render(Graphics* pGraphics, Image* pImg, VEC& vScroll);
    void HandleAttack2Render(Graphics* pGraphics, Image* pImg, VEC& vScroll);
    void HandleAttack3Render(Graphics* pGraphics, Image* pImg, VEC& vScroll);
    void HandleShieldRender (Graphics* pGraphics, Image* pImg, VEC& vScroll);
    void HandleCleaveRender (Graphics* pGraphics, Image* pImg, VEC& vScroll);

private:
    VEC                     m_vCellSize;
    SWORD_AND_SHIELD_STATE* m_pWeaponState;
    ATK_INFO*               m_pAtk;
};

