#include "pch.h"
#include "CStart.h"

#include "CButton.h"
#include "CMouse.h"
#include "CFade.h"

#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CSoundMgr.h"
#include "CUIMgr.h"
#include "CImgMgr.h"

CStart::CStart()
	: CState(START_SCENE_STATE::END, START_SCENE_STATE::HORAY),
	m_pStartBtn(nullptr), m_pExitBtn(nullptr), m_pMouse(nullptr),
	m_dStateTime(0.), m_dHorayTime(0.)
{
	ZeroMemory(&m_tTreeFrame, sizeof(FRAME));
}

CStart::~CStart()
{
	Release();
}

void CStart::Initialize()
{
	Release();

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Start/StartTree.png", L"StartTree");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Start/TEAMHORAY.png", L"TeamHoray");

	Init_LoadImg(L"../Resource/Image/Start/StartBgwLogo.png");
	Init_CreateObj();

	m_dHorayTime = 4.; // 4초
}

void CStart::Update()
{
	ApplyChange();

	m_dStateTime += DT;
	if (m_eCurState == START_SCENE_STATE::HORAY && m_dStateTime >= m_dHorayTime)
		RequestChange(START_SCENE_STATE::ANIMATION);

	CObjMgr::GetInstance()->Update();

	UpdateFrame();
}

void CStart::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();
}

void CStart::Render(Graphics* pGraphics)
{
	HDC hBackDC = pGraphics->GetHDC();

	// 처음에 두둥탁하고 팀 후레이 나오기
	// 그 다음에 씨앗 뿌리기
	// 그 다음에 띄우기
	// 종료시 종료
	// 게임 시작시 Stage 0로 전환
	
	// 배경 검정색으로 칠하기
	RECT rc{ 0, 0, WINCX, WINCY };
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hBackDC, &rc, brush);
	DeleteObject(brush);
	pGraphics->ReleaseHDC(hBackDC);

	switch (m_eCurState)
	{
	case START_SCENE_STATE::HORAY:
		HandleHorayRender(pGraphics);
		break;
	case START_SCENE_STATE::ANIMATION:
		HandleAnimationRender(pGraphics);
		break;
	case START_SCENE_STATE::START:
		HandleStartRender(pGraphics);
		break;
	default:
		break;
	}
}

void CStart::Release()
{
	// 버튼 삭제 규칙
	if (nullptr != m_pStartBtn)
	{
		m_pStartBtn->SetDead(true);
		m_pStartBtn = nullptr;
	}
	if (nullptr != m_pExitBtn)
	{
		m_pExitBtn->SetDead(true);
		m_pExitBtn = nullptr;
	}
}

void CStart::Init_CreateObj()
{
	// 마우스 UI 상태로 변경
	// 만약 없다면 만들어줌
	m_pMouse = CObjMgr::GetInstance()->GetMouse();
	if (nullptr == m_pMouse)
	{
		CMouse* m_pMouse = static_cast<CMouse*>(CAbstractFactory<CMouse>::CreateObj());
		CObjMgr::GetInstance()->AddObject(OBJID::MOUSE, m_pMouse);
		CUIMgr::GetInstance()->SetMouse(m_pMouse);
	}
	m_pMouse->RequestChange(MOUSE_STATE::UI_IDLE);

	// 버튼 등록
	VEC vOffset{};
	RectF{};
	float fGap = 150.f;
	VEC vCellSize{ 64.f, 20.f };
	VEC vImgSize = vCellSize * 2.5f;

	// 시작 버튼 위치
	vOffset = { (WINCX - (vImgSize.fX * 2 + fGap)) * 0.5f + vImgSize.fX * 0.5f, 550.f };
	RectF DestRect = { vOffset.fX - vImgSize.fX * 0.5f, vOffset.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };
	m_pStartBtn = CAbstractFactory<CButton>::CreateButton(
		CObjMgr::GetInstance()->GetMouse(),
		DestRect,
		{},
		L"GameStartButton",
		vCellSize
	);

	// 시작 버튼 위치
	vOffset.fX += vImgSize.fX + fGap;
	DestRect = { vOffset.fX - vImgSize.fX * 0.5f, vOffset.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };
	m_pExitBtn = CAbstractFactory<CButton>::CreateButton(
		CObjMgr::GetInstance()->GetMouse(),
		DestRect,
		{},
		L"GameExitButton",
		vCellSize
	);

	// 버튼 함수 등록
	m_pStartBtn->SetOnClick([this]() {
		CSceneMgr::GetInstance()->RequestChange(SCENEID::STAGE1);
		m_pMouse->RequestChange(MOUSE_STATE::COMBAT);
	});

	m_pExitBtn->SetOnClick([this]() {
		DestroyWindow(g_hWnd);
	});

	CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pStartBtn);
	CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pExitBtn);

	m_pStartBtn->Hide();
	m_pExitBtn->Hide();
}

void CStart::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case START_SCENE_STATE::HORAY:
			CSoundMgr::GetInstance()->PlaySound(L"StartIntroSound.wav", CHANNEL_GROUPID::SFX, 1.f);
			break;
		case START_SCENE_STATE::ANIMATION:
			CSoundMgr::GetInstance()->PlaySound(L"StartSeed.wav", CHANNEL_GROUPID::SFX, 1.f);
			SetFrame(0, 32, 0, 0.1);
			break;
		case START_SCENE_STATE::START:
			Init_BGM(L"StartTheme.wav", 0.3f);
			SetFrame(33, 47, 0, 0.1);
			m_pStartBtn->Show();
			m_pExitBtn->Show();
			break;
		default:
			break;
		}

		m_eCurState = m_eNextState;
	}
}

void CStart::HandleHorayRender(Graphics* pGraphics)
{
	VEC vOffset{ WINCX >> 1, WINCY >> 1 };
	VEC vCellSize{45.f, 42.f};
	VEC vImgSize = vCellSize * PIXEL_SCALE;
	RectF DestRect = { vOffset.fX - vImgSize.fX * 0.5f, vOffset.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };

	Image* pImg = CImgMgr::GetInstance()->FindImg(L"TeamHoray");
	if (nullptr == pImg)
		return; 

	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY,
		UnitPixel
	);
}

void CStart::HandleAnimationRender(Graphics* pGraphics)
{
	VEC vOffset = VEC{ 320.f, 94.f } *2.f;
	VEC vCellSize{ 147.f, 155.f };
	VEC vImgSize = vCellSize * PIXEL_SCALE * 0.4f;
	RectF DestRect = { vOffset.fX - vImgSize.fX * 0.5f, vOffset.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };

	Image* pImg = CImgMgr::GetInstance()->FindImg(L"StartTree");
	if (nullptr == pImg)
		return;

	pGraphics->DrawImage(
		pImg, DestRect,
		vCellSize.fX * m_tTreeFrame.iStart, 
		0, 
		vCellSize.fX, vCellSize.fY,
		UnitPixel
	);
}

void CStart::HandleStartRender(Graphics* pGraphics)
{
	HDC hBackDC = pGraphics->GetHDC();

	VEC vCellSize = { 640.f, 360.f };
	VEC vImgSize = { WINCX, WINCY };
	TransparentBlt(
		hBackDC,
		0,
		0,
		vImgSize.fX,
		vImgSize.fY,
		m_hMapDC,
		0,
		0,
		vCellSize.fX,
		vCellSize.fY,
		RGB(255, 0, 255)
	);

	pGraphics->ReleaseHDC(hBackDC);

	VEC vOffset = VEC{ 320.f, 94.f } * 2.f;
	vCellSize = { 147.f, 155.f };
	vImgSize = vCellSize * PIXEL_SCALE * 0.4f;
	RectF DestRect = { vOffset.fX - vImgSize.fX * 0.5f, vOffset.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };

	Image* pImg = CImgMgr::GetInstance()->FindImg(L"StartTree");
	if (nullptr == pImg)
		return;

	pGraphics->DrawImage(
		pImg, DestRect,
		vCellSize.fX * m_tTreeFrame.iStart,
		0,
		vCellSize.fX, vCellSize.fY,
		UnitPixel
	);


	CObjMgr::GetInstance()->Render(pGraphics);
}

void CStart::SetFrame(int iStart, int iEnd, int iMotion, double dFrameSpeed)
{
	m_tTreeFrame = { iStart, iEnd, iMotion, dFrameSpeed, 0. };
}

void CStart::UpdateFrame()
{
	m_tTreeFrame.dFrameElapsedTime += DT;
	if (m_tTreeFrame.dFrameSpeed <= m_tTreeFrame.dFrameElapsedTime)
	{
		++m_tTreeFrame.iStart;
		m_tTreeFrame.dFrameElapsedTime -= m_tTreeFrame.dFrameSpeed;

		if (m_tTreeFrame.iStart > m_tTreeFrame.iEnd)
		{
			if (m_eCurState == START_SCENE_STATE::ANIMATION)
			{
				RequestChange(START_SCENE_STATE::START);
				CUI* pFade = CUIMgr::GetInstance()->ShowUI(UIID::FADE);
				if (nullptr != pFade)
					static_cast<CFade*>(pFade)->SetFadeType(FADE_TYPE::RECTANGLE);
			}

			m_tTreeFrame.iStart = 33;
		}
	}
}