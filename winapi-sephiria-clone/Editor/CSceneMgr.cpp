#include "pch.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CStage.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr() 
	: 
	m_eCurScene(SCENEID::STAGE), 
	m_ePreScene(SCENEID::END),
	m_pScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Update()
{
	ApplyChange();

	m_pScene->Update();
}

void CSceneMgr::LateUpdate()
{
	m_pScene->LateUpdate();
}

void CSceneMgr::Render(Graphics* pGraphics)
{
	m_pScene->Render(pGraphics);
}

void CSceneMgr::Release()
{
	SafeDelete(m_pScene);
}

void CSceneMgr::ApplyChange()
{
	if (m_ePreScene != m_eCurScene)
	{
		SafeDelete(m_pScene);

		switch (m_eCurScene)
		{
		case SCENEID::STAGE:
			m_pScene = new CStage;
			break;

		default:
			break;
		}

		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::RequestChange(SCENEID eScene)
{
	m_eCurScene = eScene;
}