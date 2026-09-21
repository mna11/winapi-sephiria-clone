#include "pch.h"
#include "CBossErma.h"

#include "CErmaHead.h"
#include "CErmaBody.h"
#include "CErmaHand.h"
#include "CErma.h"

#include "CErmaPhase.h"
#include "CErmaPhase1.h"
#include "CErmaPhase2.h"

#include "CCameraMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"

CBossErma::CBossErma()
	: 
	CState(BOSS_ERMA_STATE::END, BOSS_ERMA_STATE::WAIT),
	m_pHead(nullptr),
	m_pBody(nullptr),
	m_pLeftHand(nullptr),
	m_pRightHand(nullptr),
	m_pErma(nullptr),
	m_dStateElapseTime(0.),
	m_pPhase(nullptr)
{
}

CBossErma::~CBossErma()
{
	Release();
}

void CBossErma::Initialize()
{
	// 기본 정보
	m_tInfo = { 0.f, 0.f, 0.f, 0.f };
	m_tStat = { 0, 0 };

	// 렌더링 정보 
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 5; // 플레이어보다 매우 높음

	// iFrame 설정
	m_dIframeTime = 0.16;
	m_dHitElapseTime = 0.;
	m_bHit = false;

	// 스프라이트 시트 인서트
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Body/BigGolem_Body.png", L"Erma_Golem_Body");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Effect/Head/BigGolem_Head.png",L"Erma_Golem_Head");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Hand/BigGolem_Hand_LEFT.png", L"Erma_Golem_Hand_L");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Hand/BigGolem_Hand_RIGHT.png", L"Erma_Golem_Hand_R");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_LEFT.png", L"Erma_L");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_RIGHT.png", L"Erma_R");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_GoToStone_LEFT.png", L"Erma_Stone_L");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_GoToStone_RIGHT.png", L"Erma_Stone_R");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_Stone_Shiny.png", L"Erma_Stone_Shiny");

	// 파츠 만들기
	m_pHead = CAbstractFactory<CErmaHead>::CreateObj();
	m_pBody = CAbstractFactory<CErmaBody>::CreateObj();
	m_pLeftHand = CAbstractFactory<CErmaHand>::CreateObj();
	m_pRightHand = CAbstractFactory<CErmaHand>::CreateObj();
	m_pErma = CAbstractFactory<CErma>::CreateObj();

	CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pHead);
	CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pBody);
	CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pLeftHand);
	CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pRightHand);
	CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, m_pErma);
}

int CBossErma::Update()
{
	if (m_bDead)
		return DEAD;

	ApplyChange();
	UpdateTime();

	if (m_pPhase != nullptr)
		m_pPhase->Update();

	return NOEVENT;
}

void CBossErma::LateUpdate()
{
	if (m_pPhase != nullptr)
		m_pPhase->LateUpdate();
}

void CBossErma::Render(Graphics* pGraphics)
{
}

void CBossErma::Release()
{
	SafeDelete<CErmaPhase*>(m_pPhase);
}

void CBossErma::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case BOSS_ERMA_STATE::WAIT:
			break;
		case BOSS_ERMA_STATE::INTRO:
			break;
		case BOSS_ERMA_STATE::IDLE:
			break;
		case BOSS_ERMA_STATE::ATTACK:
			break;
		case BOSS_ERMA_STATE::LASER:
			break;
		case BOSS_ERMA_STATE::MISSILE:
			break;
		default:
			break;
		}

		m_eCurState = m_eNextState;
		m_dStateElapseTime = 0.;
	}
}

void CBossErma::SetDamage(int iDamage, CObj* pObj)
{
}

void CBossErma::UpdateTime()
{
	// 여러 상태 기준 시간 업데이트
	m_dStateElapseTime += DT;

	// 피해 유효 근거용 시간
	if (m_bHit)
	{
		m_dHitElapseTime += DT;

		if (m_dHitElapseTime >= m_dIframeTime)
		{
			m_bHit = false;
			m_dHitElapseTime = 0.;
		}
	}
}

void CBossErma::ChangePhase(CErmaPhase* pNextPhase)
{
	if (m_pPhase != nullptr)
	{
		SafeDelete<CErmaPhase*>(m_pPhase);
		return;
	}

	m_pPhase = pNextPhase;

	if (m_pPhase != nullptr)
		m_pPhase->Initialize();
}
