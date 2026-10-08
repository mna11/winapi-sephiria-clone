#include "pch.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CStage.h"
#include "CObjMgr.h"
#include "CStage1.h"
#include "CLibLoading.h"
#include "CBossStage.h"
#include "CShop.h"
#include "CForge.h"
#include "CStageTest.h"

#include "CShopTable.h"
#include "CUIMgr.h"
#include "CAbstractFactory.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr() 
	: 
	m_eCurScene(SCENEID::END), 
	m_ePreScene(SCENEID::END),
	m_eSaveScene(SCENEID::END),
	m_pScene(nullptr), m_pSaveScene(nullptr),
	m_iCurrentNodeID(0), m_iRequestNodeID(-1)
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
	SafeDelete(m_pSaveScene);
}

void CSceneMgr::ApplyChange()
{
	if (m_ePreScene != m_eCurScene || m_iRequestNodeID != -1)
	{

		// 씬 전환할 때, UI를 다 닫음
		CUIMgr::GetInstance()->HideAllUI();

		// 상점, 대장간과 관련된 씬 전환이 아니라면 이전 씬 삭제
		if(!HandleChangeReturnScene())
			SafeDelete(m_pScene);

		switch (m_eCurScene)
		{
		case SCENEID::LIB_LOADING:
		{
			if (nullptr == m_pScene)
				m_pScene = CAbstractFactory<CLibLoading>::CreateScene();
			break;
		}
		case SCENEID::SHOP:
		{
			if (nullptr == m_pScene)
				m_pScene = CAbstractFactory<CShop>::CreateScene();
			CUIMgr::GetInstance()->ShowUI(UIID::INVENTORY);
			CUIMgr::GetInstance()->ShowUI(UIID::SHOP_TABLE);
			static_cast<CShopTable*>(CUIMgr::GetInstance()->GetUI(UIID::SHOP_TABLE))->SetSellingItem(m_eSaveScene);
			break;
		}
		case SCENEID::FORGE:
		{
			if (nullptr == m_pScene)
				m_pScene = CAbstractFactory<CForge>::CreateScene();
			CUIMgr::GetInstance()->ShowUI(UIID::FORGE);
			break;
		}
		case SCENEID::STAGE0:
		{
			//if (nullptr == m_pScene)
			//	m_pScene = CAbstractFactory<CStage0>::CreateScene();
			CUIMgr::GetInstance()->ShowUI(UIID::BASIC_INFO);
			break;
		}
		case SCENEID::STAGE1:
		{

			if (nullptr == m_pScene)
				m_pScene = CAbstractFactory<CStage1>::CreateScene();
			CUIMgr::GetInstance()->ShowUI(UIID::BASIC_INFO);
			CUIMgr::GetInstance()->ShowUI(UIID::FADE);
			break;
		}
		case SCENEID::STAGE_TEST:
		{

			if (nullptr == m_pScene)
				m_pScene = CAbstractFactory<CStageTest>::CreateScene();
			CUIMgr::GetInstance()->ShowUI(UIID::BASIC_INFO);
			CUIMgr::GetInstance()->ShowUI(UIID::FADE);
			break;
		}
		case SCENEID::BOSS_STAGE:
		{
			if (nullptr == m_pScene)
				m_pScene = CAbstractFactory<CBossStage>::CreateScene();
			CUIMgr::GetInstance()->ShowUI(UIID::BASIC_INFO);
			CUIMgr::GetInstance()->ShowUI(UIID::FADE);

			break;
		}
		default:
			break;
		}

		m_ePreScene = m_eCurScene;

		if (m_iRequestNodeID != -1)
		{
			m_iCurrentNodeID = m_iRequestNodeID;
			m_iRequestNodeID = -1;
		}
	}
}

void CSceneMgr::RequestChange(SCENEID eScene)
{
	m_eCurScene = eScene;
}

void CSceneMgr::RequestNodeChange(int iNodeID, SCENEID eSceneID)
{
	m_iRequestNodeID = iNodeID;
	RequestChange(eSceneID);
}

void CSceneMgr::BackToSaveScene()
{
	if (SCENEID::END != m_eSaveScene)
	{
		RequestChange(m_eSaveScene);
	}
}

bool CSceneMgr::HandleChangeReturnScene()
{
	// 상점에 진입하는 경우
	if (m_eCurScene == SCENEID::SHOP || m_eCurScene == SCENEID::FORGE)
	{
		// 이전 스테이지 정보를 기억한다.
		m_pSaveScene = m_pScene;

		// 세팅 
		m_pScene = nullptr;
		m_eSaveScene = m_ePreScene;
		return true;
	}

	// 상점에서 나가는 경우
	if (m_ePreScene == SCENEID::SHOP || m_ePreScene == SCENEID::FORGE)
	{
		// 상점 씬을 삭제하고 원래 기억해둔 씬으로 바꾼다.
		SafeDelete<CScene*>(m_pScene);
		m_pScene = m_pSaveScene;

		// 초기화
		m_pSaveScene = nullptr;
		m_eSaveScene = SCENEID::END;
		return true;
	}

	return false;
}