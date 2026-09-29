#include "pch.h"
#include "CUI.h"

CUI::CUI()
	: m_bView(false), m_fUIScale(PIXEL_SCALE), m_pMouse(nullptr)
{
}

CUI::~CUI()
{
}

void CUI::Show()
{
	m_bView = true;
}

void CUI::Hide()
{
	m_bView = false;
}

void CUI::Toggle()
{
	m_bView = !m_bView;
}
