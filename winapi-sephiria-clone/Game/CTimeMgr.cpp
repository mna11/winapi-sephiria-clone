#include "pch.h"
#include "CTimeMgr.h"
#include "Define.h"
#include "CMainGame.h"

CTimeMgr* CTimeMgr::m_pInstance = nullptr;

CTimeMgr::CTimeMgr()
	: m_iFPS(0), m_dDT(0.), m_dAcc(0.), m_iCallCount(0)
{
	ZeroMemory(&m_llCurCount, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_llPrevCount, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_llFrequncyCount, sizeof(LARGE_INTEGER));
	ZeroMemory(m_szFPS, sizeof(LARGE_INTEGER));
}

CTimeMgr::~CTimeMgr()
{
	Release();
}

void CTimeMgr::Initialize()
{
	QueryPerformanceFrequency(&m_llFrequncyCount);
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeMgr::Update()
{
	++m_iCallCount;

	QueryPerformanceCounter(&m_llCurCount);
	// QuadPart는 LONGLONG 64비트 한번에 보기
	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequncyCount.QuadPart;
	m_llPrevCount = m_llCurCount;

	m_dAcc += m_dDT;
	if (m_dAcc >= 1.)
	{
		m_iFPS = m_iCallCount;
		m_dAcc = 0.;
		m_iCallCount = 0;

		swprintf_s(m_szFPS, L"FPS : %d DT : %lf", m_iFPS, m_dDT);
		SetWindowText(g_hWnd, m_szFPS);
	}
}

void CTimeMgr::Release()
{
}