#pragma once

#include "Define.h"

class CMyImg
{
public:
	CMyImg();
	~CMyImg();

public:
	HDC		GetMemDC() { return m_hMemDC; }

public:
	void		LoadImg(const TCHAR* pFilePath);
	void		Release();

private:
	HDC		m_hMemDC;

	Image*   m_pImage;
};

