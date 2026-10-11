#include "pch.h"
#include "CLetterBox.h"
#include "CTimeMgr.h"

CLetterBox::CLetterBox()
    : m_fHeight(0.), m_fTargetHeight(0.)
{
}

CLetterBox::~CLetterBox()
{
    Release();
}

void CLetterBox::Initialize()
{
    m_eRender = RENDERID::UI;
    m_iRenderLayer = 98; // 대충 Fade 아래로 설정

    m_fHeight = 0.f;
    m_fTargetHeight = 0.f;

    m_fSpeed = 100.f;
}

int CLetterBox::Update()
{
    if (m_bDead)
        return DEAD;

    if (!m_bView)
        return NOEVENT;

    float fStep = m_fSpeed * DT; 

    // 현재 높이가 목표 높이보다 작으면 펼쳐야 함
    if (m_fHeight < m_fTargetHeight)
        m_fHeight = clamp(m_fHeight + fStep, 0.f, m_fTargetHeight);
    // 현재 높이가 목표 높이보다 크면 줄여야 함
    else if (m_fHeight > m_fTargetHeight)
        m_fHeight = max(m_fHeight - fStep, m_fTargetHeight);

    // 타겟 높이랑 현재 높이가 둘 다 0이면 m_bView를 false로 바꿈
    if (m_fTargetHeight == 0.f && m_fHeight == 0.f)
        m_bView = false;

    return NOEVENT;
}

void CLetterBox::LateUpdate()
{
    if (!m_bView)
        return;
}

void CLetterBox::Render(Graphics* pGraphics)
{
    if (!m_bView)
        return;

    SolidBrush brush(Color(255, 0, 0, 0));
    pGraphics->FillRectangle(&brush, RectF(0.f, 0.f, static_cast<float>(WINCX), m_fHeight)); // 위 그리기
    pGraphics->FillRectangle(&brush, RectF(0.f, static_cast<float>(WINCY) - m_fHeight, static_cast<float>(WINCX), m_fHeight)); // 아래 그리기
}

void CLetterBox::Release()
{
}

void CLetterBox::Show()
{
    m_bView = true;

    // 80까지 점점 커짐
    m_fTargetHeight = 80.f;
}

void CLetterBox::Hide()
{
    // 목표를 0으로 두면, 알아서 0으로 줄어들고, m_bView는 false가 됨
    m_fTargetHeight = 0.f;
}