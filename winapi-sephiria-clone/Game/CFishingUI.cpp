#include "pch.h"
#include "CFishingUI.h"
#include "CFishingRodUI.h"
#include "CFishingPlayingUI.h"

#include "CKeyGuide.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CEffectMgr.h"
#include "CTimeMgr.h"
#include "CUIMgr.h"
#include "CKeyMgr.h"

CFishingUI::CFishingUI()
    : CState(FISHING_STATE::END, FISHING_STATE::READY),
    m_pPlayer(nullptr), m_pRodUI(nullptr), m_pPlayingUI(nullptr),
    m_fCatchProb(0.), m_dCatchElapseTime(0.), m_dCatchInterval(0.),
    m_dNotColElapseTime(0.), m_dNotColJudTime(0.)
{
}

CFishingUI::~CFishingUI()
{
    Release();
}

void CFishingUI::Initialize()
{
    m_eRender = RENDERID::UI;
    m_iRenderLayer = 0;

    m_fCatchProb = 0.3f;        // 30퍼 확률
    m_dCatchElapseTime = 0.f;   
    m_dCatchInterval = 1.f;     // 1초마다 한번씩 검사

    m_dNotColElapseTime = 0.f;
    m_dNotColJudTime = 0.2f;

    if (m_pRodUI == nullptr)
    {
        m_pRodUI = static_cast<CFishingRodUI*>(CAbstractFactory<CFishingRodUI>::CreateUI(m_pMouse));
        m_pRodUI->Initialize();
    }

    if (m_pPlayingUI == nullptr)
    {
        m_pPlayingUI = static_cast<CFishingPlayingUI*>(CAbstractFactory<CFishingPlayingUI>::CreateUI(m_pMouse));
        m_pPlayingUI->Initialize();
    }

    Hide();
}

int CFishingUI::Update()
{
    if (m_bDead)
        return DEAD;

    if (!m_bView)
        return NOEVENT;

    ApplyChange();
    UpdateTime();

    if (m_pRodUI != nullptr)
        m_pRodUI->Update();

    if (m_pPlayingUI != nullptr)
        m_pPlayingUI->Update();

    return NOEVENT;
}

void CFishingUI::LateUpdate()
{
    if (!m_bView)
        return;

    if (m_pRodUI != nullptr)
        m_pRodUI->LateUpdate();

    if (m_pPlayingUI != nullptr)
        m_pPlayingUI->LateUpdate();
}

void CFishingUI::Render(Graphics* pGraphics)
{
    if (!m_bView)
        return;

    if (m_pRodUI != nullptr)
        m_pRodUI->Render(pGraphics);

    if (m_pPlayingUI != nullptr)
        m_pPlayingUI->Render(pGraphics);
}

void CFishingUI::Release()
{
    SafeDelete(m_pRodUI);
    SafeDelete(m_pPlayingUI);
}

void CFishingUI::Show()
{
    CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
    if (nullptr == pPlayer)
        return;

    m_bView = true;
    m_pPlayer = pPlayer;

    if (m_pRodUI != nullptr)
        m_pRodUI->Show();
}

void CFishingUI::Hide()
{
    m_bView = false;
    m_pPlayer = nullptr;
    
    m_eCurState = m_eNextState = FISHING_STATE::READY;
    m_dCatchElapseTime = 0.;
    m_dNotColElapseTime = 0.;

    if (m_pRodUI != nullptr)
        m_pRodUI->Hide();

    if (m_pPlayingUI != nullptr)
        m_pPlayingUI->Hide();
}

void CFishingUI::Toggle()
{
    if (m_bView)
        Hide();
    else
        Show();
}

void CFishingUI::UpdateTime()
{
    if (nullptr == m_pRodUI || nullptr == m_pPlayingUI || nullptr == m_pPlayer)
    {
        Hide();
        return;
    }

    switch (m_eCurState)
    {
    case FISHING_STATE::READY:
        if (KEY_DOWN(VK_LBUTTON))
        {
            auto pKeyGuide = static_cast<CKeyGuide*>(CUIMgr::GetInstance()->GetUI(UIID::KEY_GUIDE));
            pKeyGuide->HideGuide(m_pPlayer);
            RequestChange(FISHING_STATE::BAIT);
        }
        break;
    case FISHING_STATE::BAIT:
        m_dCatchElapseTime += DT;
        if (m_dCatchElapseTime >= m_dCatchInterval)
        {
            m_dCatchElapseTime -= m_dCatchInterval;
            bool bCatch = (uniform_real_distribution<float>(0.f, 1.f)(g_engine) < m_fCatchProb);
            if (bCatch)
                RequestChange(FISHING_STATE::PLAYING);
        }
        break;
    case FISHING_STATE::PLAYING:
    {
        if (m_pPlayingUI->GetIsCol())
        {
            m_dNotColElapseTime = 0.;
            m_pRodUI->RequestChange(FISHING_ROD_STATE::REELING);
        }
        else
        {
            m_dNotColElapseTime += DT;
            if (m_dNotColElapseTime >= m_dNotColJudTime)
            {
                m_dNotColElapseTime -= DT;
                m_pRodUI->RequestChange(FISHING_ROD_STATE::IDLE_IN_WATER);
            }

        }

        if (m_pPlayingUI->GetGauge() >= 100.f)
        {
            RequestChange(FISHING_STATE::CAUGHT);
        }
        break;
    }
    case FISHING_STATE::CAUGHT:
        break;
    case FISHING_STATE::MISS:
        break;
    default:
        break;
    }
}


void CFishingUI::ApplyChange()
{
    if (nullptr == m_pRodUI || nullptr == m_pPlayingUI)
    {
        Hide();
        return;
    }

    if (m_eCurState != m_eNextState)
    {
        switch (m_eNextState)
        {
        case FISHING_STATE::READY:
            m_pRodUI->RequestChange(FISHING_ROD_STATE::IDLE_OUT_WATER);
            break;
        case FISHING_STATE::BAIT:
            m_pRodUI->RequestChange(FISHING_ROD_STATE::PREPARE_CAST);
            break;
        case FISHING_STATE::PLAYING:
            m_pPlayingUI->Show();
            m_pRodUI->RequestChange(FISHING_ROD_STATE::IDLE_IN_WATER);
            break;
        case FISHING_STATE::CAUGHT:
            m_pPlayingUI->Hide();
            m_pRodUI->RequestChange(FISHING_ROD_STATE::CAUGHT);
            break;
        case FISHING_STATE::MISS:
            break;
        case FISHING_STATE::END:
            break;
        }


        m_eCurState = m_eNextState;
    }
}
