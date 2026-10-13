#include "pch.h"
#include "CAnvil.h"

#include "CKeyGuide.h"

#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CSoundMgr.h"
#include "CFontMgr.h"
#include "CCameraMgr.h"
#include "CSceneMgr.h"
#include "CUIMgr.h"

CAnvil::CAnvil()
	: m_bCol(false), m_bUse(false), m_pKeyGuide(nullptr)
{
	ZeroMemory(&m_tInteractRect, sizeof(RECT));
}

CAnvil::~CAnvil()
{
	Release();
}

void CAnvil::Initialize()
{
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Anvil/Anvil.png", L"Anvil");

	// 기초 정보 초기화
	m_tInfo = { 0.f, 0.f, 13.f * PIXEL_SCALE, 12.f * PIXEL_SCALE };
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 0;

	m_pFrameKey = L"Anvil";
}

int CAnvil::Update()
{
	HandleInteraction();

	UpdateInteractRect();

	__super::UpdateRect();
	return NOEVENT;
}

void CAnvil::LateUpdate()
{
}

void CAnvil::Render(Graphics* pGraphics)
{
	Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

#ifdef _DEBUG
	// 상호작용 렉트
	SolidBrush blackBrush(Color(255, 0, 0, 0));
	pGraphics->FillRectangle(&blackBrush, (int)(m_tInteractRect.left + vScroll.fX),
		(int)(m_tInteractRect.top + vScroll.fY),
		(int)(m_tInteractRect.right - m_tInteractRect.left),
		(int)(m_tInteractRect.bottom - m_tInteractRect.top)
	);
	// 충돌 렉트
	SolidBrush whiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&whiteBrush, (int)(m_tRect.left + vScroll.fX),
		(int)(m_tRect.top + vScroll.fY),
		(int)m_tInfo.vSize.fX,
		(int)m_tInfo.vSize.fY);
#endif // _DEBUG

	VEC vCellSize = m_tInfo.vSize / PIXEL_SCALE;
	VEC vImgSize = m_tInfo.vSize;
	RectF rcDest = { m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX,
					 m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY,
					 vImgSize.fX, vImgSize.fY };

	pGraphics->DrawImage(pImg, rcDest,
		vCellSize.fX * m_tFrame.iStart,
		vCellSize.fY * m_tFrame.iMotion,
		vCellSize.fX, vCellSize.fY, UnitPixel);
}

void CAnvil::Release()
{
}

void CAnvil::UpdateInteractRect()
{
	VEC vSize{ 100.f, 100.f };
	SetRect(&m_tInteractRect, m_tInfo.vPoint.fX - vSize.fX, m_tInfo.vPoint.fY - vSize.fY,
		m_tInfo.vPoint.fX + vSize.fX, m_tInfo.vPoint.fY + vSize.fY);
}

void CAnvil::HandleInteraction()
{
	if (m_bUse)
	{
		m_bCol = false;
		return;
	}

	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	m_pKeyGuide = static_cast<CKeyGuide*>(CUIMgr::GetInstance()->GetUI(UIID::KEY_GUIDE));

	if (CCollisionMgr::CollisionRect(pPlayer->GetRect(), m_tInteractRect))
	{
		if (!m_bCol)
		{
			m_pKeyGuide->ShowGuide(this);
			m_pKeyGuide->SetKeyGuideInfo('F', L"강화", { 0.f, -m_tInfo.vSize.fY }, { 28.f, 10.f });
		}

		m_bCol = true;

		if (KEY_DOWN('F'))
		{
			m_pKeyGuide->HideGuide(this);
			CSceneMgr::GetInstance()->RequestChange(SCENEID::FORGE);
			//CSoundMgr::GetInstance()->PlaySound(L"SephiriteOpen.wav", CHANNEL_GROUPID::SFX, 1.f);
			m_bCol = false;
			m_bUse = false;
		}
	}
	else
	{
		m_pKeyGuide->HideGuide(this);
		m_bCol = false;
	}
}
