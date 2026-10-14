#include "pch.h"
#include "CScoreBoard.h"

#include "CPlayer.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CSceneMgr.h"
#include "CFontMgr.h"

CScoreBoard::CScoreBoard()
	: m_pRetryButton(nullptr)
{
}

CScoreBoard::~CScoreBoard()
{
	Release();
}

void CScoreBoard::Initialize()
{
	Init_CreateObj();
}

void CScoreBoard::Update()
{
	CObjMgr::GetInstance()->Update();
}

void CScoreBoard::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();
}

void CScoreBoard::Render(Graphics* pGraphics)
{
	// 배경 검정색으로 칠하기
	HDC hBackDC = pGraphics->GetHDC();

	RECT rc{ 0, 0, WINCX, WINCY };
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hBackDC, &rc, brush);
	DeleteObject(brush);
	pGraphics->ReleaseHDC(hBackDC);

	// 클리어 문구 띄우기
	VEC vRectSize{ 200.f, 50.f };
	VEC vOffset{ WINCX >> 1, WINCY >> 2 };
	RectF DestRect{ vOffset.fX - vRectSize.fX * 0.5f, vOffset.fY - vRectSize.fY * 0.5f, vRectSize.fX, vRectSize.fY };
	CFontMgr::GetInstance()->DrawString(pGraphics, L"클리어", FONT_TYPE::NORMAL, DestRect, { 255, 36, 191, 255 }, 32.f);

	// 생존 시간 띄우기

	// 장소 띄우기

	// 가한 총 데미지

	// 처치한 몬스터 수

	CObjMgr::GetInstance()->Render(pGraphics);
}

void CScoreBoard::Release()
{
}

void CScoreBoard::Init_CreateObj()
{
	if (nullptr == CObjMgr::GetInstance()->GetPlayer())
	{
		CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(WINCX, WINCY));
	}
	else
		CObjMgr::GetInstance()->GetPlayer()->SetPos({ WINCX, WINCY });

	// 버튼 등록
	m_pRetryButton = CAbstractFactory<CButton>::CreateButton(
		CObjMgr::GetInstance()->GetMouse(),
		RectF{ WINCX >> 1, WINCY - WINCY >> 2, 21.f * PIXEL_SCALE * 0.5f, 33.f * PIXEL_SCALE * 0.5f },
		L"돌아가기"
	);

	// 버튼 함수 등록
	m_pRetryButton->SetOnClick([this]() {
		CSceneMgr::GetInstance()->RequestNodeChange(1, SCENEID::START);
	});
	CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pRetryButton);
}
