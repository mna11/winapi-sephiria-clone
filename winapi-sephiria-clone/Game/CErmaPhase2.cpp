#include "pch.h"
#include "CErmaPhase2.h"
#include "CBossErma.h"

CErmaPhase2::CErmaPhase2(CBossErma* pBoss)
	: CErmaPhase(pBoss)
{
}

CErmaPhase2::~CErmaPhase2()
{
	Release();
}

void CErmaPhase2::Initialize()
{
}

void CErmaPhase2::Update()
{
	if (m_pBoss->GetState() == BOSS_ERMA_STATE::IDLE)
	{
		m_pBoss->RequestChange(BOSS_ERMA_STATE::LASER);
	}
}

void CErmaPhase2::LateUpdate()
{
}

void CErmaPhase2::Release()
{
}
