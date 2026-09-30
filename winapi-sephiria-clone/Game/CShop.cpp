#include "pch.h"
#include "CShop.h"

#include "CEscapeButton.h"

#include "CObjMgr.h"
#include "CImgMgr.h"
#include "CUIMgr.h"
#include "CCameraMgr.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

CShop::CShop()
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
}

void CShop::Update()
{
	m_dFrameTime += DT;

	if (m_dFrameTime >= 0.05)
	{
		m_iFrame = (m_iFrame + 1) % 60;
		m_dFrameTime -= 0.05;
	}

	CObjMgr::GetInstance()->UpdateOnly({ OBJID::UI, OBJID::MOUSE });
}

void CShop::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdateOnly({ OBJID::UI, OBJID::MOUSE });

	if (KEY_DOWN(VK_ESCAPE) 
		|| static_cast<CEscapeButton*>(CUIMgr::GetInstance()->GetUI(UIID::ESCAPE_BUTTON))->GetMouseCollide()
			&& KEY_DOWN(VK_LBUTTON))
	{
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
}

void CShop::Init_CreateObj()
{
}
