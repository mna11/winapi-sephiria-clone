#pragma once

#include "Define.h"

class CLine
{
public:
	CLine();
	explicit CLine(LINEPOINT& tLeft, LINEPOINT& tRight);
	~CLine();

public:
	void	Render(HDC hDC);

public:
	const LINE& GetLine() const { return m_tLine; }

private:
	LINE		m_tLine;

};