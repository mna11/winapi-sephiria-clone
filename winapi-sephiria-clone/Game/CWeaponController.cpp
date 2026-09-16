#include "pch.h"
#include "CWeaponController.h"

#include "CSwordAndShield.h"

#include "CAbstractFactory.h"
#include "CImgMgr.h"
#include "CObjMgr.h"

CWeaponController::CWeaponController()
	: CState(CWeapon::TYPE::SwordAndShield, CWeapon::TYPE::NONE), m_pWeapon(nullptr)
{
}

CWeaponController::~CWeaponController()
{
	Release();
}

void CWeaponController::Initialize()
{
	// 한손검
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/Sword_Tier1.png", L"Sword_Tier1");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/Shield_Tier1.png", L"Shield_Tier1");

}

int CWeaponController::Update()
{
	ApplyChange();

	if (nullptr != m_pWeapon)
		m_pWeapon->Update();

	return NOEVENT;
}

void CWeaponController::LateUpdate()
{
	if (nullptr != m_pWeapon)
		m_pWeapon->LateUpdate();
}

void CWeaponController::Render(Graphics* pGraphics)
{
	// 렌더는 CObjMgr 담당 -> 렌더 ID에 따른 렌더 계층화를 위해
}

void CWeaponController::Release()
{
	SafeDelete<CWeapon*>(m_pWeapon);
}

void CWeaponController::Attack()
{
	if (nullptr != m_pWeapon)
		m_pWeapon->Attack();
}

void CWeaponController::SpecialAttack()
{
	if (nullptr != m_pWeapon)
		m_pWeapon->SpecialAttack();
}

void CWeaponController::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		SafeDelete<CWeapon*>(m_pWeapon);

		switch (m_eCurState)
		{
		case CWeapon::TYPE::NONE:
			CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pWeapon);
			break;
		case CWeapon::TYPE::SwordAndShield:
			// 한손검만 따로 Initialize 끝나고 지정해주는 이유는, 한손검 내부에서
			// Sword랑 Shield를 생성하고 있기 때문임
			// 추가로 한손검은 CObjMgr에 안넣는데
			// -> 이 또한 안에서 소드와 실드를 대신 넣고 있기 때문이다.
			// CSwrodAndShield 클래스는 소드와 실드를 조작하는 일종의 컨트롤러 클래스이다.
			m_pWeapon = CAbstractFactory<CSwordAndShield>::CreateWeapon();
			m_pWeapon->SetTarget(m_pTarget);
			break;
		case CWeapon::TYPE::Greatsword:
			CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pWeapon);
			break;
		case CWeapon::TYPE::Dagger:
			CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pWeapon);
			break;
		case CWeapon::TYPE::Crossbow:
			CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pWeapon);
			break;
		case CWeapon::TYPE::Katana:
			CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pWeapon);
			break;
		case CWeapon::TYPE::Staff:
			CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pWeapon);
			break;
		}
		
		m_eCurState = m_eNextState;
	}
}
