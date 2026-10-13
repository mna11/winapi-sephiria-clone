#include "pch.h"
#include "CFishingUI.h"
#include "CFishingRodUI.h"
#include "CFishingPlayingUI.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CEffectMgr.h"

CFishingUI::CFishingUI()
    : CState(FISHING_STATE::END, FISHING_STATE::READY), m_pPlayer(nullptr), m_pRodUI(nullptr), m_pPlayingUI(nullptr)
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


    if (m_pPlayingUI != nullptr)
        m_pPlayingUI->Show();
}

void CFishingUI::Hide()
{
    m_bView = false;
    m_pPlayer = nullptr;

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

void CFishingUI::ApplyChange()
{
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
            if (m_pPlayingUI != nullptr)
                m_pPlayingUI->Show();
            break;
        case FISHING_STATE::CAUGHT:
            break;
        case FISHING_STATE::MISS:
            break;
        case FISHING_STATE::END:
            break;
        }


        m_eCurState = m_eNextState;
    }
}
