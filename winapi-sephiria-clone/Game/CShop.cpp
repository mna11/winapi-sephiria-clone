#include "pch.h"
#include "CShop.h"

#include "CInventory.h"
#include "CButton.h"

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

		// 근데 이게 뭐 때문에 열린 메세지 박스인지도 알아둬야 함
		// 일단 Buy만 생각한다면
		int iMsgBoxResult = m_pMsgBoxUI->Update();
		switch (iMsgBoxResult)
		{
		case 0:
			BuyItem();
			m_pMsgBoxUI->SetDead(true);
			m_pMsgBoxUI = nullptr;
			break;
		case 1:
		case 2:
			m_pMsgBoxUI->SetDead(true);
			m_pMsgBoxUI = nullptr;
			break;
		}
	}
}

void CShop::LateUpdate()
{
	if (nullptr == m_pMsgBoxUI)
	{
		CObjMgr::GetInstance()->LateUpdateOnly({ OBJID::UI, OBJID::MOUSE });

		if (KEY_DOWN(VK_ESCAPE))
		{
			CSceneMgr::GetInstance()->BackToSaveScene();
		}
	}
	else
	{
		m_pMsgBoxUI->LateUpdate();
		CObjMgr::GetInstance()->LateUpdateOnly({ OBJID::MOUSE });
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

	if (nullptr != m_pMsgBoxUI)
	{
		SolidBrush dimBrush(Color(128, 0, 0, 0));
		pGraphics->FillRectangle(&dimBrush, 0, 0, WINCX, WINCY);
	}

	CObjMgr::GetInstance()->Render(pGraphics);
}

void CShop::Release()
{
	// 버튼 삭제 규칙
	m_pEscapeButton->SetDead(true);
	m_pEscapeButton = nullptr;
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
	m_pEscapeButton->SetOnClick([]() {
		CSceneMgr::GetInstance()->BackToSaveScene();
		});
	CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pEscapeButton);
}

void CShop::TryBuyItem(int iInventoryIdx, int iID)
{
	CUIMgr::GetInstance()->ShowUI(UIID::MSG_BOX);
	// 메세지 박스 세팅
}

void CShop::BuyItem()
{
	//CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	//if (nullptr == pPlayer)
	//	return;

	//int iPrice = CItemData::GetInstance()->FindItemInfo(iID)->iLeaf;
	//if (pPlayer->GetLeaf() >= iPrice)
	//{
	//	pPlayer->GetInventory()->InsertItem(iInventoryIdx, iID);
	//	pPlayer->AddLeaf(-iPrice);
	//}
	//else
	//{
	//	// 돈이 부족합니다 ㅜㅜ 메세지 박스 띄우기
	//}

	// 초기화
	// 이것저것들
}

void CShop::TrySellItem(int iInventoryIdx)
{
	CUIMgr::GetInstance()->ShowUI(UIID::MSG_BOX);
	// 메세지 박스 세팅
}

void CShop::SellItem()
{
	/*CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	int iPrice = CItemData::GetInstance()->FindItemInfo(iID)->iLeaf;
	if (pPlayer->GetLeaf() >= iPrice)
	{
		pPlayer->GetInventory()->EraseItem(iInventoryIdx);
		pPlayer->AddLeaf(iPrice);
	}*/
}