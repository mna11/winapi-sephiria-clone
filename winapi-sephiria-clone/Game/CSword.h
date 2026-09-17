#pragma once
#include "CObj.h"

#include "CSwordAndShield.h"

class CSword :
    public CObj
{
public:
    CSword();
    ~CSword();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void SetState(SWORD_AND_SHIELD_STATE* pWeaponeState) { m_pWeaponState = pWeaponeState; }
    void SetAtkInfo(ATK_INFO* pAtk)                      { m_pAtk = pAtk; }

private: // 각 상태별 업데이트 
    void HandleIdleUpdate   ();
    void HandleAttackUpdate ();
    void HandleAttack1Update();
    void HandleAttack2Update();
    void HandleAttack3Update();
    void HandleShieldUpdate ();
    void HandleCleaveUpdate ();

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

    // Sword and Shield에서 참조
    SWORD_AND_SHIELD_STATE* m_pWeaponState;
    ATK_INFO*               m_pAtk;

    bool                    m_bAtkStart;
};

