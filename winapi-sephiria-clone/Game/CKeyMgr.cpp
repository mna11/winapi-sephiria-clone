#include "pch.h"
#include "CKeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
	m_arrKeyState.fill(KEY_STATE::NONE);
}

CKeyMgr::~CKeyMgr()
{
	Release();
}

void CKeyMgr::Initialize()
{
	// 혹시 모를 재사용을 위해
	m_arrKeyState.fill(KEY_STATE::NONE);
}

void CKeyMgr::Update()
{
	HWND focusHWnd = GetFocus();
	if (g_hWnd == focusHWnd) // 현재 포커스된 윈도우의 경우
	{
		for (int i = 0; i < VK_MAX; ++i)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{
				if (KEY_STATE::DOWN == m_arrKeyState[i] || KEY_STATE::HOLD == m_arrKeyState[i])
					m_arrKeyState[i] = KEY_STATE::HOLD;
				else
					m_arrKeyState[i] = KEY_STATE::DOWN;
			}
			else
			{
				if (KEY_STATE::HOLD == m_arrKeyState[i] || KEY_STATE::DOWN == m_arrKeyState[i])
					m_arrKeyState[i] = KEY_STATE::UP;
				else
					m_arrKeyState[i] = KEY_STATE::NONE;
			}
		}
	}
	else // 현재 포커스된 윈도우가 아닐 경우, 키보드 입력 처리
	{
		for_each(m_arrKeyState.begin(), m_arrKeyState.end(),
			[](auto& eKeyState)
			{
				if (KEY_STATE::HOLD == eKeyState || KEY_STATE::DOWN == eKeyState)
					eKeyState = KEY_STATE::UP;
				else if (KEY_STATE::UP == eKeyState)
					eKeyState = KEY_STATE::NONE;
			});
	}
}

void CKeyMgr::Release()
{
}
