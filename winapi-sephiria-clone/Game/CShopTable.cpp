#include "pch.h"
#include "CShopTable.h"

#include "CMouse.h"
#include "CShopTableSlot.h"

#include "CAbstractFactory.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CUIMgr.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include "CFontMgr.h"

CShopTable::CShopTable()
	: m_iItemSlotSize(4), m_iMouseHoverSlot(-1), m_eLastStage(SCENEID::END)
{
	m_vecItemSlot.reserve(m_iItemSlotSize);
}

CShopTable::~CShopTable()
{
	Release();
}

void CShopTable::Initialize()
{
	m_tInfo = { 50.f, 14.f, 0.f, 0.f};

	m_eRender = RENDERID::UI;
	m_iRenderLayer = 3;
	m_fUIScale = PIXEL_SCALE * 0.5f;
}

int CShopTable::Update()
{
	if (!m_bView)
		return NOEVENT;
	if (m_bDead)
		return DEAD;

	for (auto& slot : m_vecItemSlot)
		slot->Update();

	// 마우스 포인터와 충돌하는 슬롯이 없는 경우에는 -1을 반환해줌
	m_iMouseHoverSlot = CCollisionMgr::GetCollisionSlotIndex<CShopTableSlot>(m_pMouse->GetInfo().vPoint, m_vecItemSlot);

	for (int i = 0; i < m_vecItemSlot.size(); ++i)
	{
		bool bCol = (i == m_iMouseHoverSlot && -1 != m_vecItemSlot[i]->GetItemID());
		m_vecItemSlot[i]->SetCollide(bCol);
	}

	// 마우스 호버 아이템 세팅
	if (-1 != m_iMouseHoverSlot)
	{
		m_pMouse->SetHoverReferItem({ m_vecItemSlot[m_iMouseHoverSlot]->GetItemID(), ITEM_SOURCE::SHOP });
		CUIMgr::GetInstance()->ShowUI(UIID::ITEM_TOOLTIP);

		VEC vPoint = m_vecItemSlot[m_iMouseHoverSlot]->GetInfo().vPoint;
		vPoint += VEC{ 144 * 0.5f, 0.f } * m_fUIScale;
		CUIMgr::GetInstance()->SetPos(UIID::ITEM_TOOLTIP, vPoint);
	}
	else
	{
		// 왜냐면 지금 인벤토리 업데이트 - 상점 업데이트 순서가 종속적인데, 여기서 가려버리면
		// 인벤토리에서 호버한 아이템의 툴팁이 나오지 않음
		//CUIMgr::GetInstance()->HideUI(UIID::ITEM_TOOLTIP);
	}

	// 드래그 시작
	if (KEY_DOWN(VK_LBUTTON) && -1 != m_iMouseHoverSlot)
	{
		// 마우스가 현재 드래그 중인 아이템을 참조하게 해줌 - 렌더용
		m_pMouse->SetDragReferItem({ m_vecItemSlot[m_iMouseHoverSlot]->GetItemID(), ITEM_SOURCE::SHOP });
	}

	return NOEVENT;
}

void CShopTable::LateUpdate()
{
	if (!m_bView)
		return;

	for (auto& slot : m_vecItemSlot)
		slot->LateUpdate();
}

void CShopTable::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	// 배경 그리기
	Image* pInventoryBaseImg = CImgMgr::GetInstance()->FindImg(L"ShopList");
	if (nullptr == pInventoryBaseImg)
		return;

	VEC vCellSize = { 196.f, 277.f };
	VEC vImgSize = vCellSize * m_fUIScale;
	RectF rcDest = { m_tInfo.vPoint.fX, m_tInfo.vPoint.fY, vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(pInventoryBaseImg, rcDest, 0, 0, vCellSize.fX, vCellSize.fY, UnitPixel);

	for (auto& slot : m_vecItemSlot)
		slot->Render(pGraphics);

	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	// 텍스트 출력
	wstring strLeafTitle	= L"플레이어 리프 보유량";
	wstring strLeaf			= to_wstring(pPlayer->GetLeaf());
	rcDest = {	m_tInfo.vPoint.fX + 25 * m_fUIScale, 
				m_tInfo.vPoint.fY + 200 * m_fUIScale, 
				100.f * m_fUIScale, 10.f * m_fUIScale };
#ifdef _DEBUG
SolidBrush BlackBrush(Color(255, 0, 0, 0));
pGraphics->FillRectangle(&BlackBrush, rcDest);
#endif // _DEBUG

	CFontMgr::GetInstance()->DrawString(pGraphics, strLeafTitle, FONT_TYPE::NORMAL, rcDest, Color(255, 255, 255, 255), 22.f, StringAlignmentNear, StringAlignmentNear);
	rcDest = { m_tInfo.vPoint.fX + 80 * m_fUIScale,
				m_tInfo.vPoint.fY + 210 * m_fUIScale,
				60.f * m_fUIScale, 40.f * m_fUIScale };
#ifdef _DEBUG
	pGraphics->FillRectangle(&BlackBrush, rcDest);
#endif // _DEBUG
	RectF rcBackStringDest = rcDest;
	rcBackStringDest.X += 4.f;
	rcBackStringDest.Y += 4.f;
	CFontMgr::GetInstance()->DrawString(pGraphics, strLeaf, FONT_TYPE::PIXEL_BIG, rcBackStringDest, Color(255, 0, 0, 0), 52.f, StringAlignmentFar, StringAlignmentCenter);
	CFontMgr::GetInstance()->DrawString(pGraphics, strLeaf, FONT_TYPE::PIXEL_BIG, rcDest, Color(255, 255, 255, 255), 52.f, StringAlignmentFar, StringAlignmentCenter);
	

	// 리프 이미지 출력 
	rcDest = { m_tInfo.vPoint.fX + 60 * m_fUIScale,
				m_tInfo.vPoint.fY + 215 * m_fUIScale,
				30.f * m_fUIScale, 30.f * m_fUIScale };
#ifdef _DEBUG
	pGraphics->FillRectangle(&BlackBrush, rcDest);
#endif // _DEBUG
	Image* pLeafImg = CImgMgr::GetInstance()->FindImg(L"Leaf");
	if (nullptr == pInventoryBaseImg)
		return;
	vCellSize = {10.f, 10.f};
	pGraphics->DrawImage(pLeafImg, rcDest, 0, 0, vCellSize.fX, vCellSize.fY, UnitPixel);
}

void CShopTable::Release()
{
	for_each(m_vecItemSlot.begin(), m_vecItemSlot.end(), SafeDelete<CShopTableSlot*>);
	m_vecItemSlot.clear();
	m_vecItemSlot.shrink_to_fit();
}

void CShopTable::SetSellingItem(SCENEID eCurStage)
{
	if (eCurStage == m_eLastStage)
		return;

	VEC vPoint = m_tInfo.vPoint + VEC{ 23.f + 144.f * 0.5f, 20.f + 42.f * 0.5f} * m_fUIScale;
	float fGap = 41.f * m_fUIScale;

	for (int i = 0; i < m_iItemSlotSize; ++i)
	{
		// 처음에는 push_back
		if (i == m_vecItemSlot.size())
			m_vecItemSlot.push_back(static_cast<CShopTableSlot*>(CAbstractFactory<CShopTableSlot>::CreateUI(vPoint.fX, vPoint.fY + fGap * i, m_pMouse)));
		// 랜덤 뽑기
		m_vecItemSlot[i]->SetItemID(uniform_int_distribution<int>(0, 8)(g_engine));
	}
	m_eLastStage = eCurStage;
}
