#include "pch.h"
#include "CErmaPhase1.h"
#include "CBossErma.h"

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
	if (m_pBoss->GetState() == BOSS_ERMA_STATE::IDLE)
	{
		m_pBoss->RequestChange(BOSS_ERMA_STATE::ATTACK);
	}
}

void CErmaPhase1::LateUpdate()
{
}

void CErmaPhase1::Release()
{
}
