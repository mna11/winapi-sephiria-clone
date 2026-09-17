#include "pch.h"
#include "CObj.h"

#include "CTimeMgr.h"

CObj::CObj()
	:
	m_fSpeed(0.f),
	m_fAngle(0.f),
	m_bDead(false),
	m_pTarget(nullptr),
	m_pFrameKey(L""),
	m_eRender(RENDERID::END),
	m_iRenderLayer(0)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_vPrePoint, sizeof(VEC));
}

CObj::~CObj()
{
}

void CObj::SetFrame(int iStart, int iEnd, int iMotion, double dFrameSpeed)
{
	m_tFrame = { iStart, iEnd, iMotion, dFrameSpeed, 0UL};
	QueryPerformanceCounter(&m_tFrame.iFrameTime);
}

void CObj::UpdateRect()
{
	m_tRect.left =		LONG(m_tInfo.vPoint.fX - (m_tInfo.vSize.fX / 2.f));
	m_tRect.top =		LONG(m_tInfo.vPoint.fY - (m_tInfo.vSize.fY / 2.f));
	m_tRect.right =		LONG(m_tInfo.vPoint.fX + (m_tInfo.vSize.fX / 2.f));
	m_tRect.bottom =	LONG(m_tInfo.vPoint.fY + (m_tInfo.vSize.fY / 2.f));
}

void CObj::UpdateFrame()
{
	if (m_tFrame.dFrameSpeed <= CTimeMgr::GetInstance()->GetTime(m_tFrame.iFrameTime))
	{
		++m_tFrame.iStart;

		QueryPerformanceCounter(&m_tFrame.iFrameTime);

		if (m_tFrame.iStart > m_tFrame.iEnd)
			m_tFrame.iStart = 0;
	}
}