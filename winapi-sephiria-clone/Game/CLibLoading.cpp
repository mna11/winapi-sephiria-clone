#include "pch.h"
#include "CLibLoading.h"

#include "CEffectMgr.h"
#include "CFontMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"
#include "CUIMgr.h"
#include "CSceneMgr.h"

CLibLoading::CLibLoading()
{
}

CLibLoading::~CLibLoading()
{
	Release();
}

void CLibLoading::Initialize()
{
	CUIMgr::GetInstance()->HideUI(UIID::BASIC_INFO);
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Stage/LibLoading.png",L"LibLoading");
}

void CLibLoading::Update()
{
	m_dFrameTime += DT;
    m_dNextStageTime -= DT;


	if (m_dFrameTime >= 0.08)
	{
		m_iFrame = (m_iFrame + 1) % 16;
		m_dFrameTime -= 0.08;
	}
}

void CLibLoading::LateUpdate()
{
    if (m_dNextStageTime <= 0)
        CSceneMgr::GetInstance()->RequestChange(SCENEID::BOSS_STAGE);
}

void CLibLoading::Render(Graphics* pGraphics)
{
    pGraphics->Clear(Color(255, 0, 0, 0));
    Image* pImg = CImgMgr::GetInstance()->FindImg(L"LibLoading");

    if (nullptr == pImg)
        return;

    VEC vCellSize = {320, 780};
    VEC vDrawSize = vCellSize * PIXEL_SCALE;
    RectF rcDest(0.f, 0.f, WINCX, vDrawSize.fY);

    pGraphics->DrawImage(
        pImg,
        rcDest,
        vCellSize.fX * m_iFrame,
        0.f,
        vCellSize.fX,
        vCellSize.fY,
        UnitPixel
    );

    wstring wstr = L"Library";
    wstring wstrDescription = L"뭐시기 저시기 도서관 설명";

    RectF rcTitleRect{0, 400, WINCX, 80};
    RectF rcDescriptionRect{ 0, 500, WINCX, 100 };
    CFontMgr::GetInstance()->DrawString(pGraphics, wstr, FONT_TYPE::NORMAL, rcTitleRect, Color{255, 126, 192, 192}, 32.f);

    CFontMgr::GetInstance()->DrawString(pGraphics, wstrDescription, FONT_TYPE::NORMAL, rcDescriptionRect, Color{255, 185, 187, 183}, 32.f);
}

void CLibLoading::Release()
{
}

void CLibLoading::Init_CreateObj()
{
}
