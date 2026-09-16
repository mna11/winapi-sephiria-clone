#pragma once

#include "CState.h"
class CScene;

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();
	CSceneMgr(const CSceneMgr& rhs) = delete;
	CSceneMgr& operator=(CSceneMgr& rSceneMgr) = delete;

public:
	static CSceneMgr* GetInstance()
	{
		if (!m_pInstance) {
			m_pInstance = new CSceneMgr;
		}

		return m_pInstance;
	}

	static void	DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Update();
	void LateUpdate();
	void Render(Graphics* pGraphics);
	void Release();

public:
	// 저번 팀 과제 때, LateUpdate에서 씬 체인지를 불렀더니
	// 이후의 것들을 부를려고 하다가 크래시 난 적이 있어서 그냥 아싸리 프레임 끝나고 
	// 다음 프레임 시작할 때, 다르면 변경할거다.
	void ApplyChange();
	void RequestChange(SCENEID eScene);
	
private:
	static CSceneMgr* m_pInstance;
	CScene* m_pScene;

	SCENEID	m_eCurScene;
	SCENEID m_ePreScene;
};

