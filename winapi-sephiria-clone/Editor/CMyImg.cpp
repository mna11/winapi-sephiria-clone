#include "pch.h"
#include "CMyImg.h"

CMyImg::CMyImg()
	: m_pImage(nullptr)
{
}

CMyImg::~CMyImg()
{
}

void CMyImg::LoadImg(const TCHAR* pFilePath)
{
	HDC		hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);



	// 이미지 파일 불러오기
	m_hBitmap = (HBITMAP)LoadImage(NULL,		// 프로그램 인스턴스 핸들(파일로부터 읽어올 것이기 때문에 null값 전달)
		pFilePath,   // 파일 이름을 포함한 경로
		IMAGE_BITMAP, // 어떤 타입을 읽어올 것인가
		0, 0,		 // 이미지 가로, 세로 사이즈(파일로부터 읽어오기 때문에 별도의 크기 설정이 필요없다)
		LR_LOADFROMFILE | LR_CREATEDIBSECTION); // LR_LOADFROMFILE : 파일로부터 이미지 부르기 설정
	// LR_CREATEDIBSECTION : 읽어온 파일을 DIB 형태로 변환

	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CMyImg::Release()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
