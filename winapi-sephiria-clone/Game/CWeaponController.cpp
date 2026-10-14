#include "pch.h"
#include "CWeaponController.h"

#include "CNormalSwordAndShield.h"
#include "CMagicStaffSwordAndShield.h"
#include "CReturningGuardian.h"
#include "CFishingRod.h"

#include "CAbstractFactory.h"
#include "CImgMgr.h"
#include "CObjMgr.h"

CWeaponController::CWeaponController()
	: CState(-1, 0), m_pWeapon(nullptr)
{
}

CWeaponController::~CWeaponController()
{
	Release();
}

void CWeaponController::Initialize()
{
}

int CWeaponController::Update()
{
	ApplyChange();

	return NOEVENT;
}

void CWeaponController::LateUpdate()
{
}

void CWeaponController::Render(Graphics* pGraphics)
{
	// 렌더는 CObjMgr 담당 -> 렌더 ID에 따른 렌더 계층화를 위해
}

void CWeaponController::Release()
{
	if (m_pWeapon != nullptr)
	{
		m_pWeapon->Release();
		m_pWeapon->SetDead(true);
		m_pWeapon = nullptr;
	}
}

void CWeaponController::Attack()
{
	if (nullptr != m_pWeapon)
		m_pWeapon->Attack();
}

void CWeaponController::SpecialAttack()
{
	if (nullptr != m_pWeapon)
	{
		m_pWeapon->SpecialAttack();
	}
}

void CWeaponController::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		if (nullptr != m_pWeapon)
		{
			m_pWeapon->Release();
			m_pWeapon->SetDead(true);
			m_pWeapon = nullptr;
		}

		switch (m_eNextState)
		{
		case -1:
			CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pWeapon);
			break;
		case 0:
			// 한손검만 따로 Initialize 끝나고 지정해주는 이유는, 한손검 내부에서
			// Sword랑 Shield를 생성하고 있기 때문임
			// 추가로 한손검은 CObjMgr에 안넣는데
			// -> 이 또한 안에서 소드와 실드를 대신 넣고 있기 때문이다.
			// CSwrodAndShield 클래스는 소드와 실드를 조작하는 일종의 컨트롤러 클래스이다.
			m_pWeapon = CAbstractFactory<CNormalSwordAndShield>::CreateWeapon();
			m_pWeapon->SetTarget(m_pTarget);
			CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pWeapon);
			m_iID = 0;
			break;
		case 1:
			m_pWeapon = CAbstractFactory<CMagicStaffSwordAndShield>::CreateWeapon();
			m_pWeapon->SetTarget(m_pTarget);
			CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pWeapon);
			m_iID = 1;
			break;
		case 2:
			m_pWeapon = CAbstractFactory<CReturningGuardian>::CreateWeapon();
			m_pWeapon->SetTarget(m_pTarget);
			CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pWeapon);
			m_iID = 2;
			break;
		case 3: // 일반 낚시대
			m_pWeapon = CAbstractFactory<CFishingRod>::CreateWeapon(m_pTarget);
			CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pWeapon);
			m_iID = 3;
			break;
		}
		
		m_eCurState = m_eNextState;
	}
}
