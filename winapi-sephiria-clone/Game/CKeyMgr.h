#pragma once

class CKeyMgr
{
private:
	CKeyMgr();
	~CKeyMgr();
	CKeyMgr(const CKeyMgr&) = delete;
	const CKeyMgr& operator=(const CKeyMgr&) = delete;
public:
	static CKeyMgr* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CKeyMgr;
			m_pInstance->Initialize();
		}
		return m_pInstance;
	}

	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Initialize();
	void Update();
	void Release();

public:
	bool KeyDown(int iKey) const	{ return KEY_STATE::DOWN == m_arrKeyState[iKey]; }
	bool KeyUp(int iKey) const		{ return KEY_STATE::UP == m_arrKeyState[iKey]; }
	bool KeyHold(int iKey) const	{ return KEY_STATE::HOLD == m_arrKeyState[iKey]; }
	bool KeyPress(int iKey) const	{ return (KeyDown(iKey) || KeyHold(iKey)); }

private:
	static CKeyMgr* m_pInstance;
	array<KEY_STATE, VK_MAX> m_arrKeyState;
};
