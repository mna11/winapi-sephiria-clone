#include "pch.h"
#include "CScene.h"

CScene::CScene()
	: m_hMapDC(0), m_hMapBitmap(0), m_hOldBitmap(0)
{
}

CScene::~CScene()
{
	SelectObject(m_hMapDC, m_hOldBitmap);
	DeleteObject(m_hMapBitmap);
	DeleteDC(m_hMapDC);
}

void CScene::Init_LoadImg(wstring wstr)
{
    Bitmap::FromFile(wstr.c_str())->GetHBITMAP(
        Color(255, 255, 0, 255),  // 배경색
        &m_hMapBitmap             // HBITMAP 저장
    );

    HDC hDC = GetDC(g_hWnd);
    m_hMapDC = CreateCompatibleDC(hDC);
    ReleaseDC(g_hWnd, hDC);

    m_hOldBitmap = (HBITMAP)SelectObject(m_hMapDC, m_hMapBitmap);
}
