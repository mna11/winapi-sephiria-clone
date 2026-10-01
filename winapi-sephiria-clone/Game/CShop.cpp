#include "pch.h"
#include "CShop.h"

#include "CInventory.h"
#include "CButton.h"
#include "CMsgBox.h"

#include "CInventoryUI.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CItemData.h"
#include "CImgMgr.h"
#include "CUIMgr.h"
#include "CCameraMgr.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

CShop::CShop()
	: m_pMsgBoxUI(nullptr), m_pEscapeButton(nullptr)
{
}

CShop::~CShop()
{
	Release();
}

void CShop::Initialize()
{
	//CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Stage/ShopBackground.png", L"Shop_BG");
	Init_LoadImg(L"../Resource/Image/Shop/ShopBackground.png");
	Init_CreateObj();

	// 메세지 박스 렉트
	m_rcMsgBox = { (WINCX >> 1) - 200.f , (WINCY >> 1) - 100.f, 400.f, 200.f };
}

void CShop::Update()
{
	m_dFrameTime += DT;

	if (m_dFrameTime >= 0.05)
	{
		m_iFrame = (m_iFrame + 1) % 60;
		m_dFrameTime -= 0.05;
	}

	if (nullptr == m_pMsgBoxUI)
	{
		CObjMgr::GetInstance()->UpdateOnly({ OBJID::UI, OBJID::MOUSE });
	}
	else
	{
		CObjMgr::GetInstance()->UpdateOnly({ OBJID::MOUSE });
		m_pMsgBoxUI->Update();
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

	VEC vCellSize = {256.f, 90.f};
	VEC vImgSize = vCellSize * PIXEL_SCALE;
	TransparentBlt(
		hBackDC,
		0,
		160,
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

void CShop::TryBuyItem(int iInventoryIdx, int iID)
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	// 상품 가격
	const ITEM_INFO* pItemInfo = CItemData::GetInstance()->FindItemInfo(iID);
	if (nullptr == pItemInfo)
		return;
	int iPrice = CItemData::GetInstance()->FindItemInfo(iID)->iLeaf;

	// 돈이 있음
	if (pPlayer->GetLeaf() >= iPrice)
	{
		wstring wstr = pItemInfo->strName + L"을(를) " + to_wstring(pItemInfo->iLeaf) + L" 리프로 구매하시겠습니까?";

		m_pMsgBoxUI = CAbstractFactory<CMsgBox>::CreateMsgBox(
			m_rcMsgBox,
			2,
			MSG_BOX_LAYOUT::NORMAL,
			wstr
		);
		CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pMsgBoxUI);

		// 버튼 콜백 함수 등록
		auto vecButtons = m_pMsgBoxUI->GetButtons();
		vecButtons[0]->SetOnClick([this, iInventoryIdx, iID, iPrice]()
			{
				this->BuyItem(iInventoryIdx, iID, iPrice);
				m_pMsgBoxUI->SetDead(true);
				m_pMsgBoxUI = nullptr;
			});
		vecButtons[0]->SetString(L"Yes");

		vecButtons[1]->SetOnClick([this]()
			{
				m_pMsgBoxUI->SetDead(true);
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
				m_pMsgBoxUI->SetDead(true);
				m_pMsgBoxUI = nullptr;
			});
		vecButtons[0]->SetString(L"Close");
	}
}

void CShop::BuyItem(int iInventoryIdx, int iID, int iPrice)
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	pPlayer->GetInventory()->InsertItem(iInventoryIdx, iID);
	pPlayer->AddLeaf(-iPrice);
}

void CShop::TrySellItem(int iInventoryIdx, int iID)
{
	const ITEM_INFO* pItemInfo = CItemData::GetInstance()->FindItemInfo(iID);

	wstring wstr = pItemInfo->strName + L"을(를) " + to_wstring(static_cast<int>(pItemInfo->iLeaf * 0.5)) + L" 리프에 판매하시겠습니까?";

	m_pMsgBoxUI = CAbstractFactory<CMsgBox>::CreateMsgBox(
		m_rcMsgBox,
		2,
		MSG_BOX_LAYOUT::NORMAL,
		wstr
	);
	CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pMsgBoxUI);
	int iPrice = pItemInfo->iLeaf;
	// 버튼 콜백 함수 등록
	auto vecButtons = m_pMsgBoxUI->GetButtons();
	vecButtons[0]->SetOnClick([this, iInventoryIdx, iPrice]()
		{
			this->SellItem(iInventoryIdx, iPrice);
			m_pMsgBoxUI->SetDead(true);
			m_pMsgBoxUI = nullptr;
		});
	vecButtons[0]->SetString(L"Yes");

	vecButtons[1]->SetOnClick([this]()
		{
			m_pMsgBoxUI->SetDead(true);
			m_pMsgBoxUI = nullptr;
		});
	vecButtons[1]->SetString(L"No");
}

void CShop::SellItem(int iInventoryIdx, int iPrice)
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	pPlayer->GetInventory()->EraseItem(iInventoryIdx);
	static_cast<CInventoryUI*>(CUIMgr::GetInstance()->GetUI(UIID::INVENTORY))->SyncInventorySlot();
	pPlayer->AddLeaf(iPrice * 0.5f);
}