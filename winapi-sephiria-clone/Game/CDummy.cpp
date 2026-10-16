#include "pch.h"
#include "CDummy.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"
#include "CTimeMgr.h"
#include "CEffectMgr.h"

CDummy::CDummy()
{
}

CDummy::~CDummy()
{
    Release();
}

void CDummy::Initialize()
{
    // 기초 정보 초기화
    m_tInfo = { 0.f, 0.f, 50.f, 50.f };
    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 4;
    m_fSpeed = 0.f;

    CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Dummy/Dummy.png", L"Dummy");

    m_pFrameKey = L"Dummy";

    // 스탯 초기화
    m_tStat = {0, 0, 0 };

    m_iLeaf = 0;
    m_iExp = 0;
}

int CDummy::Update()
{
    if (m_bDead)
        return DEAD;

    m_vPrePoint = m_tInfo.vPoint;

    UpdateTime();

    __super::UpdateRect();
    return NOEVENT;
}

void CDummy::LateUpdate()
{
}

void CDummy::Render(Graphics* pGraphics)
{
    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

#ifdef _DEBUG
    // 피격 충돌 박스 렌더링
    SolidBrush WhiteBrush(Color(255, 255, 255, 255));

    pGraphics->FillRectangle(
        &WhiteBrush,
        (int)(m_tRect.left + vScroll.fX),
        (int)(m_tRect.top + vScroll.fY),
        (int)m_tInfo.vSize.fX,
        (int)m_tInfo.vSize.fY
    );
#endif

    Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
    if (nullptr == pImg)
        return;

    VEC vCellSize{ 9.f, 14.f };
    VEC vImgSize = vCellSize * PIXEL_SCALE;

    float fOffset(0.f);

    if (m_bHit && m_dIframeTime > 0.)
    {
        // 살짝 위로 길어졌다가 돌아오기
        float fRatio = clamp(static_cast<float>(m_dHitElapseTime / m_dIframeTime), 0.f, 1.f);
        float fAmplitude = vCellSize.fY * 0.5f * PIXEL_SCALE;
        float fDecay = (1.f - fRatio) * (1.f - fRatio);

        fOffset = -fAmplitude * sinf(fRatio * PI) * fDecay;
    }

    RectF rcDest{
        m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX - fOffset * 0.5f,
        m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY + fOffset,
        vImgSize.fX + fOffset,
        vImgSize.fY - fOffset
    };

    ImageAttributes* pImgAttr = m_bHit ? &m_imgAttrHit : nullptr;
    pGraphics->DrawImage(
        pImg, rcDest,
        vCellSize.fX * m_tFrame.iStart,
        vCellSize.fY * m_tFrame.iMotion,
        vCellSize.fX,
        vCellSize.fY,
        UnitPixel,
        pImgAttr
    );
}

void CDummy::Release()
{
}

void CDummy::UpdateTime()
{
    // 피해 유효 근거용 시간
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