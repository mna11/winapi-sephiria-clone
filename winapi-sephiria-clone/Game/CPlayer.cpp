#include "pch.h"
#include "CPlayer.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCameraMgr.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo = { WINCX >> 1, WINCY >> 1, 100.f, 100.f };
	m_eRender = RENDERID::GAMEOBJECT;

	m_fSpeed = 200.f;
}

int CPlayer::Update()
{
	if (m_bDead)
		return DEAD;

	KeyInput();

	__super::UpdateRect();
	return NOEVENT;
}

void CPlayer::LateUpdate()
{
}

void CPlayer::Render(Graphics* pGraphics)
{

#pragma push_macro("new")
#undef new
	Pen* pen = new Pen(Color(255, 0, 0, 0), 3);
	int iScrollX = CCameraMgr::GetInstance()->GetScrollX();
	int iScrollY = CCameraMgr::GetInstance()->GetScrollY();
	pGraphics->DrawRectangle(pen, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY, (int)m_tInfo.vSize.fX, (int)m_tInfo.vSize.fY);
	delete pen;
#pragma pop_macro("new")

}

void CPlayer::Release()
{
}

void CPlayer::KeyInput()
{
	if (KEY_PRESS(VK_RIGHT))
	{
		m_tInfo.vPoint.fX += m_fSpeed * DT;
	}
	if (KEY_PRESS(VK_LEFT))
	{
		m_tInfo.vPoint.fX -= m_fSpeed * DT;
	}
	if (KEY_PRESS(VK_UP))
	{
		m_tInfo.vPoint.fY -= m_fSpeed * DT;
	}
	if (KEY_PRESS(VK_DOWN))
	{
		m_tInfo.vPoint.fY += m_fSpeed * DT;
	}
}
