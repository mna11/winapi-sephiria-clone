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
    void UpdateInfo();
    void Move();

public:
    void SetDirVector(VEC vDir) { m_vDir = vDir; }
    void SetCellSize(VEC vCellSize) { m_vCellSize = vCellSize; }
    void SetOwner(CObj* pObj) { m_pOwner = pObj; }
    void SetDamage(float fDamage) { m_fDamage = fDamage; }

public:
    CObj*           GetOwner() { return m_pOwner; }
    const float&    GetDamage() { return m_fDamage; }

private:
    void CreateDisapperEffect();

private:
    VEC m_vDir;
    VEC m_vCellSize;
    
    float m_fDamage;
    CObj* m_pOwner;
};

