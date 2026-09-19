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
#include "CEffectMgr.h"
#include "CUIMgr.h"
#include "CFontMgr.h"
#include "CAbstractFactory.h"

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

	// Gdiplus는 오버로딩된 new를 쓰기 때문에, 해결 위해 작성
#pragma push_macro("new")
#undef new
	m_pBackGraphics = new Gdiplus::Graphics(m_hBackDC);
	m_pBackGraphics->SetInterpolationMode(InterpolationModeNearestNeighbor);
	m_pBackGraphics->SetPixelOffsetMode(PixelOffsetModeHalf); 
	m_pBackGraphics->SetTextRenderingHint(TextRenderingHintSingleBitPerPixel); // 폰트 안티 앨리어싱 없고 글자 픽셀 격자 맞게 보정
#pragma pop_macro("new")

	CSceneMgr::GetInstance()->RequestChange(SCENEID::STAGE);

	// 디버깅 시 콘솔 창 출력 
#ifdef _DEBUG
	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}
#endif // _DEBUG
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
	m_pBackGraphics->Clear(Color(255, 43, 44, 64));
	CSceneMgr::GetInstance()->Render(m_pBackGraphics);
	//m_pBackGraphics->Flush(FlushIntentionSync); // 대충 다 그릴 때까지 대기라는데

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, m_hBackDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	CEffectMgr::DestroyInstance();
	CTileMgr::DestroyInstance();
	CSceneMgr::DestroyInstance();
	CImgMgr::DestroyInstance();
	CCameraMgr::DestroyInstance();
	CKeyMgr::DestroyInstance();
	CLineMgr::DestroyInstance();
	CObjMgr::DestroyInstance();
	CTimeMgr::DestroyInstance();
	CUIMgr::DestroyInstance();
	CFontMgr::DestroyInstance();

	// 그래픽스 먼저 없애야 함
	SafeDelete<Graphics*>(m_pBackGraphics);

	SelectObject(m_hBackDC, m_hOldBit);
	DeleteObject(m_hBackBit);
	DeleteDC(m_hBackDC);

	ReleaseDC(g_hWnd, m_hDC);
}