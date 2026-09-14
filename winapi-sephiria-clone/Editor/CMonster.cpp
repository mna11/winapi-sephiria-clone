#include "pch.h"
#include "CMonster.h"

#include "CCameraMgr.h"

CMonster::CMonster()
{
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	m_tInfo = { 0.f, 0.f, 100.f, 100.f };
	m_eRender = RENDERID::GAMEOBJECT;
}

int CMonster::Update()
{
	__super::UpdateRect();
	return NOEVENT;
}

void CMonster::LateUpdate()
{
}

void CMonster::Render(Graphics* pGraphics)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
	SolidBrush blackBrush(Color(255, 133, 0, 0));
	pGraphics->FillRectangle(&blackBrush, (int)(m_tRect.left + vScroll.fX),
		(int)(m_tRect.top + vScroll.fY),
		(int)m_tInfo.vSize.fX,
		(int)m_tInfo.vSize.fY);
}

void CMonster::Release()
{
}
