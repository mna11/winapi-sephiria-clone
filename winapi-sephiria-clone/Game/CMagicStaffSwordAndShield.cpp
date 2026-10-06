#include "pch.h"
#include "CMagicStaffSwordAndShield.h"

#include "CBullet.h"

#include "CMagicStaffSword.h"
#include "CMagicStaffShield.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"
#include "CSoundMgr.h"
#include "CEffectMgr.h"

CMagicStaffSwordAndShield::CMagicStaffSwordAndShield()
{
}

CMagicStaffSwordAndShield::~CMagicStaffSwordAndShield()
{
}

void CMagicStaffSwordAndShield::Initialize()
{
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 0; // 아무거나 줘도 됨, 어차피 한손검은 직접 렌더 안함
	// 실드는 플레이어 앞에 있어야 하고, 검은 뒤에 있어야 해서
	// 실드랑 소드를 따로 생성해서 ObjMgr에 넣을거임

	// 검과 방패 생성 후 초기화 작업
	// 이후 ObjMgr에 넣어주기
	m_pSword = CAbstractFactory<CMagicStaffSword>::CreateSwordAndShield(&m_eCurState, &m_tAtk);
	m_pShield = CAbstractFactory<CMagicStaffShield>::CreateSwordAndShield(&m_eCurState, &m_tAtk);
	CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pSword);
	CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pShield);

	// 공격 정보 초기화
	// 최대 3연격 가능, 공격 걸리는 시간 1초
	// 공격 상태 처음 진입 시 초기화하긴 하지만, 방어적으로 코드 작성
	SetAtk(0, 3, 0., 0.2, 1.0);
}

void CMagicStaffSwordAndShield::Render(Graphics* pGraphics)
{
}

void CMagicStaffSwordAndShield::CreateEffect()
{
	if (nullptr == m_pTarget)
		return;

	// 총알 공격력
	int iFireAtk = m_pTarget->GetStat().iFireAtk;

	VEC vRenderPoint{};
	VEC vRectSize{};

	float fTargetAngle = m_pTarget->GetAngle();
	float fDistance = 50.f;
	VEC vOffset{ fDistance * cosf(fTargetAngle), fDistance * sinf(fTargetAngle) };

	// 공격
	if (KEY_DOWN(VK_LBUTTON) && m_eCurState != SWORD_AND_SHIELD_STATE::DEFENSE)
	{
		float fTargetAngle = m_pTarget->GetAngle();
		float fDistance = 50.f;
		VEC vOffset{ fDistance * cosf(fTargetAngle), fDistance * sinf(fTargetAngle) };

		vRenderPoint = m_tInfo.vPoint + vOffset;



		switch (m_tAtk.iLevel)
		{
		case 0:
			CEffectMgr::GetInstance()->CreateEffect(L"MagicWandSwing1", vRenderPoint, EFTMGR_IMAGE | EFTMGR_FIXED, m_pTarget->GetAngle());
			CObjMgr::GetInstance()->AddObject(OBJID::PLAYER_BULLET, CAbstractFactory<CBullet>::CreateBullet(vRenderPoint, L"FireBullet", fTargetAngle, 500.f, iFireAtk, m_pTarget));
			break;
		case 1:
			CEffectMgr::GetInstance()->CreateEffect(L"MagicWandSwing2", vRenderPoint, EFTMGR_IMAGE | EFTMGR_FIXED, m_pTarget->GetAngle());
			CObjMgr::GetInstance()->AddObject(OBJID::PLAYER_BULLET, CAbstractFactory<CBullet>::CreateBullet(vRenderPoint, L"FireBullet", fTargetAngle, 500.f, iFireAtk, m_pTarget));
			vRenderPoint -= vOffset * 0.5f;
			CObjMgr::GetInstance()->AddObject(OBJID::PLAYER_BULLET, CAbstractFactory<CBullet>::CreateBullet(vRenderPoint, L"FireBullet", fTargetAngle, 500.f, iFireAtk, m_pTarget));
			break;
		case 2:
			CEffectMgr::GetInstance()->CreateEffect(L"MagicWandSwing1", vRenderPoint, EFTMGR_IMAGE | EFTMGR_FIXED, m_pTarget->GetAngle());
			CObjMgr::GetInstance()->AddObject(OBJID::PLAYER_BULLET, CAbstractFactory<CBullet>::CreateBullet(vRenderPoint, L"FireBullet", fTargetAngle, 500.f, iFireAtk, m_pTarget));
			vRenderPoint -= vOffset * 0.5f;
			CObjMgr::GetInstance()->AddObject(OBJID::PLAYER_BULLET, CAbstractFactory<CBullet>::CreateBullet(vRenderPoint, L"FireBullet", fTargetAngle, 500.f, iFireAtk, m_pTarget));
			vRenderPoint -= vOffset * 0.5f;
			CObjMgr::GetInstance()->AddObject(OBJID::PLAYER_BULLET, CAbstractFactory<CBullet>::CreateBullet(vRenderPoint, L"FireBullet", fTargetAngle, 500.f, iFireAtk, m_pTarget));
			break;
		}
	}

	// 방어
	vRenderPoint = { 0.f, 0.f };
	vRectSize = { 0.f, 0.f };
	if (m_eCurState == SWORD_AND_SHIELD_STATE::DEFENSE)
	{
		float fTargetAngle = m_pTarget->GetAngle();
		float fDistance = 40.f;
		VEC vOffset{ fDistance * cosf(fTargetAngle), fDistance * sinf(fTargetAngle) };

		vRenderPoint = m_tInfo.vPoint + vOffset;
		float fSizeFactor = 27.f;
		vRectSize = { fSizeFactor + fSizeFactor * fabsf(sinf(fTargetAngle)), fSizeFactor + fSizeFactor * fabsf(cosf(fTargetAngle)) };

		CEffectMgr::GetInstance()->CreateEffect(L"Shield", vRenderPoint, EFTMGR_IMAGE | EFTMGR_FIXED, m_pTarget->GetAngle());

		vRenderPoint -= vOffset * 0.2f;
	}
	SetRect(&m_vecDefRect[toUType(SWORD_AND_SHIELD_DEF_RECT::DEFENSE)], vRenderPoint.fX - vRectSize.fX, vRenderPoint.fY - vRectSize.fY, vRenderPoint.fX + vRectSize.fX, vRenderPoint.fY + vRectSize.fY);

	// 회전 베기
	if (m_eCurState == SWORD_AND_SHIELD_STATE::CLEAVE)
	{
		vRenderPoint = m_tInfo.vPoint;
		CEffectMgr::GetInstance()->CreateEffect(L"MagicWandCleave", vRenderPoint, EFTMGR_IMAGE | EFTMGR_FIXED, m_pTarget->GetAngle());

		for (int i = 0; i < 8; ++i)
		{
			CObjMgr::GetInstance()->AddObject(OBJID::PLAYER_BULLET, CAbstractFactory<CBullet>::CreateBullet(vRenderPoint, L"FireBullet", 45 * i * PI / 180.f, 500.f, iFireAtk, m_pTarget ));
		}
	}
}

void CMagicStaffSwordAndShield::CleaveUpdate()
{
	if (m_eCurState != SWORD_AND_SHIELD_STATE::CLEAVE_READY
		&& m_eCurState != SWORD_AND_SHIELD_STATE::CLEAVE)
		return;

	m_tAtk.dElapseTime += DT;

	if (m_eCurState == SWORD_AND_SHIELD_STATE::CLEAVE_READY)
	{
		m_tAtk.dElapseTime = 0;
		m_eNextState = SWORD_AND_SHIELD_STATE::CLEAVE;
	}
	else
	{
		if (m_tAtk.dElapseTime > m_tAtk.dMaxTime)
		{
			m_tAtk.dElapseTime = 0;
			m_eNextState = SWORD_AND_SHIELD_STATE::IDLE;
			static_cast<CPlayer*>(m_pTarget)->RequestChange(PLAYER_STATE::IDLE);
		}
	}
}
