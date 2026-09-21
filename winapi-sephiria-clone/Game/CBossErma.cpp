#include "pch.h"
#include "CBossErma.h"

#include "CErma.h"
#include "CErmaBody.h"
#include "CErmaHand.h"
#include "CErmaHead.h"
#include "CErmaLaser.h"
#include "CErmaMissile.h"
#include "CErmaPhase.h"
#include "CErmaPhase1.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CTimeMgr.h"
#include "CUIMgr.h"

namespace
{
    constexpr float HEAD_OFFSET_Y = -100.f;
    constexpr float STUN_OFFSET_Y = 350.f;
    constexpr float HAND_OFFSET_X = 480.f;
}

CBossErma::CBossErma()
    : CState(BOSS_ERMA_STATE::END, BOSS_ERMA_STATE::WAIT),
    m_pHead(nullptr),
    m_pBody(nullptr),
    m_pRightHand(nullptr),
    m_pLeftHand(nullptr),
    m_pAttackingHand(nullptr),
    m_pErma(nullptr),
    m_pPhase(nullptr),
    m_dStateElapseTime(0.),
    m_bNextLeftHand(false),
    m_iNextPattern(0),
    m_bBattleStarted(false),
    m_bPartsInitialized(false),
    m_bEnding(false)
{
}

CBossErma::~CBossErma()
{
    Release();
}

void CBossErma::Initialize()
{
    // The controller is intentionally non-collidable. Body and hands own the
    // collision rectangles and forward stagger damage here.
    m_tInfo = { 0.f, 0.f, 0.f, 0.f };
    m_tStat = { 600, 600, 30 };

    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 0;

    m_dIframeTime = 0.16;
    m_dHitElapseTime = 0.;
    m_bHit = false;

    ChangePhase(new CErmaPhase1(this));
}

int CBossErma::Update()
{
    if (m_bDead)
        BeginEnding();

    if (m_bEnding)
        return DEAD;

    UpdateTime();

    // Damage is applied during LateUpdate, so consume that pending transition
    // before the phase can schedule its next normal action.
    ApplyChange();

    if (m_pPhase != nullptr)
        m_pPhase->Update();

    ApplyChange();

    if (m_pErma != nullptr && m_pErma->IsDefeated())
    {
        BeginEnding();
        return DEAD;
    }

    __super::UpdateRect();
    return NOEVENT;
}

void CBossErma::LateUpdate()
{
    if (m_pPhase != nullptr)
        m_pPhase->LateUpdate();
}

void CBossErma::Render(Graphics* pGraphics)
{
}

void CBossErma::Release()
{
    SafeDelete<CErmaPhase*>(m_pPhase);
}

void CBossErma::ApplyChange()
{
    if (m_eCurState == m_eNextState)
        return;

    m_eCurState = m_eNextState;
    m_dStateElapseTime = 0.;

    switch (m_eCurState)
    {
    case BOSS_ERMA_STATE::INTRO:
        m_bBattleStarted = true;
        if (m_pErma != nullptr)
            CUIMgr::GetInstance()->ShowUI(UIID::BOSS_HP, m_pErma);
        break;

    case BOSS_ERMA_STATE::ATTACK:
        if (m_pAttackingHand != nullptr && m_pTarget != nullptr)
            m_pAttackingHand->StartSlam(m_pTarget->GetInfo().vPoint);
        break;

    case BOSS_ERMA_STATE::MISSILE:
        StartMissileVolley();
        break;

    case BOSS_ERMA_STATE::LASER:
        StartLaserPattern();
        break;

    case BOSS_ERMA_STATE::EXPOSED:
        if (m_pLeftHand != nullptr)
            m_pLeftHand->ForceBreak();
        if (m_pRightHand != nullptr)
            m_pRightHand->ForceBreak();
        if (m_pErma != nullptr)
        {
            const VEC vHeadPoint = m_tInfo.vPoint + VEC{ 0.f, HEAD_OFFSET_Y };
            const VEC vStunPoint = m_tInfo.vPoint + VEC{ 0.f, STUN_OFFSET_Y };
            m_pErma->StartExposure(vHeadPoint, vStunPoint);
        }
        break;

    case BOSS_ERMA_STATE::RECOVER:
        m_tStat.iHp = m_tStat.iMaxHp;
        if (m_pErma != nullptr)
        {
            m_pErma->SetExposed(false);
            m_pErma->SetVisible(false);
        }
        if (m_pLeftHand != nullptr)
            m_pLeftHand->Restore();
        if (m_pRightHand != nullptr)
            m_pRightHand->Restore();
        break;

    default:
        break;
    }
}

void CBossErma::SetDamage(int iDamage, CObj* pObj)
{
    AddStaggerDamage(iDamage);
}

void CBossErma::InitializeParts()
{
    if (m_bPartsInitialized)
        return;

    m_pBody = static_cast<CErmaBody*>(CAbstractFactory<CErmaBody>::CreateObj());
    m_pHead = static_cast<CErmaHead*>(CAbstractFactory<CErmaHead>::CreateObj());
    m_pLeftHand = static_cast<CErmaHand*>(CAbstractFactory<CErmaHand>::CreateObj());
    m_pRightHand = static_cast<CErmaHand*>(CAbstractFactory<CErmaHand>::CreateObj());
    m_pErma = static_cast<CErma*>(CAbstractFactory<CErma>::CreateObj());

    m_pBody->SetOwner(this);
    m_pBody->SetPos(m_tInfo.vPoint);

    m_pHead->SetOwner(this);
    m_pHead->SetPos(m_tInfo.vPoint + VEC{ 0.f, HEAD_OFFSET_Y });

    m_pLeftHand->SetSide(ERMA_HAND_SIDE::LEFT);
    m_pLeftHand->SetOwner(this);
    m_pLeftHand->SetAnchor(m_tInfo.vPoint + VEC{ -HAND_OFFSET_X, 0.f });

    m_pRightHand->SetSide(ERMA_HAND_SIDE::RIGHT);
    m_pRightHand->SetOwner(this);
    m_pRightHand->SetAnchor(m_tInfo.vPoint + VEC{ HAND_OFFSET_X, 0.f });

    m_pErma->SetManagedByGolem(true);
    m_pErma->SetExposed(false);
    m_pErma->SetVisible(false);
    m_pErma->SetAnchor(m_tInfo.vPoint + VEC{ 0.f, HEAD_OFFSET_Y });

    CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pBody);
    CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pHead);
    CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pLeftHand);
    CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pRightHand);
    CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pErma);

    m_bPartsInitialized = true;
}

bool CBossErma::AddStaggerDamage(int iDamage)
{
    if (!m_bBattleStarted ||
        m_eCurState == BOSS_ERMA_STATE::INTRO ||
        m_eCurState == BOSS_ERMA_STATE::EXPOSED ||
        m_eCurState == BOSS_ERMA_STATE::RECOVER ||
        m_bHit)
    {
        return false;
    }

    m_bHit = true;
    m_tStat.iHp = std::clamp(m_tStat.iHp - iDamage, 0, m_tStat.iMaxHp);

    if (m_tStat.iHp == 0)
        m_eNextState = BOSS_ERMA_STATE::EXPOSED;

    return true;
}

void CBossErma::UpdateTime()
{
    m_dStateElapseTime += DT;

    if (m_bHit)
    {
        m_dHitElapseTime += DT;
        if (m_dHitElapseTime >= m_dIframeTime)
        {
            m_bHit = false;
            m_dHitElapseTime = 0.;
        }
    }
}

void CBossErma::ChangePhase(CErmaPhase* pNextPhase)
{
    if (m_pPhase == pNextPhase)
        return;

    SafeDelete<CErmaPhase*>(m_pPhase);
    m_pPhase = pNextPhase;

    if (m_pPhase != nullptr)
        m_pPhase->Initialize();
}

bool CBossErma::IsTargetInArena() const
{
    if (m_pTarget == nullptr)
        return false;

    constexpr float ARENA_LEFT = 3350.f;
    constexpr float ARENA_RIGHT = 4570.f;
    constexpr float ARENA_TOP = 1080.f;
    constexpr float ARENA_BOTTOM = 2300.f;

    const VEC& vPlayerPoint = m_pTarget->GetInfo().vPoint;
    return ARENA_LEFT <= vPlayerPoint.fX && vPlayerPoint.fX <= ARENA_RIGHT &&
        ARENA_TOP <= vPlayerPoint.fY && vPlayerPoint.fY <= ARENA_BOTTOM;
}

void CBossErma::StartMissileVolley()
{
    constexpr float ARENA_LEFT = 3350.f;
    constexpr float ARENA_RIGHT = 4570.f;
    constexpr float ARENA_TOP = 1080.f;
    constexpr float ARENA_BOTTOM = 2300.f;

    constexpr int MISSILE_COUNT = 5;
    constexpr float TARGET_MARGIN_X = 170.f;
    constexpr float LINE_MARGIN_Y = 170.f;
    constexpr double MISSILE_STAGGER = 0.08;

    const float fTargetX = (m_pTarget != nullptr)
        ? m_pTarget->GetInfo().vPoint.fX
        : (ARENA_LEFT + ARENA_RIGHT) * 0.5f;
    const float fImpactX = std::clamp(
        fTargetX,
        ARENA_LEFT + TARGET_MARGIN_X,
        ARENA_RIGHT - TARGET_MARGIN_X);
    const float fLineTop = ARENA_TOP + LINE_MARGIN_Y;
    const float fLineBottom = ARENA_BOTTOM - LINE_MARGIN_Y;
    const float fLineInterval =
        (fLineBottom - fLineTop) / static_cast<float>(MISSILE_COUNT - 1);

    for (int i = 0; i < MISSILE_COUNT; ++i)
    {
        const float fImpactY = fLineTop + fLineInterval * i;
        CErmaMissile* pMissile = static_cast<CErmaMissile*>(
            CAbstractFactory<CErmaMissile>::CreateObj());
        pMissile->Configure(
            VEC{ fImpactX, fImpactY },
            MISSILE_STAGGER * i);
        CObjMgr::GetInstance()->AddObject(OBJID::EFFECT, pMissile);
    }
}

void CBossErma::StartLaserPattern()
{
    constexpr float ARENA_TOP = 1080.f;
    constexpr float ARENA_BOTTOM = 2300.f;
    constexpr float LANE_MARGIN_Y = 260.f;

    const float fUpperLaneY = ARENA_TOP + LANE_MARGIN_Y;
    const float fLowerLaneY = ARENA_BOTTOM - LANE_MARGIN_Y;

    CErmaLaser* pDownwardLaser = static_cast<CErmaLaser*>(
        CAbstractFactory<CErmaLaser>::CreateObj());
    pDownwardLaser->Configure(fUpperLaneY, fLowerLaneY, 0., true);
    CObjMgr::GetInstance()->AddObject(OBJID::EFFECT, pDownwardLaser);

    CErmaLaser* pUpwardLaser = static_cast<CErmaLaser*>(
        CAbstractFactory<CErmaLaser>::CreateObj());
    pUpwardLaser->Configure(fLowerLaneY, fUpperLaneY, 0., false);
    CObjMgr::GetInstance()->AddObject(OBJID::EFFECT, pUpwardLaser);
}

void CBossErma::BeginEnding()
{
    if (m_bEnding)
        return;

    CUIMgr::GetInstance()->HideUI(UIID::BOSS_HP);

    if (m_pErma != nullptr)
        m_pErma->RequestRemove();
    if (m_pBody != nullptr)
        m_pBody->RequestRemove();
    if (m_pHead != nullptr)
        m_pHead->RequestRemove();
    if (m_pLeftHand != nullptr)
        m_pLeftHand->RequestRemove();
    if (m_pRightHand != nullptr)
        m_pRightHand->RequestRemove();

    m_bEnding = true;
}
