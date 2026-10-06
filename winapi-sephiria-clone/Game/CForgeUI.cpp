#include "pch.h"

#include "CForgeUI.h"
#include "CForgeSlotUI.h"

#include "CPlayer.h"

#include "CForge.h"

#include "CSceneMgr.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"
#include "CWeaponData.h"
#include "CKeyMgr.h"
#include "CUIMgr.h"
#include "CAbstractFactory.h"

CForgeUI::CForgeUI()
{
	// 일단 무기 데이터에 있는 개수만큼 자리를 마련해둠
	m_vecWeaponSlot.resize(CWeaponData::GetInstance()->GetMapSize(), nullptr);
}

CForgeUI::~CForgeUI()
{
	Release();
}

void CForgeUI::Initialize()
{
	m_tInfo = { WINCX >> 1, WINCY >> 1, 0.f, 0.f };
	m_fUIScale = PIXEL_SCALE * 0.5f;
	
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 0;
}

int CForgeUI::Update()
{
	if (m_bDead)
		return DEAD;

	if (!m_bView)
		return NOEVENT;

	for (auto& weaponSlot : m_vecWeaponSlot)
	{
		if (nullptr != weaponSlot)
			weaponSlot->Update();
	}

	HandleCollisionMouse();

	return NOEVENT;
}

void CForgeUI::LateUpdate()
{
	if (!m_bView)
		return;

	for (auto& weaponSlot : m_vecWeaponSlot)
	{
		if (nullptr != weaponSlot)
			weaponSlot->LateUpdate();
	}
}

void CForgeUI::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	for (auto& weaponSlot : m_vecWeaponSlot)
	{
		if (nullptr != weaponSlot)
			weaponSlot->Render(pGraphics);
	}
}

void CForgeUI::Release()
{
	for_each(m_vecWeaponSlot.begin(), m_vecWeaponSlot.end(), SafeDelete<CForgeSlotUI*>);
	m_vecWeaponSlot.clear();
	m_vecWeaponSlot.shrink_to_fit();
}

void CForgeUI::Show()
{
	m_bView = true;

	Initialize();
	UpdateWeaponList();
}

void CForgeUI::UpdateWeaponList()
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return; 

	int iCnt(0);
	for (int i = 0; i < CWeaponData::GetInstance()->GetMapSize(); ++i)
	{
		const WEAPON_INFO* pWeaponInfo = CWeaponData::GetInstance()->FindWeaponInfo(i);
		if (nullptr == pWeaponInfo)
			continue;

		// 현재 플레이어가 착용하고 있는 무기와 같은 종류만 리스트에 넣을 것이다.
		if (pPlayer->GetWeaponController()->GetWeapon()->GetWeaponType() != pWeaponInfo->eWeaponType)
			continue; 

		auto& weaponSlot = m_vecWeaponSlot[i];
		if (weaponSlot == nullptr)
		{
			float fGapY = 84.f * m_fUIScale;
			weaponSlot = static_cast<CForgeSlotUI*>(CAbstractFactory<CForgeSlotUI>::CreateUI(m_tInfo.vPoint.fX, WINCY / 4 + fGapY * (iCnt++) + fGapY * 0.5f, m_pMouse));
		}
		weaponSlot->SetWeaponID(i);
	}
}

void CForgeUI::HandleCollisionMouse()
{
	int iMouseHoverSlot = CCollisionMgr::GetCollisionSlotIndex<CForgeSlotUI>(m_pMouse->GetInfo().vPoint, m_vecWeaponSlot);
	for (int i = 0; i < m_vecWeaponSlot.size(); ++i)
	{
		if (m_vecWeaponSlot[i] == nullptr)
			continue;

		bool bCol = (i == iMouseHoverSlot && -1 != m_vecWeaponSlot[i]->GetWeaponID());
		m_vecWeaponSlot[i]->SetCollide(bCol);
	}

	if (KEY_DOWN(VK_LBUTTON) && -1 != iMouseHoverSlot)
	{
		static_cast<CForge*>(CSceneMgr::GetInstance()->GetCurrentScene())->TryChangeWeapon(iMouseHoverSlot);
	}
}