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
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Stage/LibLoading.png",L"LibLoading");
    Init_BGM(L"LibLoading.wav", 1.f);
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
        CSceneMgr::GetInstance()->RequestChange(SCENEID::STAGE1);
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

    wstring wstr = L"도서관";
    wstring wstrDescription1 = L"당신은 초목지대의 거대한 유적에 들어갑니다. 도서관이라 불리는 이곳은 동물들이 탑에 정착하기 전부터 존재하던 곳입니다.";
    wstring wstrDescription2 = L"비록 오랜 시간이 지났지만, 유적의 경비는 삼엄합니다.";

    RectF rcTitleRect{0, 430, WINCX, 80};
    RectF rcDescriptionRect1{ 300, 530, 700, 100 };
    RectF rcDescriptionRect2{ 300, 630, 700, 100 };

    CFontMgr::GetInstance()->DrawString(pGraphics, wstr, FONT_TYPE::NORMAL, rcTitleRect, Color{255, 126, 192, 192}, 48.f);
    CFontMgr::GetInstance()->DrawString(pGraphics, wstrDescription1, FONT_TYPE::NORMAL, rcDescriptionRect1, Color{255, 185, 187, 183}, 22.f, StringAlignmentNear, StringAlignmentNear);
    CFontMgr::GetInstance()->DrawString(pGraphics, wstrDescription2, FONT_TYPE::NORMAL, rcDescriptionRect2, Color{255, 185, 187, 183}, 22.f, StringAlignmentNear, StringAlignmentNear);
}

void CLibLoading::Release()
{
}

void CLibLoading::Init_CreateObj()
{
}
