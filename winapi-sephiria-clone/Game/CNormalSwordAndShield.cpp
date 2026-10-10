#include "pch.h"
#include "CNormalSwordAndShield.h"

#include "CNormalSword.h"
#include "CNormalShield.h"
#include "CPlayer.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"
#include "CSoundMgr.h"
#include "CEffectMgr.h"

CNormalSwordAndShield::CNormalSwordAndShield()
{
}

CNormalSwordAndShield::~CNormalSwordAndShield()
{
	Release();
}

void CNormalSwordAndShield::Initialize()
{
	m_iID = 0;

	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 0; // 아무거나 줘도 됨, 어차피 한손검은 직접 렌더 안함
	// 실드는 플레이어 앞에 있어야 하고, 검은 뒤에 있어야 해서
	// 실드랑 소드를 따로 생성해서 ObjMgr에 넣을거임

	// 검과 방패 생성 후 초기화 작업
	// 이후 ObjMgr에 넣어주기
	m_pSword = CAbstractFactory<CNormalSword>::CreateSwordAndShield(&m_eCurState, &m_tAtk);
	m_pShield = CAbstractFactory<CNormalShield>::CreateSwordAndShield(&m_eCurState, &m_tAtk);
	CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pSword);
	CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pShield);

	// 공격 정보 초기화
	// 최대 3연격 가능, 공격 걸리는 시간 1초
	// 공격 상태 처음 진입 시 초기화하긴 하지만, 방어적으로 코드 작성
	SetAtk(0, 3, 0., 0.2, 1.0);

	m_eWeaponType = WEAPON_TYPE::SWORD_AND_SHIELD;
	m_iID = 0;

	m_iMpCost = 30;
}

void CNormalSwordAndShield::Render(Graphics* pGraphics)
{
	// 소드, 실드에서 각자 렌더링함

#ifdef _DEBUG
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
	SolidBrush blackBrush(Color(255, 0, 0, 0));

	// 공격 충돌 RECT
	pGraphics->FillRectangle(&blackBrush, (int)(m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::ATTACK)].left + vScroll.fX),
		(int)(m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::ATTACK)].top + vScroll.fY),
		(int)m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::ATTACK)].right - m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::ATTACK)].left,
		(int)m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::ATTACK)].bottom - m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::ATTACK)].top);

	// 방어 충돌 RECT
	pGraphics->FillRectangle(&blackBrush, (int)(m_vecDefRect[toUType(SWORD_AND_SHIELD_DEF_RECT::DEFENSE)].left + vScroll.fX),
		(int)(m_vecDefRect[toUType(SWORD_AND_SHIELD_DEF_RECT::DEFENSE)].top + vScroll.fY),
		(int)m_vecDefRect[toUType(SWORD_AND_SHIELD_DEF_RECT::DEFENSE)].right - m_vecDefRect[toUType(SWORD_AND_SHIELD_DEF_RECT::DEFENSE)].left,
		(int)m_vecDefRect[toUType(SWORD_AND_SHIELD_DEF_RECT::DEFENSE)].bottom - m_vecDefRect[toUType(SWORD_AND_SHIELD_DEF_RECT::DEFENSE)].top);

	// 회전 충돌 RECT
	pGraphics->FillRectangle(&blackBrush, (int)(m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::CLEAVE)].left + vScroll.fX),
		(int)(m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::CLEAVE)].top + vScroll.fY),
		(int)m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::CLEAVE)].right - m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::CLEAVE)].left,
		(int)m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::CLEAVE)].bottom - m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::CLEAVE)].top);

#endif // _DEBUG
}

void CNormalSwordAndShield::CreateEffect()
{
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
			CEffectMgr::GetInstance()->CreateEffect(L"SwordSwing1", vRenderPoint, EFTMGR_IMAGE | EFTMGR_FIXED, m_pTarget->GetAngle());
			vRectSize = { 50.f, 50.f };
			break;
		case 1:
			CEffectMgr::GetInstance()->CreateEffect(L"SwordSwing2", vRenderPoint, EFTMGR_IMAGE | EFTMGR_FIXED, m_pTarget->GetAngle());
			vRectSize = { 60.f, 60.f };
			break;
		case 2:
			CEffectMgr::GetInstance()->CreateEffect(L"SwordSwing3", vRenderPoint, EFTMGR_IMAGE | EFTMGR_FIXED, m_pTarget->GetAngle());
			vRectSize = { 80.f, 80.f };
			break;
		}
	}
	SetRect(&m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::ATTACK)], vRenderPoint.fX - vRectSize.fX, vRenderPoint.fY - vRectSize.fY, vRenderPoint.fX + vRectSize.fX, vRenderPoint.fY + vRectSize.fY);

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
	vRenderPoint = { 0.f, 0.f };
	vRectSize = { 0.f, 0.f };
	if (m_eCurState == SWORD_AND_SHIELD_STATE::CLEAVE)
	{
		float fTargetAngle = m_pTarget->GetAngle();
		float fDistance = 60.f;
		VEC vOffset{ fDistance * cosf(fTargetAngle), fDistance * sinf(fTargetAngle) };
		vRenderPoint = m_tInfo.vPoint;
		float fSizeFactor = 130.f;
		vRectSize = { fSizeFactor + fSizeFactor * fabsf(cosf(m_fAngle)), fSizeFactor + fSizeFactor * fabsf(sinf(m_fAngle)) };
	}
	SetRect(&m_vecAtkRect[toUType(SWORD_AND_SHIELD_ATK_RECT::CLEAVE)], vRenderPoint.fX - vRectSize.fX, vRenderPoint.fY - vRectSize.fY, vRenderPoint.fX + vRectSize.fX, vRenderPoint.fY + vRectSize.fY);
}


void CNormalSwordAndShield::ApplyChange()
{
	if (nullptr == m_pTarget)
		return;
	double dAtkSpeed = m_pTarget->GetStat().fAttackSpeed;
	dAtkSpeed = clamp(dAtkSpeed, 0.1, 5.0);
	double dComboTime = max(0.5, 0.2 / dAtkSpeed + 0.15);

	// 상태 첫 진입
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case SWORD_AND_SHIELD_STATE::IDLE:
			SetAtk(0, 0, 0., 0., 0.);
			break;
		case SWORD_AND_SHIELD_STATE::ATTACK:
			SetAtk(1, 3, 0., 0.2 / dAtkSpeed, dComboTime); // 공격 처음 진입 시 초기화
			break;
		case SWORD_AND_SHIELD_STATE::DEFENSE:
			break;
		case SWORD_AND_SHIELD_STATE::CLEAVE_READY:
			SetAtk(0, 0, 0., 0.2, 0.5);
			break;
		case SWORD_AND_SHIELD_STATE::CLEAVE:
			SetAtk(0, 0, 0., 0.01, 0.01); 
			CEffectMgr::GetInstance()->CreateEffect(L"Cleave", m_tInfo.vPoint, EFTMGR_IMAGE | EFTMGR_FIXED, m_pTarget->GetAngle());
			break;
		default:
			break;
		}
		m_eCurState = m_eNextState;
	}
}
