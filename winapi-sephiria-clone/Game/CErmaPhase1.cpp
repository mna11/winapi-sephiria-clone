#include "pch.h"
#include "CErmaPhase1.h"
#include "CBossErma.h"
#include "CErma.h"
#include "CErmaHand.h"

CErmaPhase1::CErmaPhase1(CBossErma* pBoss)
	: CErmaPhase(pBoss)
{
}

CErmaPhase1::~CErmaPhase1()
{
	Release();
}

void CErmaPhase1::Initialize()
{
}

void CErmaPhase1::Update()
{
	if (m_pBoss == nullptr)
		return;

	switch (m_pBoss->GetState())
	{
	case BOSS_ERMA_STATE::WAIT:
		if (m_pBoss->IsTargetInArena())
			m_pBoss->RequestChange(BOSS_ERMA_STATE::INTRO);
		break;

	case BOSS_ERMA_STATE::INTRO:
		if (m_pBoss->m_dStateElapseTime >= 0.8)
			m_pBoss->RequestChange(BOSS_ERMA_STATE::IDLE);
		break;

	case BOSS_ERMA_STATE::IDLE:
		if (m_pBoss->m_dStateElapseTime >= 0.5)
		{
			switch (m_pBoss->m_iNextPattern)
			{
			case 0:
				m_pBoss->m_pAttackingHand = m_pBoss->m_bNextLeftHand
					? m_pBoss->m_pLeftHand
					: m_pBoss->m_pRightHand;
				m_pBoss->m_bNextLeftHand = !m_pBoss->m_bNextLeftHand;
				m_pBoss->RequestChange(BOSS_ERMA_STATE::ATTACK);
				break;
			case 1:
				m_pBoss->RequestChange(BOSS_ERMA_STATE::MISSILE);
				break;
			default:
				m_pBoss->RequestChange(BOSS_ERMA_STATE::LASER);
				break;
			}

			m_pBoss->m_iNextPattern = (m_pBoss->m_iNextPattern + 1) % 3;
		}
		break;

	case BOSS_ERMA_STATE::ATTACK:
		if (m_pBoss->m_dStateElapseTime >= 0.5 &&
			(m_pBoss->m_pAttackingHand == nullptr ||
				!m_pBoss->m_pAttackingHand->IsBusy()))
		{
			m_pBoss->RequestChange(BOSS_ERMA_STATE::IDLE);
		}
		break;

	case BOSS_ERMA_STATE::MISSILE:
		if (m_pBoss->m_dStateElapseTime >= 3.6)
			m_pBoss->RequestChange(BOSS_ERMA_STATE::IDLE);
		break;

	case BOSS_ERMA_STATE::LASER:
		if (m_pBoss->m_dStateElapseTime >= 4.1)
			m_pBoss->RequestChange(BOSS_ERMA_STATE::IDLE);
		break;

	case BOSS_ERMA_STATE::EXPOSED:
		if (m_pBoss->m_pErma == nullptr ||
			m_pBoss->m_pErma->IsReturnComplete())
		{
			m_pBoss->RequestChange(BOSS_ERMA_STATE::RECOVER);
		}
		break;

	case BOSS_ERMA_STATE::RECOVER:
		if (m_pBoss->m_dStateElapseTime >= 1.)
			m_pBoss->RequestChange(BOSS_ERMA_STATE::IDLE);
		break;

	default:
		break;
	}
}

void CErmaPhase1::LateUpdate()
{
}

void CErmaPhase1::Release()
{
}
