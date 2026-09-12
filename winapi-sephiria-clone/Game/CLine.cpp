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
	int iScrollX = (int)CCameraMgr::GetInstance()->GetScrollX();
	int iScrollY = (int)CCameraMgr::GetInstance()->GetScrollY();

	MoveToEx(hDC, (int)m_tLine.tLPoint.vPoint.fX + iScrollX, (int)m_tLine.tLPoint.vPoint.fY, nullptr);
	LineTo(hDC, (int)m_tLine.tRPoint.vPoint.fX + iScrollX, (int)m_tLine.tRPoint.vPoint.fY);
}
