#include "pch.h"
#include "CPenetrationBullet.h"

#include "CSoundMgr.h"

CPenetrationBullet::CPenetrationBullet()
{
}

CPenetrationBullet::~CPenetrationBullet()
{
}

void CPenetrationBullet::HandleCollision()
{
    if (!m_bWallCol)
        return;

    if (!lstrcmp(m_pFrameKey, L"ShieldBullet"))
        CSoundMgr::GetInstance()->PlaySound(L"ShieldHit.wav", CHANNEL_GROUPID::SFX, 1.f);

    VEC vDir{ cosf(m_fAngle), sinf(m_fAngle) };

    float fDot = vDir.Dot(m_vWallNormal);
    if (fDot < 0.f)
    {
        vDir = vDir - m_vWallNormal * (2.f * fDot);
        m_fAngle = atan2f(vDir.fY, vDir.fX);
    }

    m_setColObjID.clear();
    m_bWallCol = false;
}

void CPenetrationBullet::OnHitObj(CObj* pObj)
{
    // 동일한 원소가 있었음 -> 데미지를 주지 않고 return
    if (!m_setColObjID.insert(pObj->GetObjID()).second)
        return;

    bool bCritical = (uniform_real_distribution<float>(0.f, 1.f)(g_engine) < m_pOwner->GetStat().fCriticalChange);
    if (bCritical)
        m_fDamage = static_cast<int>(m_fDamage * (1.f + m_pOwner->GetStat().fCriticalDamage));

    pObj->HitDamage(m_fDamage, m_pOwner, HIT_SOURCE::SLASH, bCritical);
}

