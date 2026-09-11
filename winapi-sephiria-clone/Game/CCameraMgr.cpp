#include "pch.h"
#include "CCameraMgr.h"

CCameraMgr* CCameraMgr::m_pInstance = nullptr;

CCameraMgr::CCameraMgr()
	: m_fScrollX(0.f), m_fScrollY(0.f)
{
}

CCameraMgr::~CCameraMgr()
{
}

void CCameraMgr::Scroll_Lock()
{
	if (0.f < m_fScrollX)
		m_fScrollX = 0.f;

	if (0.f < m_fScrollY)
		m_fScrollY = 0.f;

	if (WINCX - 1920.f > m_fScrollX)
		m_fScrollX = WINCX - 1920.f;

	if (WINCY - 1280.f > m_fScrollY)
		m_fScrollY = WINCY - 1280.f;

}
