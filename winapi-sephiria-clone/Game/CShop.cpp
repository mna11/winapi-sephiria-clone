#include "pch.h"
#include "CShop.h"

#include "CInventory.h"
#include "CButton.h"
#include "CMsgBox.h"

#include "CInventoryUI.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CArtifactData.h"
#include "CStoneTabletData.h"
#include "CImgMgr.h"
#include "CUIMgr.h"
#include "CCameraMgr.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CSoundMgr.h"

CShop::CShop()
	: m_pMsgBoxUI(nullptr), m_pEscapeButton(nullptr), m_dFrameTime(0.0), m_iFrame(0)
{
}

CShop::~CShop()
{
	Release();
}

void CShop::Initialize()
{
	Init_LoadImg(L"../Resource/Image/Shop/ShopBackground.png");
	Init_CreateObj();

	// 메세지 박스 렉트
	m_rcMsgBox = { (WINCX >> 1) - 200.f , (WINCY >> 1) - 100.f, 400.f, 200.f };
}

void CShop::Update()
{
	UpdateTime();

	if (nullptr == m_pMsgBoxUI)
	{
		CObjMgr::GetInstance()->UpdateOnly({ OBJID::UI, OBJID::MOUSE });

		// 인벤토리 업데이트
		CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
		if (nullptr == pPlayer || nullptr == pPlayer->GetInventory())
			return;
		pPlayer->GetInventory()->Update();
	}
	else
	{
		CObjMgr::GetInstance()->UpdateOnly({ OBJID::MOUSE });
		m_pMsgBoxUI->Update();
		for (auto btn : m_pMsgBoxUI->GetButtons())
			btn->Update();
	}
}

void CShop::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdateOnly({ OBJID::UI, OBJID::MOUSE });

	if (KEY_DOWN(VK_ESCAPE))
	{
		Release();
		CSceneMgr::GetInstance()->BackToSaveScene();
	}
}

void CShop::Render(Graphics* pGraphics)
{
	HDC hBackDC = pGraphics->GetHDC();

	RECT rc{ 0, 0, WINCX, WINCY };
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hBackDC, &rc, brush);
	DeleteObject(brush);

	VEC vCellSize = {256.f, 90.f};
	VEC vImgSize = vCellSize * PIXEL_SCALE;
	TransparentBlt(
		hBackDC,
		0,
		WINCY / 6,
		vImgSize.fX,
		vImgSize.fY,
		m_hMapDC,
		vCellSize.fX * m_iFrame,
		0,
		vCellSize.fX,
		vCellSize.fY,
		RGB(255, 0, 255)
	);

	pGraphics->ReleaseHDC(hBackDC);

	CObjMgr::GetInstance()->Render(pGraphics);
}

void CShop::Release()
{
	// 버튼 삭제 규칙
	if (nullptr != m_pEscapeButton)
	{
		m_pEscapeButton->SetDead(true);
		m_pEscapeButton = nullptr;
	}
	if (nullptr != m_pMsgBoxUI)
	{
		m_pMsgBoxUI->SetDead(true);
		m_pMsgBoxUI = nullptr;
	}
}

void CShop::UpdateTime()
{
	m_dFrameTime += DT;

	if (m_dFrameTime >= 0.05)
	{
		m_iFrame = (m_iFrame + 1) % 60;
		m_dFrameTime -= 0.05;
	}
}

void CShop::Init_CreateObj()
{
	// 버튼 등록
	m_pEscapeButton = CAbstractFactory<CButton>::CreateButton(
		CObjMgr::GetInstance()->GetMouse(),
		RectF{ WINCX - 70.f, 10.f, 21.f * PIXEL_SCALE * 0.5f, 33.f * PIXEL_SCALE * 0.5f },
		L"",
		L"EscapeButton",
		VEC{ 21.f, 33.f }
	);

	// 버튼 함수 등록
	m_pEscapeButton->SetOnClick([this]() {
		CSceneMgr::GetInstance()->BackToSaveScene();
		});
	CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pEscapeButton);
}

void CShop::TryBuyItem(int iInventoryIdx, int iID, ITEM_TYPE eItemType)
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	// 아이템 가격
	ITEM_INFO tItemInfo{};
	switch (eItemType)
	{
	case ITEM_TYPE::ARTIFACT:
		tItemInfo = CArtifactData::GetInstance()->FindItemInfo(iID);
		break;
	case ITEM_TYPE::STONE_TABLET:
		tItemInfo = CStoneTabletData::GetInstance()->FindItemInfo(iID);
		break;
	default:
		break;
	}
	// 맞는 아이템이 없음
	if (-1 == tItemInfo.iID)
		return; 

	// 돈이 있음
	if (pPlayer->GetLeaf() >= tItemInfo.iLeaf)
	{
		wstring wstr = tItemInfo.strName + L"을(를) " + to_wstring(tItemInfo.iLeaf) + L" 리프로\n 구매하시겠습니까?";

		m_pMsgBoxUI = CAbstractFactory<CMsgBox>::CreateMsgBox(
			m_rcMsgBox,
			2,
			MSG_BOX_LAYOUT::NORMAL,
			wstr
		);
		CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pMsgBoxUI);

		// 버튼 콜백 함수 등록
		auto vecButtons = m_pMsgBoxUI->GetButtons();
		vecButtons[0]->SetOnClick([this, iInventoryIdx, tItemInfo]()
			{
				CMsgBox* pConfirm = m_pMsgBoxUI;

				// 일단 화면에서 안보이게
				pConfirm->Hide();
				for (auto btn : pConfirm->GetButtons())
					btn->Hide();

				// 삭제 예약
				pConfirm->SetDead(true);
				m_pMsgBoxUI = nullptr;

				this->BuyItem(iInventoryIdx, tItemInfo);
			});
		vecButtons[0]->SetString(L"Yes");

		vecButtons[1]->SetOnClick([this]()
			{
				CMsgBox* pConfirm = m_pMsgBoxUI;
				// 일단 화면에서 안보이게
				pConfirm->Hide();
				for (auto btn : pConfirm->GetButtons())
					btn->Hide();

				pConfirm->SetDead(true);
				m_pMsgBoxUI = nullptr;
			});
		vecButtons[1]->SetString(L"No");
	}
	// 돈이 없음
	else
	{
		wstring wstr = L"리프가 부족합니다.";
		m_pMsgBoxUI = CAbstractFactory<CMsgBox>::CreateMsgBox(
			m_rcMsgBox,
			1,
			MSG_BOX_LAYOUT::NORMAL,
			wstr
		);
		CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pMsgBoxUI);

		// 버튼 콜백 함수 등록
		auto vecButtons = m_pMsgBoxUI->GetButtons();
		vecButtons[0]->SetOnClick([this, iInventoryIdx, iID]()
			{
				CMsgBox* pConfirm = m_pMsgBoxUI;
				// 일단 화면에서 안보이게
				pConfirm->Hide();
				for (auto btn : pConfirm->GetButtons())
					btn->Hide();

				pConfirm->SetDead(true);
				m_pMsgBoxUI = nullptr;
			});
		vecButtons[0]->SetString(L"Close");
	}
}

void CShop::BuyItem(int iInventoryIdx, ITEM_INFO tItemInfo)
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	if (pPlayer->GetInventory()->InsertItem(iInventoryIdx, tItemInfo.iID, tItemInfo.eItemType))
	{
		pPlayer->AddLeaf(-tItemInfo.iLeaf);
		// 사운드 
		CSoundMgr::GetInstance()->PlaySound(L"ShopBuy.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else
	{
		wstring wstr = L"인벤토리 공간이 부족합니다.";
		m_pMsgBoxUI = CAbstractFactory<CMsgBox>::CreateMsgBox(
			m_rcMsgBox,
			1,
			MSG_BOX_LAYOUT::NORMAL,
			wstr
		);
		CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pMsgBoxUI);

		// 버튼 콜백 함수 등록
		auto vecButtons = m_pMsgBoxUI->GetButtons();
		vecButtons[0]->SetOnClick([this]()
			{
				CMsgBox* pConfirm = m_pMsgBoxUI;
				// 일단 화면에서 안보이게
				pConfirm->Hide();
				for (auto btn : pConfirm->GetButtons())
					btn->Hide();

				pConfirm->SetDead(true);
				m_pMsgBoxUI = nullptr;
			});
		vecButtons[0]->SetString(L"Close");
	}
}

void CShop::TrySellItem(int iInventoryIdx, int iID, ITEM_TYPE eItemType)
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	// 아이템 가격
	ITEM_INFO tItemInfo{};
	switch (eItemType)
	{
	case ITEM_TYPE::ARTIFACT:
		tItemInfo = CArtifactData::GetInstance()->FindItemInfo(iID);
		break;
	case ITEM_TYPE::STONE_TABLET:
		tItemInfo = CStoneTabletData::GetInstance()->FindItemInfo(iID);
		break;
	default:
		break;
	}
	// 맞는 아이템이 없음
	if (-1 == tItemInfo.iID)
		return;

	wstring wstr = tItemInfo.strName + L"을(를) " + to_wstring(static_cast<int>(tItemInfo.iLeaf * 0.5)) + L" 리프에\n 판매하시겠습니까?";

	m_pMsgBoxUI = CAbstractFactory<CMsgBox>::CreateMsgBox(
		m_rcMsgBox,
		2,
		MSG_BOX_LAYOUT::NORMAL,
		wstr
	);
	CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pMsgBoxUI);
	// 버튼 콜백 함수 등록
	auto vecButtons = m_pMsgBoxUI->GetButtons();
	vecButtons[0]->SetOnClick([this, iInventoryIdx, tItemInfo]()
		{
			CMsgBox* pConfirm = m_pMsgBoxUI;
			// 일단 화면에서 안보이게
			pConfirm->Hide();
			for (auto btn : pConfirm->GetButtons())
				btn->Hide();

			pConfirm->SetDead(true);
			m_pMsgBoxUI = nullptr;

			this->SellItem(iInventoryIdx, tItemInfo);
		});
	vecButtons[0]->SetString(L"Yes");

	vecButtons[1]->SetOnClick([this]()
		{
			CMsgBox* pConfirm = m_pMsgBoxUI;
			// 일단 화면에서 안보이게
			pConfirm->Hide();
			for (auto btn : pConfirm->GetButtons())
				btn->Hide();

			pConfirm->SetDead(true);
			m_pMsgBoxUI = nullptr;
		});
	vecButtons[1]->SetString(L"No");
}

void CShop::SellItem(int iInventoryIdx, ITEM_INFO tItemInfo)
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	pPlayer->GetInventory()->EraseItem(iInventoryIdx);
	static_cast<CInventoryUI*>(CUIMgr::GetInstance()->GetUI(UIID::INVENTORY))->SyncInventorySlot();
	pPlayer->AddLeaf(tItemInfo.iLeaf * 0.5f);
	// 사운드 
	CSoundMgr::GetInstance()->PlaySound(L"ShopBuy.wav", CHANNEL_GROUPID::SFX, 1.f);
}