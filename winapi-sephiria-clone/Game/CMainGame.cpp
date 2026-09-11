#include "pch.h"
#include "CMainGame.h"

// 매니저
#include "CTimeMgr.h"

CMainGame::CMainGame()
	: m_hDC(0)
{
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

}

void CMainGame::Update()
{
	CTimeMgr::GetInstance()->Update();

}

void CMainGame::LateUpdate()
{
	CTimeMgr::GetInstance()->LateUpdate();

}
void CMainGame::Render()
{
	CTimeMgr::GetInstance()->Render();

}

void CMainGame::Release()
{
	CTimeMgr::GetInstance()->DestroyInstance();

	ReleaseDC(g_hWnd, m_hDC);
}