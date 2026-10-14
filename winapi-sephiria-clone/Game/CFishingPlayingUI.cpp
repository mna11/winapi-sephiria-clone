#include "pch.h"
#include "CFishingPlayingUI.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CCollisionMgr.h"
#include "CImgMgr.h"

CFishingPlayingUI::CFishingPlayingUI()
	: m_fCatchGauge(0.f), m_fFishSpeed(0.f), m_fNetSpeed(0.f),
	m_dFishChangeMoveElapseTime(0.), m_dFishChangeMoveInterval(0.), m_bFishChangeMove(true),
	m_bIsCol(false)
{
	ZeroMemory(&m_rcFish, sizeof(RECT));
	ZeroMemory(&m_rcNet, sizeof(RECT));
}

CFishingPlayingUI::~CFishingPlayingUI()
{
	Release();
}

void CFishingPlayingUI::Initialize()
{
	// 플레이어의 위치는 연출씬이 아니라면 화면 중앙이므로
	m_tInfo = { WINCX >> 1, WINCY >> 1, 0.f, 0.f};
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 1;
	m_fUIScale = PIXEL_SCALE * 0.7f;
	m_fCatchGauge = 0.f;

	m_bView = false;

	m_dFishChangeMoveElapseTime = 0.;
	m_dFishChangeMoveInterval = 1.;		// 1초마다 방향 바꾼다.


	// Panel에 사용할 imgAttr 초기화 하기 
	ColorMatrix colorAlphaMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.7f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	m_imgAttrAlpha.SetColorMatrix(&colorAlphaMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
}

int CFishingPlayingUI::Update()
{
	if (m_bDead)
		return DEAD;

	if (!m_bView)
		return NOEVENT;

	UpdateTime();
	Move();
	UpdateRect();

	return NOEVENT;
}

void CFishingPlayingUI::LateUpdate()
{
	if (!m_bView)
		return;

	HandleCollision();
}

void CFishingPlayingUI::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	// 패널 그리기
	Image* pImg = CImgMgr::GetInstance()->FindImg(L"Fishing_Playing_Panel");
	if (nullptr == pImg)
		return;
	VEC vCellSize = { static_cast<float>(pImg->GetWidth()), static_cast<float>(pImg->GetHeight()) };
	VEC vImgSize = vCellSize * m_fUIScale;
	VEC vOffset = { vImgSize.fX, 0.f };
	VEC vCenter = m_tInfo.vPoint + vOffset;
	RectF DestRect{ vCenter.fX - vImgSize.fX * 0.5f, vCenter.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };

	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY,
		UnitPixel, &m_imgAttrAlpha
	);

	// 베이스 그리기
	pImg = CImgMgr::GetInstance()->FindImg(L"Fishing_Playing_Base");
	if (nullptr == pImg)
		return;
	vCellSize = { static_cast<float>(pImg->GetWidth()), static_cast<float>(pImg->GetHeight()) };
	vImgSize = vCellSize * m_fUIScale;
	DestRect = { vCenter.fX - vImgSize.fX * 0.5f, vCenter.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY,
		UnitPixel
	);

	// 게이지 그리기
	pImg = CImgMgr::GetInstance()->FindImg(L"Fishing_Playing_Progress_Fill");
	if (nullptr == pImg)
		return;
	vCellSize = { static_cast<float>(pImg->GetWidth()), static_cast<float>(pImg->GetHeight()) };
	VEC vGaugeSize = VEC{ 4.3f, 145.f } * m_fUIScale;
	VEC vBottom = vCenter + VEC{ 15.f, 71.5f } * m_fUIScale;
	vImgSize = { vGaugeSize.fX, clamp(vGaugeSize.fY * m_fCatchGauge / 100.f, 0.f, vGaugeSize.fY) };
	DestRect = { round(vBottom.fX - vImgSize.fX * 0.5f), vBottom.fY - vImgSize.fY, vImgSize.fX, vImgSize.fY };
	
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY,
		UnitPixel
	);

	vBottom.fX -= 12.5f * m_fUIScale;

	// 네트 그리기
	pImg = CImgMgr::GetInstance()->FindImg(L"Fishing_Playing_Net");
	if (nullptr == pImg)
		return;

	vCellSize = { static_cast<float>(pImg->GetWidth()), static_cast<float>(pImg->GetHeight()) };

	// 3 - slicing
	float fCellTopSize = 2.f;
	float fCellMiddleSize = 5.f;
	float fCellBottomSize = 3.f;

	float fCellTopEnd = 2.f;
	float fCellMiddleEnd = 7.f;

	// 전체 렉트
	DestRect = { static_cast<float>(m_rcNet.left) + vBottom.fX,
				static_cast<float>(m_rcNet.top) + vBottom.fY,
				static_cast<float>(m_rcNet.right - m_rcNet.left),
				static_cast<float>(m_rcNet.bottom - m_rcNet.top)};

	// 높이 조정
	RectF TopRect = DestRect;
	TopRect.Height = fCellTopSize * m_fUIScale;
	RectF BottomRect = DestRect;
	BottomRect.Height = fCellBottomSize * m_fUIScale;
	RectF MiddleRect = DestRect;
	MiddleRect.Height = DestRect.Height - TopRect.Height - BottomRect.Height;

	// 위치 조정
	MiddleRect.Y = TopRect.Y + TopRect.Height;
	BottomRect.Y = MiddleRect.Y + MiddleRect.Height;

	// 그리기
#ifdef _DEBUG
	SolidBrush BlackBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&BlackBrush, DestRect);
#endif

	pGraphics->DrawImage(
		pImg, TopRect,
		0, 0, vCellSize.fX, fCellTopSize,
		UnitPixel
	);
	pGraphics->DrawImage(
		pImg, MiddleRect,
		0, fCellTopEnd, vCellSize.fX, fCellMiddleSize,
		UnitPixel
	);
	pGraphics->DrawImage(
		pImg, BottomRect,
		0, fCellMiddleEnd, vCellSize.fX, fCellBottomSize,
		UnitPixel
	);


	// 물고기 그리기
	pImg = CImgMgr::GetInstance()->FindImg(L"Fishing_Playing_Fish");
	if (nullptr == pImg)
		return;
	DestRect = { static_cast<float>(m_rcFish.left) + vBottom.fX,
				static_cast<float>(m_rcFish.top) + vBottom.fY,
				static_cast<float>(m_rcFish.right - m_rcFish.left),
				static_cast<float>(m_rcFish.bottom - m_rcFish.top) };

#ifdef _DEBUG
	SolidBrush WhiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&WhiteBrush, DestRect);
#endif

	vCellSize = { static_cast<float>(pImg->GetWidth()), static_cast<float>(pImg->GetHeight()) };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY,
		UnitPixel
	);

}

void CFishingPlayingUI::Release()
{
}

void CFishingPlayingUI::Show()
{
	// 이미 Show 상태라면 패스
	if (m_bView)
		return;

	m_bView = true;
	
	InitializeData();
}

void CFishingPlayingUI::Hide()
{
	// 이미 Hide 상태라면 패스
	if (!m_bView)
		return;

	m_bView = false;
}

void CFishingPlayingUI::InitializeData()
{
	m_vFish = { 0.f, 0.f };
	m_vNet = { 0.f, 0.f };
	m_fCatchGauge = 0.f;

	m_fFishSpeed = 0.f;
	m_fNetSpeed = 0.f;

	m_dFishChangeMoveElapseTime = 0.;
	m_bFishChangeMove = true;

	m_bIsCol = false;

	UpdateRect();
}

void CFishingPlayingUI::UpdateTime()
{
	m_dFishChangeMoveElapseTime += DT;

	if (m_dFishChangeMoveElapseTime >= m_dFishChangeMoveInterval)
	{
		m_dFishChangeMoveElapseTime -= m_dFishChangeMoveInterval;
		m_bFishChangeMove = true;
	}
}


void CFishingPlayingUI::Move()
{
	MoveFish();
	MoveNet();
}

void CFishingPlayingUI::MoveFish()
{
	float fRectHeight = m_rcFish.bottom - m_rcFish.top;
	float fMinY = -143.f * m_fUIScale + fRectHeight * 0.5f;	// 가장 위 
	float fMaxY = -fRectHeight * 0.5f;						// 가장 아래

	if (m_bFishChangeMove)
	{
		// 속도와 방향 랜덤 뽑기
		float fSpeed = uniform_real_distribution<float>(100.f, 200.f)(g_engine);
		int	iDir = uniform_int_distribution<int>(0, 1)(g_engine);

		m_fFishSpeed = (iDir == 0) ? fSpeed : -fSpeed;
		m_bFishChangeMove = false;

		if (m_vFish.fY <= fMinY)
			m_fFishSpeed = fSpeed;
		if (m_vFish.fY >= fMaxY)
			m_fFishSpeed = -fSpeed;
	}

	m_vFish.fY += m_fFishSpeed * DT;
	m_vFish.fY = clamp(m_vFish.fY, fMinY, fMaxY);
}

void CFishingPlayingUI::MoveNet()
{
	// 가속도
	float fAccel = 300.f;

	float fRectHeight = m_rcNet.bottom - m_rcNet.top;
	float fMinY = -143.f * m_fUIScale + fRectHeight * 0.5f;	// 가장 위 
	float fMaxY = -fRectHeight * 0.5f;						// 가장 아래

	if (KEY_DOWN(VK_LBUTTON) || KEY_HOLD(VK_LBUTTON))
		m_fNetSpeed -= fAccel * DT;
	else
		m_fNetSpeed += fAccel * DT;

	m_fNetSpeed = clamp(m_fNetSpeed, -100.f, 100.f);

	m_vNet.fY += m_fNetSpeed * m_fUIScale * DT;

	// 만약에 바닥에 도달했을 시 속도 크기를 조금 줄이는 대신 방향을 바꿔 통통 튀게 만든다.
	if (m_vNet.fY >= fMaxY)
		m_fNetSpeed *= -0.9;

	// 특정 크기 clamp
	m_vNet.fY = clamp(m_vNet.fY, fMinY, fMaxY);
}

void CFishingPlayingUI::UpdateRect()
{
	UpdateFishRect();
	UpdateNetRect();
}
void CFishingPlayingUI::UpdateFishRect()
{
	VEC vSize = VEC{ 15.f, 15.f } * m_fUIScale;
	SetRect(&m_rcFish, m_vFish.fX - vSize.fX * 0.5f, 
					   m_vFish.fY - vSize.fY * 0.5f, 
					   m_vFish.fX + vSize.fX * 0.5f, 
					   m_vFish.fY + vSize.fY * 0.5f);
}

void CFishingPlayingUI::UpdateNetRect()
{
	VEC vSize = VEC{ 9.f, 30.f } * m_fUIScale;
	SetRect(&m_rcNet, m_vNet.fX - vSize.fX * 0.5f,
					  m_vNet.fY - vSize.fY * 0.5f,
					  m_vNet.fX + vSize.fX * 0.5f,
					  m_vNet.fY + vSize.fY * 0.5f);
}

void CFishingPlayingUI::HandleCollision()
{
	float fGaugeInc = 15.f * DT;
	if (m_bIsCol = CCollisionMgr::CollisionRect(m_rcFish, m_rcNet))
		m_fCatchGauge = clamp(m_fCatchGauge + fGaugeInc, 0.f, 100.f);
	else
		m_fCatchGauge = clamp(m_fCatchGauge - fGaugeInc * 0.8f, 0.f, 100.f);
}
