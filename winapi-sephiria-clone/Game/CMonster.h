#pragma once
#include "CObj.h"
class CMonster abstract :
    public CObj
{
public:
    CMonster();
    virtual ~CMonster();

private:
    virtual void UpdateTime()   PURE;   // 시간 변수 업데이트
    virtual void Move()         PURE;   // 이동 
    virtual void Attack()       PURE;   // 공격
    virtual void Down()         PURE;   // 사망

public:
    void HitDamage(int iDamage, CObj* pObj = nullptr, HIT_SOURCE eHit = HIT_SOURCE::END, bool bCritical = false) override;
    bool ApplyDamage(int iDamage, HIT_SOURCE eHit);
    void CreateHitEffect(int iDamage, CObj* pObj, HIT_SOURCE eHit, bool bCritical);

protected: 
    ImageAttributes m_imgAttrHit;  
    ImageAttributes m_imgAttrDown; 

    int             m_iLeaf;       // 사망 시, 드랍할 리프 
    int             m_iExp;        // 사망 시, 드랍할 경험치
};

