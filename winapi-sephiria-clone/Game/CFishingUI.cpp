#include "pch.h"
#include "CFishingUI.h"
#include "CFishingRodUI.h"

#include "CAbstractFactory.h"

CFishingUI::CFishingUI()
    : m_pRodUI(nullptr)
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

    Hide();
}

int CFishingUI::Update()
{
    if (m_bDead)
        return DEAD;

    if (!m_bView)
        return NOEVENT;

    if (m_pRodUI != nullptr)
        m_pRodUI->Update();

    return NOEVENT;
}

void CFishingUI::LateUpdate()
{
    if (!m_bView)
        return;

    if (m_pRodUI != nullptr)
        m_pRodUI->LateUpdate();
}

void CFishingUI::Render(Graphics* pGraphics)
{
    if (!m_bView)
        return;

    if (m_pRodUI != nullptr)
        m_pRodUI->Render(pGraphics);
}

void CFishingUI::Release()
{
    SafeDelete(m_pRodUI);
}

void CFishingUI::Show()
{
    m_bView = true;

    if (m_pRodUI != nullptr)
        m_pRodUI->Show();
}

void CFishingUI::Hide()
{
    m_bView = false;

    if (m_pRodUI != nullptr)
        m_pRodUI->Hide();
}

void CFishingUI::Toggle()
{
    if (m_bView)
        Hide();
    else
        Show();
}