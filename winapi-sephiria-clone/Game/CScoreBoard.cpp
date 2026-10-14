#include "pch.h"
#include "CScoreBoard.h"

#include "CPlayer.h"

#include "CImgMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CSoundMgr.h"
#include "CCameraMgr.h"
#include "CFontMgr.h"

CScoreBoard::CScoreBoard()
	: m_pRetryButton(nullptr), m_pPlayer(nullptr)
{
}

CScoreBoard::~CScoreBoard()
{
	Release();
}

void CScoreBoard::Initialize()
{
	Init_CreateObj();
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/ScoreBoard/GameEnd.png", L"GameEnd");
	CSoundMgr::GetInstance()->PlaySound(L"GameClear.wav", CHANNEL_GROUPID::SFX, 1.f);
}

void CScoreBoard::Update()
{
	CObjMgr::GetInstance()->Update();
}

void CScoreBoard::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();
}

void CScoreBoard::Render(Graphics* pGraphics)
{
	// 배경 검정색으로 칠하기
	HDC hBackDC = pGraphics->GetHDC();

	RECT rc{ 0, 0, WINCX, WINCY };
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hBackDC, &rc, brush);
	DeleteObject(brush);
	pGraphics->ReleaseHDC(hBackDC);

	// GameEnd 플레이어 백그라운드 띄우기
	Image* pImg = CImgMgr::GetInstance()->FindImg(L"GameEnd");
	if (nullptr == pImg)
		return;
	VEC vCellSize = { static_cast<float>(pImg->GetWidth()), static_cast<float>(pImg->GetHeight()) };
	VEC vImgSize = vCellSize * PIXEL_SCALE;
	VEC vOffset{ WINCX >> 1, WINCY >> 1 };
	RectF DestRect{ vOffset.fX - vImgSize.fX * 0.5f, vOffset.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY,
		UnitPixel
	);

	// 클리어 문구 띄우기
	VEC vRectSize{ 200.f, 100.f };
	vOffset = { WINCX >> 1, WINCY * 0.1f };
	DestRect = { vOffset.fX - vRectSize.fX * 0.5f, vOffset.fY - vRectSize.fY * 0.5f, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush WhiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&WhiteBrush, DestRect);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, L"클리어", FONT_TYPE::NORMAL, DestRect, { 255, 36, 191, 255 }, 48.f);

	vOffset = { WINCX >> 1, WINCY * 0.7f };
	vRectSize = { 500.f, 50.f };
	float fGap = 50.f;
	DestRect = { vOffset.fX - vRectSize.fX * 0.5f, vOffset.fY - vRectSize.fY * 0.5f, vRectSize.fX, vRectSize.fY };

	// 생존 시간 띄우기
	double dAliveTime = CTimeMgr::GetInstance()->GetGameTime();
	wstring strTitle = L"생존 시간";
	CFontMgr::GetInstance()->DrawString(pGraphics, strTitle, FONT_TYPE::NORMAL, DestRect, { 255, 255, 255, 255 }, 24.f, StringAlignmentNear);
	int iTotalSec = static_cast<int>(dAliveTime);
	int iHour = iTotalSec / 3600;
	int iMinute = (iTotalSec % 3600) / 60;
	int iSecond = iTotalSec % 60;
	wstring strValue = format(L"{:02}:{:02}:{:02}", iHour, iMinute, iSecond);
	CFontMgr::GetInstance()->DrawString(pGraphics, strValue, FONT_TYPE::NORMAL, DestRect, { 255, 255, 255, 255 }, 24.f, StringAlignmentFar);

	// 처치한 몬스터 수
	DestRect.Y += fGap;
	strTitle = L"처치한 몬스터 수";
	CFontMgr::GetInstance()->DrawString(pGraphics, strTitle, FONT_TYPE::NORMAL, DestRect, { 255, 255, 255, 255 }, 24.f, StringAlignmentNear);
	strValue = to_wstring(m_pPlayer->GetKill());
	CFontMgr::GetInstance()->DrawString(pGraphics, strValue, FONT_TYPE::NORMAL, DestRect, { 255, 255, 255, 255 }, 24.f, StringAlignmentFar);

	// 최종 리프
	DestRect.Y += fGap;
	strTitle = L"최종 리프";
	CFontMgr::GetInstance()->DrawString(pGraphics, strTitle, FONT_TYPE::NORMAL, DestRect, { 255, 255, 255, 255 }, 24.f, StringAlignmentNear);
	strValue = to_wstring(m_pPlayer->GetLeaf());
	CFontMgr::GetInstance()->DrawString(pGraphics, strValue, FONT_TYPE::NORMAL, DestRect, { 255, 255, 255, 255 }, 24.f, StringAlignmentFar);


	CObjMgr::GetInstance()->Render(pGraphics);
}

void CScoreBoard::Release()
{
	// 버튼 삭제 규칙
	if (nullptr != m_pRetryButton)
	{
		m_pRetryButton->SetDead(true);
		m_pRetryButton = nullptr;
	}
}

void CScoreBoard::Init_CreateObj()
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
	{
		pPlayer = static_cast<CPlayer*>(CAbstractFactory<CPlayer>::CreateObj(WINCX >> 1, WINCY >> 1));
		CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, pPlayer);
	}
	else
		pPlayer->SetPos({ WINCX >> 1, WINCY >> 1 });

	pPlayer->SetPlayerBehaviorEnable(false);
	m_pPlayer = pPlayer;

	VEC vButtonPoint{ WINCX >> 1, WINCY * 0.93f };
	VEC vButtonSize{ 260.f, 70.f };
	RectF DestRect{ vButtonPoint.fX - vButtonSize.fX * 0.5f, vButtonPoint.fY - vButtonSize.fY * 0.5f, vButtonSize.fX, vButtonSize.fY };
	// 버튼 등록
	m_pRetryButton = CAbstractFactory<CButton>::CreateButton(
		CObjMgr::GetInstance()->GetMouse(),
		DestRect,
		L"시작 화면으로 돌아가기"
	);

	// 버튼 함수 등록
	m_pRetryButton->SetOnClick([this]() {
		CSceneMgr::GetInstance()->RequestNodeChange(1, SCENEID::START);
	});
	CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pRetryButton);
}
