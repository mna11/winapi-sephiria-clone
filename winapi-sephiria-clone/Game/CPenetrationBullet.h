#pragma once
#include "CBullet.h"
class CPenetrationBullet :
    public CBullet
{
public: 
    CPenetrationBullet();
    ~CPenetrationBullet(); 
protected:
    void HandleCollision() override;
    void OnHitObj(CObj* pObj) override;
    
private:
    unordered_set<int> m_setColObjID;
};

