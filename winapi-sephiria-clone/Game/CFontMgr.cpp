#include "pch.h"
#include "CFontMgr.h"

CFontMgr* CFontMgr::m_pInstance = nullptr;

CFontMgr::CFontMgr()
	: m_sf(0), m_solidBrush(0)
{
	m_vecFont.reserve(toUType(FONT_TYPE::END));
	m_vecFontFamily.reserve(toUType(FONT_TYPE::END));
}

CFontMgr::~CFontMgr()
{
	Release();
}

void CFontMgr::Initialize()
{
#pragma push_macro("new")
#undef new
	// 폰트 패밀리 초기화
	m_vecFontFamily.push_back(new FontFamily(L"Pixel Big", g_pFontCollection));
	m_vecFontFamily.push_back(new FontFamily(L"Pixel Bold", g_pFontCollection));
	m_vecFontFamily.push_back(new FontFamily(L"Pixel Small", g_pFontCollection));
	m_vecFontFamily.push_back(new FontFamily(L"Galmuri7 Regular", g_pFontCollection));
	
	// 폰트 초기화
	m_vecFont.push_back(new Font(m_vecFontFamily[toUType(FONT_TYPE::PIXEL_BIG)], 24.f, FontStyleRegular, UnitPixel));
	m_vecFont.push_back(new Font(m_vecFontFamily[toUType(FONT_TYPE::PIXEL_BOLD)], 24.f, FontStyleRegular, UnitPixel));
	m_vecFont.push_back(new Font(m_vecFontFamily[toUType(FONT_TYPE::PIXEL_SMALL)], 18.f, FontStyleRegular, UnitPixel));
	m_vecFont.push_back(new Font(m_vecFontFamily[toUType(FONT_TYPE::NORMAL)], 21.f, FontStyleRegular, UnitPixel));
#pragma pop_macro("new")

	// 폰트 정렬 초기화
	m_sf.SetAlignment(StringAlignmentCenter);
	m_sf.SetLineAlignment(StringAlignmentCenter);

	// 폰트 브러쉬 초기화
	m_solidBrush.SetColor(Color(255, 0, 0, 0));
}

void CFontMgr::Release()
{
	// 할당 해제하기
	for_each(m_vecFont.begin(), m_vecFont.end(), SafeDelete<Font*>);
	m_vecFont.clear();
	m_vecFont.shrink_to_fit();

	for_each(m_vecFontFamily.begin(), m_vecFontFamily.end(), SafeDelete<FontFamily*>);
	m_vecFontFamily.clear();
	m_vecFontFamily.shrink_to_fit();
}

void CFontMgr::DrawString(Graphics* pGraphics, wstring& wstr, FONT_TYPE eFont, RectF rc, Color color, float fSize, StringAlignment alignment, StringAlignment lineAlignment)
{
	m_sf.SetAlignment(alignment);
	m_sf.SetLineAlignment(lineAlignment);
	m_solidBrush.SetColor(color);

	// 별 다른 폰트 크기 요청 없었음
	if (fSize == -1)
	{
		pGraphics->DrawString(wstr.c_str(), -1, m_vecFont[toUType(eFont)], rc, &m_sf, &m_solidBrush);
	}
	else
	{
		Font font(m_vecFontFamily[toUType(eFont)], fSize, FontStyleRegular, UnitPixel);
		pGraphics->DrawString(wstr.c_str(), -1, &font, rc, &m_sf, &m_solidBrush);
	}
}

