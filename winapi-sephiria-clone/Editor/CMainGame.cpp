#include "pch.h"
#include "CMainGame.h"

// 매니저
#include "CObjMgr.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CSceneMgr.h"
#include "CTileMgr.h"
#include "CTimeMgr.h"

CMainGame::CMainGame()
	: m_hDC(0), m_hBackDC(0), m_pBackGraphics(nullptr), m_hBackBit(0), m_hOldBit(0)
{
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	m_hBackBit = CreateCompatibleBitmap(m_hDC, WINCX, WINCY);
	m_hBackDC = CreateCompatibleDC(m_hDC);
	m_hOldBit = (HBITMAP)SelectObject(m_hBackDC, m_hBackBit);

#pragma push_macro("new")
#undef new

	m_pBackGraphics = new Gdiplus::Graphics(m_hBackDC);
	m_pBackGraphics->SetInterpolationMode(InterpolationModeNearestNeighbor);
	m_pBackGraphics->SetPixelOffsetMode(PixelOffsetModeHalf);
#pragma pop_macro("new")

	CSceneMgr::GetInstance()->RequestChange(SCENEID::EDITOR);
}

void CMainGame::Update()
{
	CTimeMgr::GetInstance()->Update();
	CKeyMgr::GetInstance()->Update();
	CCameraMgr::GetInstance()->Update();

	CSceneMgr::GetInstance()->Update();
}

void CMainGame::LateUpdate()
{
	CSceneMgr::GetInstance()->LateUpdate();

	CCameraMgr::GetInstance()->LateUpdate();
}

void CMainGame::Render()
{
	m_pBackGraphics->Clear(Color(255, 44, 43, 64));
	CSceneMgr::GetInstance()->Render(m_pBackGraphics);
	//m_pBackGraphics->Flush(FlushIntentionSync); // 대충 다 그릴 때까지 대기라는데

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, m_hBackDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	CTileMgr::DestroyInstance();
	CSceneMgr::DestroyInstance();
	CImgMgr::DestroyInstance();
	CCameraMgr::DestroyInstance();
	CKeyMgr::DestroyInstance();
	CLineMgr::DestroyInstance();
	CObjMgr::DestroyInstance();
	CTimeMgr::DestroyInstance();

	// 그래픽스 먼저 없애야 함
	SafeDelete<Graphics*>(m_pBackGraphics);

	SelectObject(m_hBackDC, m_hOldBit);
	DeleteObject(m_hBackBit);
	DeleteDC(m_hBackDC);

	ReleaseDC(g_hWnd, m_hDC);
}