#include "pch.h"
#include "CSpeechBubble.h"
#include "CCameraMgr.h"
#include "CFontMgr.h"
#include "CTimeMgr.h"
#include "CImgMgr.h"


CSpeechBubble::CSpeechBubble()
    : m_dElapseTime(0.), m_dAliveTime(0.), m_fWidth(0.f), m_fHeight(0.f)
{
    m_vOffset = { 0.f, 0.f };
}

CSpeechBubble::~CSpeechBubble()
{
}

void CSpeechBubble::Initialize()
{
    m_tInfo = { 0.f, 0.f, 0.f, 0.f };
    m_pFrameKey = L"SpeechBubble_Base";
    m_eRender = RENDERID::UI;
    m_iRenderLayer = 97; // 페이드, 레터 박스보다 밑에

    // 기본 세팅
    m_dElapseTime = 0.;
    m_dAliveTime = 0.;
    m_fWidth = 0.f;
    m_fHeight = 0.f;

    m_fUIScale = PIXEL_SCALE * 0.5f;
}

int CSpeechBubble::Update()
{
    if (m_bDead)
        return DEAD;

    if (!m_bView)
        return NOEVENT;

    UpdateTime();

    return NOEVENT;
}

void CSpeechBubble::LateUpdate()
{
    if (!m_bView)
        return;
}

void CSpeechBubble::Render(Graphics* pGraphics)
{
    if (!m_bView || !m_pTarget)
        return;

    Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
    if (nullptr == pImg)
        return;
    
    // 중앙 위치
    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
    VEC vPoint = m_pTarget->GetInfo().vPoint;
    VEC vRectSize = VEC{ m_fWidth, m_fHeight } * m_fUIScale; // 화면에 보이는 사이즈

    // 박스 좌상단 위치
    VEC vRectLT = VEC{ vPoint.fX - vRectSize.fX * 0.5f, vPoint.fY - vRectSize.fY} + vScroll + m_vOffset;

    // ToolTip처럼 3-Slicing 진행
    VEC vCellSize{};
    VEC vImgSize{};
    RectF DestRect{};
    VEC vOffset{};   // 왼쪽 상단 기준 Offset

    float fCellLeftSize = 2.f;
    float fCellMiddleSize = 156.f;
    float fCellRightSize = 2.f;

    float fCellLeftEnd = 2.f;
    float fCellMiddleEnd = 158.f;

    // 가운데 길이 구하기
    float fMiddleSize = vRectSize.fX - (fCellLeftSize + fCellRightSize) * m_fUIScale;

    ///// 배경 그리기

    // 왼쪽
    vCellSize = { fCellLeftSize, 24.f };
    vImgSize = VEC{ fCellLeftSize * m_fUIScale, vRectSize.fY };
    vOffset = VEC{ 0.f, 0.f };
    DestRect = { vRectLT.fX + vOffset.fX, vRectLT.fY + vOffset.fY, vImgSize.fX, vImgSize.fY };
    pGraphics->DrawImage(
        pImg, DestRect,
        0.f, 0.f, vCellSize.fX, vCellSize.fY, 
        UnitPixel
    );

    // 가운데
    vCellSize = { fCellMiddleSize, 24.f };
    vImgSize = VEC{ fMiddleSize, vRectSize.fY };
    vOffset = VEC{ fCellLeftEnd, 0.f } *m_fUIScale;
    DestRect = { vRectLT.fX + vOffset.fX, vRectLT.fY + vOffset.fY, vImgSize.fX, vImgSize.fY };
    pGraphics->DrawImage(
        pImg, DestRect,
        fCellLeftEnd, 0.f, vCellSize.fX, vCellSize.fY, 
        UnitPixel
    );

    // 오른쪽
    vCellSize = { fCellRightSize, 24.f };
    vImgSize = VEC{ fCellRightSize * m_fUIScale, vRectSize.fY};
    vOffset = VEC{ fCellLeftEnd * m_fUIScale + fMiddleSize, 0.f };
    DestRect = { vRectLT.fX + vOffset.fX, vRectLT.fY + vOffset.fY, vImgSize.fX, vImgSize.fY };
    pGraphics->DrawImage(
        pImg, DestRect,
        fCellMiddleEnd, 0.f, vCellSize.fX, vCellSize.fY, 
        UnitPixel
    );

    ///// 텍스트 그리기

    // 패딩, x축 4, y축 1
    VEC vPadding = VEC{ 4.f, 2.f } * m_fUIScale;
    // 글씨를 그릴 수 있는 영역
    VEC vUsableSize = { vRectSize.fX - vPadding.fX * 2.f, vRectSize.fY - vPadding.fY };

    // 이름 : 대사 = 3 : 7;

    // 이름 텍스트 그리기
    vOffset = vPadding;
    vRectSize = { vUsableSize.fX, vUsableSize.fY * 0.4f };
    DestRect = { vRectLT.fX + vOffset.fX, vRectLT.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };

#ifdef _DEBUG
    SolidBrush nameBrush(Color{ 255, 0, 128, 0 });
    pGraphics->FillRectangle(&nameBrush, DestRect);
#endif
    CFontMgr::GetInstance()->DrawString(pGraphics, m_strName, FONT_TYPE::NORMAL, DestRect, Color{ 255, 243, 218, 175 }, 16.f, StringAlignmentNear, StringAlignmentNear);

    // 대사 그리기 
    vOffset = VEC{ vPadding.fX, vPadding.fY + vRectSize.fY };
    vRectSize = { vUsableSize.fX, vUsableSize.fY * 0.6f };
    DestRect = { vRectLT.fX + vOffset.fX, vRectLT.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
    SolidBrush textBrush(Color{ 255, 128, 0, 0 });
    pGraphics->FillRectangle(&textBrush, DestRect);
#endif
    CFontMgr::GetInstance()->DrawString(pGraphics, m_strText, FONT_TYPE::NORMAL, DestRect, Color{ 255, 255, 255, 255 }, 22.f, StringAlignmentNear, StringAlignmentFar);
}

void CSpeechBubble::Release()
{
    Hide();
}

void CSpeechBubble::UpdateTime()
{
    m_dElapseTime += DT; 
    if (m_dElapseTime >= m_dAliveTime)
        Hide();
}

void CSpeechBubble::SetText(CObj* pTarget, wstring strName, wstring strText, double dAliveTime, float fWidth, float fHeight)
{
    // 타겟 - 말하는 Obj 없으면 리턴
    if (nullptr == pTarget)
        return;

    m_pTarget = pTarget;
    m_strName = strName;
    m_strText = strText;
    m_dElapseTime = 0.;
    m_dAliveTime = dAliveTime;
    m_fWidth = fWidth;
    m_fHeight = fHeight;
}

void CSpeechBubble::Hide()
{
    // 숨기고
    m_bView = false;

    // 값을 초기화해주기
    m_pTarget = nullptr;
    m_strName.clear();
    m_strText.clear();

    m_dElapseTime = 0.;
    m_dAliveTime = 0.;
    m_fWidth = 0.f;
    m_fHeight = 0.f;
}