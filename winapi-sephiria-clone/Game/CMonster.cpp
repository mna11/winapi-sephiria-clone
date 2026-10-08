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

	// 몬스터 사망 시 사용할, imgAttr 초기화 하기 - 채도를 내린다.
	float fDarkness = 0.3f;
	ColorMatrix colorDownMatrix = { fDarkness, 0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, fDarkness, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, fDarkness, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	// 피격 시 사용 할, imgAttr 초기화 하기 - 빨간색만 남긴다.
	ColorMatrix colorHitMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	m_imgAttrHit.SetColorMatrix(&colorHitMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
	m_imgAttrDown.SetColorMatrix(&colorDownMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	// iframe 세팅 
	m_dHitElapseTime = 0.;
	m_dIframeTime = 0.3;   // 피격 후 무적시간 0.3초
	m_bHit = false;
}

CMonster::~CMonster()
{
}
