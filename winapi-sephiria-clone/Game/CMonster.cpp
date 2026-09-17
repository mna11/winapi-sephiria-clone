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
}
