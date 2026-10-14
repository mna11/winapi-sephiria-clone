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
#include "CImgMgr.h"
#include "CSoundMgr.h"

#include "CPotionData.h"

CFishingUI::CFishingUI()
    : CState(FISHING_STATE::END, FISHING_STATE::READY),
    m_pPlayer(nullptr), m_pRodUI(nullptr), m_pPlayingUI(nullptr),
    m_fCatchProb(0.), m_dCatchElapseTime(0.), m_dCatchInterval(0.),
    m_dNotColElapseTime(0.), m_dNotColJudTime(0.),
    m_dCaughtElapseTime(0.), m_dCaughtMaxTime(0.),
    m_iPotionID(-1), m_fPotionSpeed(0.), m_fPotionAngle(0.)
{
    ZeroMemory(&m_vPotionPoint, sizeof(VEC));
}

CFishingUI::~CFishingUI()
{
    Release();
}

void CFishingUI::Initialize()
{
    m_eRender = RENDERID::UI;
    m_iRenderLayer = 0;

    InitializeData();

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

    // 낚은 포션 연출 그리기
    const POTION_INFO* pPotionInfo = CPotionData::GetInstance()->FindPotionInfo(m_iPotionID);
    if (nullptr == pPotionInfo)
        return;

    Image* pImg = CImgMgr::GetInstance()->FindImg(pPotionInfo->strImg.c_str());
    if (nullptr == pImg)
        return;

    VEC vCellSize = { static_cast<float>(pImg->GetWidth()), static_cast<float>(pImg->GetHeight()) };
    VEC vImgSize = vCellSize * m_fUIScale * 1.5f;
    RectF DestRect = { m_vPotionPoint.fX - vImgSize.fX * 0.5f, m_vPotionPoint.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };

    Matrix matRot{};
    matRot.RotateAt(m_fPotionAngle, { m_vPotionPoint.fX, m_vPotionPoint.fY });
    pGraphics->SetTransform(&matRot);
    pGraphics->DrawImage(
        pImg, DestRect,
        0, 0, vCellSize.fX, vCellSize.fY,
        UnitPixel
    );
    pGraphics->ResetTransform();
}

void CFishingUI::Release()
{
    SafeDelete(m_pRodUI);
    SafeDelete(m_pPlayingUI);
}

void CFishingUI::Show()
{
    if (m_bView)
        return;

    CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
    if (nullptr == pPlayer)
        return;

    m_bView = true;
    m_pPlayer = pPlayer;
    
    m_eCurState = FISHING_STATE::END;
    m_eNextState = FISHING_STATE::READY;

    if (m_pRodUI != nullptr)
        m_pRodUI->Show();
}

void CFishingUI::Hide()
{
    if (!m_bView)
        return;

    auto pKeyGuide = static_cast<CKeyGuide*>(CUIMgr::GetInstance()->GetUI(UIID::KEY_GUIDE));
    pKeyGuide->HideGuide(m_pPlayer);
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
            RequestChange(FISHING_STATE::BAIT);
        break;
    case FISHING_STATE::BAIT:
        m_dCatchElapseTime += DT;
        if (m_dCatchElapseTime >= m_dCatchInterval)
        {
            m_dCatchElapseTime -= m_dCatchInterval;
            bool bCatch = (uniform_real_distribution<float>(0.f, 1.f)(g_engine) < m_fCatchProb);
            if (bCatch)
            {
                RequestChange(FISHING_STATE::PLAYING);
                VEC vOffset = VEC{ -15.f, -15.f } *m_fUIScale;
                CEffectMgr::GetInstance()->CreateEffect(L"Fishing_Hit", m_pPlayer->GetInfo().vPoint + vOffset, 0, 0.f, 0.05f);
            }
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
    {
        // 낚은 포션 감속 로직 - 위로 좀 뛰어올랐다가 내려가게
        float fAccel = 600.f;
        float fRotationSpeed = 60.f * DT;
        m_fPotionSpeed += fAccel * DT;
        m_fPotionAngle = fmodf(m_fPotionAngle + fRotationSpeed, 360.f);
        m_vPotionPoint.fY += m_fPotionSpeed * DT;
        
        // Caught 끝나고 다시 Ready 상태로 돌아가게 만들기
        m_dCaughtElapseTime += DT;
        if (m_dCaughtElapseTime > m_dCaughtMaxTime)
            RequestChange(FISHING_STATE::READY);
        break;
    }
    case FISHING_STATE::MISS:
        break;
    default:
        break;
    }
}


void CFishingUI::ApplyChange()
{
    if (nullptr == m_pRodUI || nullptr == m_pPlayingUI || nullptr == m_pPlayer)
    {
        Hide();
        return;
    }

    if (m_eCurState != m_eNextState)
    {
        switch (m_eNextState)
        {
        case FISHING_STATE::READY:
        {
            InitializeData(); // 데이터를 초기화해준다.
            m_pRodUI->RequestChange(FISHING_ROD_STATE::IDLE_OUT_WATER); // 낚시대 밖

            auto pKeyGuide = static_cast<CKeyGuide*>(CUIMgr::GetInstance()->GetUI(UIID::KEY_GUIDE));
            pKeyGuide->ShowGuide(m_pPlayer);
            pKeyGuide->SetFollow(true);
            pKeyGuide->SetKeyGuideInfo(VK_LBUTTON, L"낚시", { 0.f, -m_pPlayer->GetInfo().vSize.fY * 1.5f }, { 28.f, 10.f });
            break;
        }
        case FISHING_STATE::BAIT:
        {
            m_pRodUI->RequestChange(FISHING_ROD_STATE::PREPARE_CAST); // 낚시대 던지기 -> 낚시대 물 안
            auto pKeyGuide = static_cast<CKeyGuide*>(CUIMgr::GetInstance()->GetUI(UIID::KEY_GUIDE));
            pKeyGuide->HideGuide(m_pPlayer);
            break;
        }
        case FISHING_STATE::PLAYING: 
            m_pPlayingUI->Show(); // 플레이 UI 보이게 해주고
            m_pRodUI->RequestChange(FISHING_ROD_STATE::IDLE_IN_WATER); // 낚시대 물 안 or 낚시대 Reeling
            break;
        case FISHING_STATE::CAUGHT:
            m_pPlayingUI->Hide(); // 플레이 UI 안보이게 
            m_pRodUI->RequestChange(FISHING_ROD_STATE::CAUGHT); // 낚시대 잡은 모션 -> 낚시대 밖
            m_iPotionID = uniform_int_distribution<int>(0, 2)(g_engine);                // 낚은 포션 설정하기
            m_pPlayer->GetInventory()->InsertItem(0, m_iPotionID, ITEM_TYPE::POTION);   // 인벤토리에 넣어주기

            CSoundMgr::GetInstance()->PlaySound(L"FishPull.wav", CHANNEL_GROUPID::SFX, 1.f);
            CSoundMgr::GetInstance()->PlaySound(L"FishGet.wav", CHANNEL_GROUPID::SFX, 1.f);
            break;
        case FISHING_STATE::MISS:
            // 하긴 해야되는데 일단 패스
            break;
        case FISHING_STATE::END:
            break;
        }

        m_eCurState = m_eNextState;
    }
}

void CFishingUI::InitializeData()
{
    m_eCurState = FISHING_STATE::END;
    m_eNextState = FISHING_STATE::READY;

    m_fCatchProb = 0.3f;        // 30퍼 확률
    m_dCatchElapseTime = 0.f;
    m_dCatchInterval = 1.f;     // 1초마다 한번씩 검사

    m_dNotColElapseTime = 0.f; 
    m_dNotColJudTime = 0.2f;    // 충돌 0.2초 이상 없을 시 Reeling 안하게


    m_dCaughtElapseTime = 0.f;
    m_dCaughtMaxTime = 3.f;      // 잡은 뒤 3초 뒤 READY 상태 돌아감

    m_fUIScale = PIXEL_SCALE;   // 스케일

    m_iPotionID = -1;                           // 포션 ID 초기화
    m_fPotionSpeed = -600.f;                    // 낚은거 속도
    m_vPotionPoint = { WINCX * 0.25f, WINCY };  // 낚은거 등장할 위치 
    m_fPotionAngle = 0.f;                       // 포션 각도

    // 만약 구성요소 UI들 없을 시, 생성
    if (m_pRodUI == nullptr)
        m_pRodUI = static_cast<CFishingRodUI*>(CAbstractFactory<CFishingRodUI>::CreateUI(m_pMouse));

    if (m_pPlayingUI == nullptr)
        m_pPlayingUI = static_cast<CFishingPlayingUI*>(CAbstractFactory<CFishingPlayingUI>::CreateUI(m_pMouse));
}
