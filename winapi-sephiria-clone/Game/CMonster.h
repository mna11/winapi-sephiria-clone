#pragma once
#include "CObj.h"
class CMonster abstract :
    public CObj
{
public:
    CMonster();
    virtual ~CMonster();

private:
    virtual void UpdateTime() PURE;
    virtual void Move() PURE;
    virtual void Attack() PURE;

protected:
    ImageAttributes m_imgAttrHit;
    ImageAttributes m_imgAttrDown;
};

