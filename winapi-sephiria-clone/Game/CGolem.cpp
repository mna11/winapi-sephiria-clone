#include "pch.h"
#include "CGolem.h"

#include "CErma.h"
#include "CGolemHead.h"
#include "CGolemHand.h"
#include "CGolemMissile.h"

#include "CAbstractFactory.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CTimeMgr.h"
#include "CUIMgr.h"
#include "CEffectMgr.h"
#include "CSoundMgr.h"
#include "CKeyMgr.h"

CGolem::CGolem()
    : CState(GOLEM_STATE::END, GOLEM_STATE::INTRO),
    m_pErma(nullptr), m_pHead(nullptr), m_pLeftHand(nullptr), m_pRightHand(nullptr),
    m_dStateTime(0.), m_iPhase(0),
    m_dSlamCycleElapsed(0.), m_dSlamCycle(0.), m_dSlamElapsed(0.), m_dSlamInterval(0.), m_iSlamRemain(0), m_iSlamCount(0), m_bNextSlamLeft(true),
    m_dMissileElapsed(0.), m_dMissileInterval(0.),
    m_dLaserElapsed(0.), m_dLaserInterval(0.)
{
}

CGolem::~CGolem()
{
    Release();
}

void CGolem::Initialize()
{
    m_tInfo = { 0.f, 0.f, 0.f, 0.f };
    m_tStat = { 600, 600, 30 };

    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 1;

    CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Golem/Golem_Head.png", L"GolemHead");
    CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Golem/Golem_Body.png", L"GolemBody");
    CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Golem/Golem_Hand_LEFT.png", L"GolemHand_LEFT");
    CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Golem/Golem_Hand_RIGHT.png", L"GolemHand_RIGHT");
    CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Golem_Laser_LEFT.png", L"Golem_Laser_LEFT");
    CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Golem_Laser_RIGHT.png", L"Golem_Laser_RIGHT");


    m_dSlamCycle = 7.;
    m_dSlamInterval = 1.0;
    m_iSlamCount = 4;

    m_dMissileInterval = 3.;

    m_dLaserInterval = 12.;
}

int CGolem::Update()
{
    if (m_bDead)
        return DEAD;

#ifdef _DEBUG
    if (KEY_DOWN(VK_F1) && m_pLeftHand != nullptr)
        m_pLeftHand->StartSlam();

    if (KEY_DOWN(VK_F2) && m_pRightHand != nullptr)
        m_pRightHand->StartSlam();

    if (KEY_DOWN(VK_F3))
    {
        StartMissile(0);
    }

    if (KEY_DOWN(VK_F4) && m_pLeftHand != nullptr && m_pRightHand != nullptr)
    {
        m_pLeftHand->StartLaser(
            VEC{ 3280.f, 1340.f },
            VEC{ 3280.f, 2040.f }
        );

        m_pRightHand->StartLaser(
            VEC{ 4640.f, 2040.f },
            VEC{ 4640.f, 1340.f }
        );
    }
#endif

    ApplyChange();
    UpdateTime();
    Attack();

    __super::UpdateRect();
    __super::UpdateFrame();

    return NOEVENT;
}

void CGolem::LateUpdate()
{
}

void CGolem::Render(Graphics* pGraphics)
{
    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

    // 몸 그리기
    Image* pBodyImg = CImgMgr::GetInstance()->FindImg(L"GolemBody");
    if (nullptr == pBodyImg)
        return;

    VEC vCellSize{ 113.f, 55.f };
    VEC vImgSize = vCellSize * PIXEL_SCALE;
    VEC vOffset{ vScroll.fX, vScroll.fY };
    RectF DestRect{ m_tInfo.vPoint.fX + vOffset.fX - vImgSize.fX * 0.5f, m_tInfo.vPoint.fY + vOffset.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };
    bool bDown = (m_eCurState == GOLEM_STATE::DOWN);
    
    pGraphics->DrawImage(
        pBodyImg, DestRect,
        (bDown ? vCellSize.fX : 0.f), 0, vCellSize.fX, vCellSize.fY,
        UnitPixel
    );
}

void CGolem::Release()
{
}

void CGolem::ApplyChange()
{
    if (m_eCurState != m_eNextState)
    {

        switch (m_eNextState)
        {
        case GOLEM_STATE::INTRO:

            break;

        case GOLEM_STATE::COMBAT:
        {
            m_dSlamCycleElapsed = m_dSlamCycle * 0.75;
            m_dSlamElapsed = 0.;
            m_iSlamRemain = 0;
            m_bNextSlamLeft = true;

            m_dMissileElapsed = 0.;
            m_dLaserElapsed = m_dLaserInterval;
            break;
        }
        case GOLEM_STATE::EXPOSED:
        {
            m_iSlamRemain = 0;

            if (m_pLeftHand != nullptr)
                m_pLeftHand->RequestChange(GOLEM_HAND_STATE::RETURN);

            if (m_pRightHand != nullptr)
                m_pRightHand->RequestChange(GOLEM_HAND_STATE::RETURN);

            if (m_pHead != nullptr)
                m_pHead->RequestChange(GOLEM_HEAD_STATE::EXPOSE);

            if (m_pErma != nullptr)
                m_pErma->StartExpose(GetEnterPoint() + VEC{ 0.f, 350.f });

            break;
        }

        case GOLEM_STATE::RECOVER:
        {
            if (m_eCurState == GOLEM_STATE::EXPOSED)
            {
                ++m_iPhase;

                m_tStat.iHp = m_tStat.iMaxHp;
                m_bHit = false;
                m_dHitElapseTime = 0.;
            }

            if (nullptr != m_pHead)
                m_pHead->RequestChange(GOLEM_HEAD_STATE::RECOVER);

            break;
        }
        case GOLEM_STATE::DOWN:
        {
            if (m_pHead != nullptr)
                m_pHead->RequestChange(GOLEM_HEAD_STATE::DOWN);

            if (m_pErma != nullptr)
                m_pErma->StartExpose(GetEnterPoint() + VEC{ 0.f, 350.f });
            break;
        }
        default:
            break;
        }

        m_eCurState = m_eNextState;
        m_dStateTime = 0.;
    }
}

void CGolem::HitDamage(int iDamage, CObj* pObj, HIT_SOURCE eHit, bool bCritical)
{
    if (m_bHit)
        return;

    if (m_eCurState != GOLEM_STATE::COMBAT)
        return;

    m_bHit = true;		// m_bHit은 m_bHit이 된지 경과한 시간이 iframeTime을 넘으면 false가 된다.

    m_tStat.iHp -= iDamage;

    if (m_tStat.iHp <= 0)
    {
        m_tStat.iHp = 0;
        RequestChange(GOLEM_STATE::EXPOSED);
    }

    // 사운드
    switch (eHit)
    {
    case HIT_SOURCE::SLASH:
        CSoundMgr::GetInstance()->PlaySound(L"HitSword02.wav", CHANNEL_GROUPID::SFX, 1.f);
        break;
    case HIT_SOURCE::BULLET:
        CSoundMgr::GetInstance()->PlaySound(L"HitMagicFire02.wav", CHANNEL_GROUPID::SFX, 1.f);
        break;
    }
}

void CGolem::UpdateTime()
{
    m_dStateTime += DT;

    if (m_bHit)
    {
        m_dHitElapseTime += DT;
        if (m_dHitElapseTime >= m_dIframeTime)
        {
            m_bHit = false;
            m_dHitElapseTime = 0.;
        }
    }

    switch (m_eCurState)
    {
    case GOLEM_STATE::RECOVER:
    {
        if (m_pHead != nullptr && m_pHead->GetState() == GOLEM_HEAD_STATE::IDLE && m_iPhase <= 1)
            RequestChange(GOLEM_STATE::COMBAT);
        break;
    }
    case GOLEM_STATE::COMBAT:
    {
        m_dMissileElapsed += DT;
        m_dSlamCycleElapsed += DT;
        if (m_iPhase >= 1)
            m_dLaserElapsed += DT;
        break;
    }
    }
}

void CGolem::Move()
{
}

void CGolem::Attack()
{
    // 전투 중이 아니라면 리턴
    if (m_eCurState != GOLEM_STATE::COMBAT)
        return;

    if (m_pLeftHand == nullptr || m_pRightHand == nullptr)
        return;

    switch (m_iPhase)
    {
    case 0:
    {
        if (m_pTarget != nullptr &&
            m_dMissileElapsed >= m_dMissileInterval)
        {
            StartMissile(0);
            m_dMissileElapsed = 0.;
        }


        // 슬램 주기가 되었음
        if (m_dSlamCycleElapsed >= m_dSlamCycle)
        {
            m_dSlamCycleElapsed = 0.;
            m_iSlamRemain = m_iSlamCount;
            
            m_dSlamElapsed = m_dSlamInterval;
        }

        m_dSlamElapsed += DT;
        if (m_iSlamRemain != 0 && m_dSlamElapsed >= m_dSlamInterval)
        {
            CGolemHand* pHand = m_bNextSlamLeft ? m_pLeftHand : m_pRightHand;
            pHand->StartSlam();
            m_dSlamElapsed = 0.;
            --m_iSlamRemain;
            m_bNextSlamLeft = !m_bNextSlamLeft;
        }
        break;
    }
    case 1:
    {
        if (m_pTarget != nullptr &&
            m_dMissileElapsed >= m_dMissileInterval)
        {
            StartMissile(0);
            m_dMissileElapsed = 0.;
        }

        // 레이저 주기가 되었음
        if (m_dLaserElapsed >= m_dLaserInterval)
        {
            m_dLaserElapsed = 0.;

            m_pLeftHand->StartLaser(
                VEC{ 3280.f, 1340.f },
                VEC{ 3280.f, 2040.f }
            );

            m_pRightHand->StartLaser(
                VEC{ 4640.f, 2040.f },
                VEC{ 4640.f, 1340.f }
            );

            break;
        }

        // 슬램 주기가 되었음
        if (m_dSlamCycleElapsed >= m_dSlamCycle)
        {
            m_dSlamCycleElapsed = 0.;
            m_iSlamRemain = m_iSlamCount;

            m_dSlamElapsed = m_dSlamInterval;
        }

        m_dSlamElapsed += DT;
        if (m_iSlamRemain != 0 && m_dSlamElapsed >= m_dSlamInterval)
        {
            CGolemHand* pHand = m_bNextSlamLeft ? m_pLeftHand : m_pRightHand;
            pHand->StartSlam();
            m_dSlamElapsed = 0.;
            --m_iSlamRemain;
            m_bNextSlamLeft = !m_bNextSlamLeft;
        }

        break;
    }
    }
}

void CGolem::InitializeParts()
{
    // 머리 세팅
    if (nullptr == m_pHead)
    {
        m_pHead = static_cast<CGolemHead*>(CAbstractFactory<CGolemHead>::CreateObj());
        CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pHead);
        VEC vOffset{ 0.f, -30.f };
        m_pHead->SetPos(m_tInfo.vPoint + vOffset);
        m_pHead->SetOwner(this);
    }

    // 왼손 세팅
    if (nullptr == m_pLeftHand)
    {
        m_pLeftHand = static_cast<CGolemHand*>(CAbstractFactory<CGolemHand>::CreateObj());
        CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pLeftHand);
        VEC vOffset{ -480.f, 100.f };
        m_pLeftHand->SetPos(m_tInfo.vPoint + vOffset);
        m_pLeftHand->SetIdlePoint(m_tInfo.vPoint + vOffset);
        m_pLeftHand->SetOwner(this);
        m_pLeftHand->SetDirection(true);
    }

    // 오른손 세팅
    if (nullptr == m_pRightHand)
    {
        m_pRightHand = static_cast<CGolemHand*>(CAbstractFactory<CGolemHand>::CreateObj());
        CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pRightHand);
        VEC vOffset{ 480.f, 100.f };
        m_pRightHand->SetPos(m_tInfo.vPoint + vOffset);
        m_pRightHand->SetIdlePoint(m_tInfo.vPoint + vOffset);
        m_pRightHand->SetOwner(this);
        m_pRightHand->SetDirection(false);
    }
}

void CGolem::SetErma(CErma* pErma)
{
    if (nullptr == m_pErma)
    {
        m_pErma = pErma; 
        m_pErma->SetOwner(this);
    }
}

VEC CGolem::GetEnterPoint() const
{
    VEC vOffset{ 8.f, 7.f };
    return m_pHead->GetInfo().vPoint + vOffset;
}

// 미사일 발사 패턴
void CGolem::StartMissile(int iType)
{
    if (m_pTarget == nullptr)
        return;

    VEC vTargetPoint = m_pTarget->GetInfo().vPoint;

    // 미사일 발사 이펙트 생성
    VEC vOffset[3] = {
        {0.f, -20.f},
        {-20.f, 20.f},
        {20.f, 20.f}
    };
    for (int i = 0; i < 3; ++i)
    {
        VEC vCenter = { 650.f , -100.f };
        VEC vLeftPoint{ m_tInfo.vPoint.fX - vCenter.fX + vOffset[i].fX, m_tInfo.vPoint.fY + vCenter.fY + vOffset[i].fY };
        VEC vRightPoint{ m_tInfo.vPoint.fX + vCenter.fX + vOffset[i].fX, m_tInfo.vPoint.fY + vCenter.fY + vOffset[i].fY };

        CEffectMgr::GetInstance()->CreateEffect(L"Golem_Missile_U", vLeftPoint, EFTMGR_FIXED | EFTMGR_IMAGE | EFTMGR_SCROLL, 0.f, 0.1f);
        CEffectMgr::GetInstance()->CreateEffect(L"Golem_Missile_U", vRightPoint, EFTMGR_FIXED | EFTMGR_IMAGE | EFTMGR_SCROLL, 0.f, 0.1f);
    }
    CSoundMgr::GetInstance()->PlaySound(L"GolemMissileUp.wav", CHANNEL_GROUPID::SFX, 1.f);

    switch (iType)
    {
    case 0:
    {
        VEC vOffset{ vTargetPoint.fX - m_tInfo.vPoint.fX, 150.f };
        VEC vGap{ 0.f, 150.f };
        for (int i = 0; i < 6; ++i)
        {
            VEC vPoint = m_tInfo.vPoint + vOffset + vGap * i;
            CGolemMissile* pMissile = static_cast<CGolemMissile*>(CAbstractFactory<CGolemMissile>::CreateObj(vPoint.fX,vPoint.fY));
            pMissile->SetDelay(i * 0.1);
            CObjMgr::GetInstance()->AddObject(OBJID::MONSTER,pMissile);
        }

        break;
    }
    default:
        break;
    }
}
