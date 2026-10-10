#include "pch.h"
#include "CFade.h"

#include "CTimeMgr.h"
#include "CImgMgr.h"

CFade::CFade()
	: m_eFadeType(FADE_TYPE::END), m_iAlpha(255), m_dFadeDuration(1.5), m_dFadeElapsed(0.)
{
}

CFade::~CFade()
{
	Release();
}

void CFade::Initialize()
{
	m_tInfo = { 0.f, 0.f, 0.f, 0.f };

	// 모든 UI보다 위
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 99;
	m_pFrameKey = L"Fade_Gif";
}

int CFade::Update()
{
	if (!m_bView || m_eFadeType == FADE_TYPE::END)
		return NOEVENT;

	m_dFadeElapsed += DT;
	float fRatio = m_dFadeElapsed / m_dFadeDuration;
	m_iAlpha = static_cast<int>(255. * (1. - fRatio));
	if (fRatio >= 1.)
		m_bView = false;

	UpdateFrame();
	return NOEVENT;
}

void CFade::LateUpdate()
{
	if (!m_bView || m_eFadeType == FADE_TYPE::END)
		return;
}

// 원래 Shop이나 Forge처럼 png 아틀라스 이미지로 했었는데, 이게 전체 화면을 채우는거다보니깐
// 렌더 엔진으로는 좀 부담이 되는지, 프레임 드랍이 너무 심했었음
// gif로 바꾸니깐, 슬라이싱 작업이 사라져서 그런지 좀 가벼워져서 이걸로 채택함
// 나중에 Shop이랑 Forge도 한번 바꿔볼 생각
void CFade::Render(Graphics* pGraphics)
{
	if (!m_bView || m_eFadeType == FADE_TYPE::END)
		return;

	Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

	if (m_eFadeType == FADE_TYPE::CIRCLE)
	{
		// 현재 그릴 프레임 정하기 - 원래는 이전에 GetFrameCount로 프레임 몇개 있는지 확인해야되는데, 이미 60개 있다는거 알아서 패스함 
		pImg->SelectActiveFrame(&FrameDimensionTime, m_tFrame.iStart);
		pGraphics->DrawImage(pImg, 0, 0, WINCX, WINCY);
	}
	else
	{
		SolidBrush brush(Color(m_iAlpha, 0, 0, 0));
		pGraphics->FillRectangle(&brush, 0, 0, WINCX, WINCY);
	}
}

void CFade::Release()
{
}

void CFade::Show()
{
	m_bView = true;
	
	m_dFadeElapsed = 0.;
	m_iAlpha = 255;
	SetFrame(0, 59, 0, 0.001);
}

// 이펙트처럼 마지막에 도달하면 m_bView를 끄게 만들었다.
// 다른 UI처럼 Hide 호출해줄 필요 없음
void CFade::UpdateFrame()
{
	m_tFrame.dFrameElapsedTime += DT;
	if (m_tFrame.dFrameSpeed <= m_tFrame.dFrameElapsedTime)
	{
		++m_tFrame.iStart;
		m_tFrame.dFrameElapsedTime -= m_tFrame.dFrameSpeed;

		if (m_tFrame.iStart > m_tFrame.iEnd)
		{
			if (m_eFadeType == FADE_TYPE::CIRCLE)
				m_bView = false;
		}
	}
}