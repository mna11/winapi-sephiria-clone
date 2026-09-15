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
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
	SolidBrush blackBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&blackBrush, (int)(m_tRect.left + vScroll.fX),
								  (int)(m_tRect.top + vScroll.fY), 
								  (int)m_tInfo.vSize.fX, 
								  (int)m_tInfo.vSize.fY);
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
