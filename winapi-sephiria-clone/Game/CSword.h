#pragma once
#include "CObj.h"

#include "CSwordAndShield.h"

// 한손검 렌더 레이어가 검/방패 달라서 따로 만든 렌더용 클래스
// 충돌 등은 CSwordAndShield에서 담당한다.

class CSword :
    public CObj
{
public:
    CSword();
    ~CSword();
public:
    virtual void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void SetState(SWORD_AND_SHIELD_STATE* pWeaponeState) { m_pWeaponState = pWeaponeState; }
    void SetAtkInfo(ATK_INFO* pAtk)                      { m_pAtk = pAtk; }

private:
    virtual void HandleIdleRender   (Graphics* pGraphics, Image* pImg, VEC& vScroll) PURE;
    virtual void HandleAttackRender (Graphics* pGraphics, Image* pImg, VEC& vScroll) PURE;
    virtual void HandleAttack1Render(Graphics* pGraphics, Image* pImg, VEC& vScroll) PURE;
    virtual void HandleAttack2Render(Graphics* pGraphics, Image* pImg, VEC& vScroll) PURE;
    virtual void HandleAttack3Render(Graphics* pGraphics, Image* pImg, VEC& vScroll) PURE;
    virtual void HandleShieldRender (Graphics* pGraphics, Image* pImg, VEC& vScroll) PURE;
    virtual void HandleCleaveRender (Graphics* pGraphics, Image* pImg, VEC& vScroll) PURE;

protected:
    void DrawSword(Graphics* pGraphics, Image* pImg, VEC& vScroll, float fTargetAngle = 0);

protected:
    VEC                     m_vCellSize;

    // Sword and Shield에서 참조
    SWORD_AND_SHIELD_STATE* m_pWeaponState;
    ATK_INFO*               m_pAtk;
};

