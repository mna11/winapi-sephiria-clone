#include "pch.h"
#include "CMonster.h"

#include "CDrop.h"

#include "CAbstractFactory.h"
#include "CCameraMgr.h"
#include "CObjMgr.h"
#include "CSoundMgr.h"
#include "CEffectMgr.h"

CMonster::CMonster()
	: m_iLeaf(0), m_iExp(0)
{
	// 몬스터의 타겟은 항상 플레이어
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
	m_dIframeTime = 0.1;   // 피격 후 무적시간 0.1초
	m_bHit = false;
}

CMonster::~CMonster()
{
}

// 몬스터들이 HitDamage를 받으면 호출하는 공통 함수
void CMonster::HitDamage(int iDamage, CObj* pObj, HIT_SOURCE eHit, bool bCritical)
{
	if (!ApplyDamage(iDamage, eHit))
		return;

	CreateHitEffect(iDamage, pObj, eHit, bCritical);

	// 사망 처리
	if (m_tStat.iHp <= 0)
	{
		// 몬스터를 Down 상태로 변경
		this->Down();

		if (nullptr == pObj)
			return;

		// 경험치와 리프 드랍
		int iExp = m_iExp * pObj->GetStat().fExperienceDrop;
		int iLeaf = m_iLeaf * pObj->GetStat().fLeafDrop;
		if (iExp > 0)	CObjMgr::GetInstance()->AddObject(OBJID::DROP, CAbstractFactory<CDrop>::CreateDrop(m_tInfo.vPoint.fX - m_tInfo.vSize.fX * 0.3f, m_tInfo.vPoint.fY + 20, pObj, DROP_TYPE::EXP, iExp));
		if (iLeaf > 0) CObjMgr::GetInstance()->AddObject(OBJID::DROP, CAbstractFactory<CDrop>::CreateDrop(m_tInfo.vPoint.fX + m_tInfo.vSize.fX * 0.3f, m_tInfo.vPoint.fY + 20, pObj, DROP_TYPE::LEAF, iLeaf));

		// 플레이어 킬 수 올리기
		static_cast<CPlayer*>(pObj)->AddKill(1);
	}
}

bool CMonster::ApplyDamage(int iDamage, HIT_SOURCE eHit)
{
	// 죽거나 죽을 예정이면 PASS
	if (m_bDead || m_tStat.iHp <= 0)
		return false;

	// 현재 Hit 상태인데(무적 시간 중), 만약 총알에 의한 충돌이면 이건 데미지를 받음
	if (m_bHit && eHit != HIT_SOURCE::BULLET)
		return false;

	// 무적 시간 플래그
	m_bHit = true;
	m_dHitElapseTime = 0;
	// 메인 - 체력 감소
	m_tStat.iHp = clamp(m_tStat.iHp - iDamage, 0, m_tStat.iMaxHp);

	return true;
}

void CMonster::CreateHitEffect(int iDamage, CObj* pObj, HIT_SOURCE eHit, bool bCritical)
{
	if (nullptr == pObj)
		return;

	// 공격 데미지 이펙트 생성
	Color DamageColor = bCritical ? Color{ 255, 255, 210, 60 } : Color{ 255, 255, 255, 255 };
	CEffectMgr::GetInstance()->CreateEffect(L"", m_tInfo.vPoint, EFTMGR_STRING | EFTMGR_MOVE | EFTMGR_SCROLL, 100.f, 0.5f, nullptr, VEC{ 1.f, -1.f }.Normalize(), to_wstring(iDamage), DamageColor);

	// 피격 이펙트 생성
	VEC vDir = (pObj->GetInfo().vPoint - m_tInfo.vPoint).Normalize();
	float fTargetAngle = atan2f(vDir.fY, vDir.fX);
	switch (eHit)
	{
	case HIT_SOURCE::SLASH:
		CEffectMgr::GetInstance()->CreateEffect(L"Attack_Slash", m_tInfo.vPoint, 0, fTargetAngle, 0.05);
		break;
	case HIT_SOURCE::BULLET:
		CEffectMgr::GetInstance()->CreateEffect(L"Attack_Bullet", m_tInfo.vPoint, 0, fTargetAngle, 0.05);
		break;
	}

	return;
}
