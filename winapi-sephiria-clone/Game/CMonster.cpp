#include "pch.h"
#include "CMonster.h"

#include "CCameraMgr.h"
#include "CObjMgr.h"
#include "CEffectMgr.h"

CMonster::CMonster()
{
	// 몬스터의 타겟은 항상 플레이어
	// 근데 나중에 동료 구현할 때는 이거 없애야 한다. 
	m_pTarget = CObjMgr::GetInstance()->GetPlayer();
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::SetDamage(int iDamage, CObj* pObj)
{
	m_iHp -= iDamage;

	if (m_iHp <= 0)
		m_bDead = true;

	// 나중에 피해량 이펙트로 띄우기
	// 경험치 pObj에게 넘기기 등을 하면 된다.
}
