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
    void UpdateTime();
    void UpdateInfo();
    void Move();

public:
    void SetCellSize(VEC vCellSize) { m_vCellSize = vCellSize; }
    void SetOwner(CObj* pObj) { m_pOwner = pObj; }
    void SetDamage(float fDamage) { m_fDamage = fDamage; }
    void SetAliveTime(double dAliveTime) { m_dAliveTime = dAliveTime; }
    void SetWallCollision(bool bCol) { m_bWallCol = bCol; }
    void SetWallNormal(VEC vNormal) { m_vWallNormal = vNormal; }

public:
    CObj*           GetOwner() { return m_pOwner; }
    const float&    GetDamage() { return m_fDamage; }

public:
    virtual void OnHitObj(CObj* pObj);

protected:
    virtual void HandleCollision();
    void CreateDisapperEffect();

protected:
    VEC m_vCellSize;
    
    float m_fDamage;
    CObj* m_pOwner;

    double m_dAliveTime;

    double m_dCreateEffectInterval;
    double m_dCreateEffectElapsedTIme;

    bool m_bWallCol;    // 벽과의 충돌 여부
    VEC  m_vWallNormal; // 충돌 법선
};

