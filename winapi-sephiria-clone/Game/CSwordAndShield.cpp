#include "pch.h"
#include "CSwordAndShield.h"

#include "CSword.h"
#include "CShield.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"

CSwordAndShield::CSwordAndShield()
	: CState(SWORD_AND_SHIELD_STATE::END, SWORD_AND_SHIELD_STATE::IDLE),
	m_pSword(nullptr), m_pShield(nullptr)
{
	ZeroMemory(m_tBarriorRect, sizeof(PointF) * 4); 
	ZeroMemory(&m_tSwordRect, sizeof(RECT));
}

CSwordAndShield::~CSwordAndShield()
{
	Release();
}

void CSwordAndShield::Initialize()
{
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 0; // 아무거나 줘도 됨, 어차피 한손검은 직접 렌더 안함
						// 실드는 플레이어 앞에 있어야 하고, 검은 뒤에 있어야 해서
						// 실드랑 소드를 따로 생성해서 ObjMgr에 넣을거임

	m_pSword = CAbstractFactory<CSword>::CreateObj();
	m_pShield = CAbstractFactory<CShield>::CreateObj();
	CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pSword);
	CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pShield);
}

int CSwordAndShield::Update()
{
	m_tInfo.vPoint = m_pTarget->GetInfo().vPoint;

	ApplyChange();

	m_pSword->Update();
	m_pShield->Update();

	switch (m_eCurState)
	{
	case SWORD_AND_SHIELD_STATE::IDLE:
		HandleIdleUpdate();
		break;
	case SWORD_AND_SHIELD_STATE::ATTACK_1:
		HandleAttack1Update();
		break;
	case SWORD_AND_SHIELD_STATE::ATTACK_2:
		HandleAttack2Update();
		break;
	case SWORD_AND_SHIELD_STATE::ATTACK_3:
		HandleAttack3Update();
		break;
	case SWORD_AND_SHIELD_STATE::SHIELD:
		HandleShieldUpdate();
		break;
	case SWORD_AND_SHIELD_STATE::CLEAVE:
		HandleCleaveUpdate();
		break;
	default:
		break;
	}

	return NOEVENT;
}

void CSwordAndShield::LateUpdate()
{
	m_pSword->LateUpdate();
	m_pShield->LateUpdate();
}

void CSwordAndShield::Render(Graphics* pGraphics)
{
	// 소드, 실드에서 각자 렌더링함
}

void CSwordAndShield::Release()
{
	// 다른 무기와 다르게 소드와 실드를 따로 생성하므로
	// 소멸할 때 정리해줘야 함
	CObjMgr::GetInstance()->DeleteID(OBJID::WEAPON);
}

void CSwordAndShield::SetTarget(CObj* pObj)
{
	m_pTarget = pObj;
	m_pSword->SetTarget(pObj);
	m_pShield->SetTarget(pObj);
}

void CSwordAndShield::Attack()
{
}

void CSwordAndShield::SpecialAttack()
{
}

void CSwordAndShield::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		default:
			break;
		}

		m_eCurState = m_eNextState;
	}
}

void CSwordAndShield::HandleIdleUpdate()
{
	if (nullptr == m_pTarget)
		return;

	float fTargetAngle = m_pTarget->GetAngle();

	VEC vOffset = m_pTarget->GetInfo().vSize * 0.3f;
	// 4사분면 - 여기서 분면 기준은 카테시안 좌표계 / 카테시안 좌표계 기준 마우스를 4사분면에 놓았을 때
	if (fTargetAngle >= 0.f && fTargetAngle < PI * 0.5f ) 
	{
		m_pShield->SetPos(static_cast<float>(m_pTarget->GetRect().left), m_tInfo.vPoint.fY + vOffset.fY);
		m_pSword->SetPos(static_cast<float>(m_pTarget->GetRect().right), m_tInfo.vPoint.fY - vOffset.fY);
	}
	// 3사분면 
	else if (fTargetAngle >= PI * 0.5f && fTargetAngle < PI )
	{
		m_pShield->SetPos(static_cast<float>(m_pTarget->GetRect().right), m_tInfo.vPoint.fY + vOffset.fY);
		m_pSword->SetPos(static_cast<float>(m_pTarget->GetRect().left), m_tInfo.vPoint.fY - vOffset.fY);
		
	}
	// 2사분면
	else if (fTargetAngle >= -PI && fTargetAngle < -PI * 0.5f)
	{
		m_pShield->SetPos(static_cast<float>(m_pTarget->GetRect().right) - vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY);
		m_pSword->SetPos(static_cast<float>(m_pTarget->GetRect().left), m_tInfo.vPoint.fY - vOffset.fY);
	}
	// 1사분면
	else 
	{
		m_pShield->SetPos(static_cast<float>(m_pTarget->GetRect().left) + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY);
		m_pSword->SetPos(static_cast<float>(m_pTarget->GetRect().right), m_tInfo.vPoint.fY - vOffset.fY);
	}
}

void CSwordAndShield::HandleAttack1Update()
{
}

void CSwordAndShield::HandleAttack2Update()
{
}

void CSwordAndShield::HandleAttack3Update()
{
}

void CSwordAndShield::HandleShieldUpdate()
{
}

void CSwordAndShield::HandleCleaveUpdate()
{
}
