#include "pch.h"
#include "CForge.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CItemData.h"
#include "CImgMgr.h"
#include "CUIMgr.h"
#include "CCameraMgr.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CSoundMgr.h"
#include "CFontMgr.h"

CForge::CForge()
	: m_iFrame(0), m_dFrameTime(0.0),
	m_pMsgBoxUI(nullptr), m_pEscapeButton(nullptr)
{
}

CForge::~CForge()
{
	Release();
}

void CForge::Initialize()
{
	Init_LoadImg(L"../Resource/Image/Forge/ForgeBackground.png");
	Init_CreateObj();

	// 메세지 박스 렉트
	m_rcMsgBox = { (WINCX >> 1) - 200.f , (WINCY >> 1) - 100.f, 400.f, 200.f };
}

void CForge::Update()
{
	UpdateTime();

	if (nullptr == m_pMsgBoxUI)
	{
		CObjMgr::GetInstance()->UpdateOnly({ OBJID::UI, OBJID::MOUSE });
	}
	else
	{
		CObjMgr::GetInstance()->UpdateOnly({ OBJID::MOUSE });
		m_pMsgBoxUI->Update();
		for (auto btn : m_pMsgBoxUI->GetButtons())
			btn->Update();
	}
}

void CForge::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdateOnly({ OBJID::UI, OBJID::MOUSE });

	if (KEY_DOWN(VK_ESCAPE))
	{
		Release();
		CSceneMgr::GetInstance()->BackToSaveScene();
	}
}

void CForge::Render(Graphics* pGraphics)
{
	HDC hBackDC = pGraphics->GetHDC();

	RECT rc{ 0, 0, WINCX, WINCY };
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hBackDC, &rc, brush);
	DeleteObject(brush);

	VEC vCellSize = { 256.f, 90.f };
	VEC vImgSize = vCellSize * PIXEL_SCALE;
	TransparentBlt(
		hBackDC,
		0,
		WINCY / 4,
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


	wstring strTitle = L"무기 강화";
	wstring strSubTitle = L"무기에 힘을 부여하여 새롭게 탄생시킵니다.";
	
	float fTitleHeight = 50.f;
	RectF rcTitle{ 0, 30.f, WINCX, fTitleHeight };
	float fGap = 10.f;
	float fSubTitleHeight = 50.f;
	RectF rcSubTitle{ 0, 30.f + fTitleHeight + fGap, WINCX, fSubTitleHeight };
	
#ifdef _DEBUG
	SolidBrush BlackBrush(Color(255, 0, 0, 0));
	pGraphics->FillRectangle(&BlackBrush, rcTitle);
	SolidBrush RedBrush(Color(255, 255, 0, 0));
	pGraphics->FillRectangle(&RedBrush, rcSubTitle);
#endif // _DEBUG

	CFontMgr::GetInstance()->DrawString(pGraphics, strTitle, FONT_TYPE::NORMAL, rcTitle, Color{255, 212, 79, 31}, 38.f);
	CFontMgr::GetInstance()->DrawString(pGraphics, strSubTitle, FONT_TYPE::NORMAL, rcSubTitle, Color{255, 255, 255, 255}, 24.f);
	//CFontMgr::GetInstance()->DrawString();

	CObjMgr::GetInstance()->Render(pGraphics);
}

void CForge::Release()
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

void CForge::Init_CreateObj()
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

void CForge::UpdateTime()
{
	m_dFrameTime += DT;

	if (m_dFrameTime >= 0.05)
	{
		m_iFrame = (m_iFrame + 1) % 60;
		m_dFrameTime -= 0.05;
	}
}