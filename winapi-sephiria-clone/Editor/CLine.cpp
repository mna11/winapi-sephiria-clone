#include "pch.h"
#include "CLine.h"
#include "CCameraMgr.h"

CLine::CLine()
{
}

CLine::CLine(LINEPOINT& tLeft, LINEPOINT& tRight)
	: m_tLine(tLeft, tRight)
{
}

CLine::~CLine()
{
}

void CLine::Render(HDC hDC)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

	MoveToEx(hDC, (int)(m_tLine.tLPoint.vPoint.fX + vScroll.fX), (int)(m_tLine.tLPoint.vPoint.fY + vScroll.fY), nullptr);
	LineTo(hDC, (int)(m_tLine.tRPoint.vPoint.fX + vScroll.fX), (int)(m_tLine.tRPoint.vPoint.fY + vScroll.fY));
}
