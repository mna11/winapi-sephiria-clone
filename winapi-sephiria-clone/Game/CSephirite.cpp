#include "pch.h"
#include "CSephirite.h"

#include "CPlayer.h"
#include "CItemSelectUI.h"
#include "CKeyGuide.h"

#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CUIMgr.h"
#include "CFontMgr.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CSoundMgr.h"

CSephirite::CSephirite()
	: m_bCol(false), m_eItemType(ITEM_TYPE::END)
{
	ZeroMemory(&m_tInteractRect, sizeof(RECT));
}

CSephirite::~CSephirite()
{
	Release();
}

void CSephirite::Initialize()
{
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Sephirite/Sephirite_Artifact.png", L"Sephirite_Artifact");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Sephirite/Sephirite_StoneTablet.png", L"Sephirite_StoneTablet");

	// 기초 정보 초기화
	m_tInfo = { 0.f, 0.f, 12.f * PIXEL_SCALE, 15.f * PIXEL_SCALE };
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 0;

	m_pFrameKey = L"Sephirite_Artifact";
	SetFrame(0, 19, 0, 0.2);
}

int CSephirite::Update()
{
	if (m_bDead)
		return DEAD;

	if (m_eItemType == ITEM_TYPE::END)
		return DEAD;

	UpdateInteractRect();

	HandleInteraction();

	__super::UpdateRect();
	__super::UpdateFrame();
	return NOEVENT;
}

void CSephirite::LateUpdate()
{
}

void CSephirite::Render(Graphics* pGraphics)
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

void CSephirite::Release()
{
	if (nullptr != m_pKeyGuide)
		m_pKeyGuide->HideGuide(this);
}

void CSephirite::UpdateInteractRect()
{
	VEC vSize{ 100.f, 100.f };
	SetRect(&m_tInteractRect, m_tInfo.vPoint.fX - vSize.fX, m_tInfo.vPoint.fY - vSize.fY,
		m_tInfo.vPoint.fX + vSize.fX, m_tInfo.vPoint.fY + vSize.fY);
}

void CSephirite::HandleInteraction()
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	m_pKeyGuide = static_cast<CKeyGuide*>(CUIMgr::GetInstance()->GetUI(UIID::KEY_GUIDE));

	if (CCollisionMgr::CollisionRect(pPlayer->GetRect(), m_tInteractRect))
	{
		if (!m_bCol)
		{
			m_pKeyGuide->ShowGuide(this);
			m_pKeyGuide->SetKeyGuideInfo('F', L"줍기", { 0, -m_tInfo.vSize.fY }, { 28.f, 10.f });
		}

		m_bCol = true;

		if (KEY_DOWN('F'))
		{
			m_pKeyGuide->HideGuide(this);
			m_bCol = false;
			CItemSelectUI* pItemSelectUI = static_cast<CItemSelectUI*>(CUIMgr::GetInstance()->ShowUI(UIID::ITEM_SELECT));
			pItemSelectUI->SetItemType(m_eItemType);
			m_bDead = true;
		}
	}
	else
	{
		m_pKeyGuide->HideGuide(this);
		m_bCol = false;
	}
}


void CSephirite::SetItemType(ITEM_TYPE eItemType)
{
	m_eItemType = eItemType;

	switch (m_eItemType)
	{
	case ITEM_TYPE::ARTIFACT:
		m_pFrameKey = L"Sephirite_Artifact";
		m_tInfo.vSize = { 12.f * PIXEL_SCALE, 15.f * PIXEL_SCALE };
		break;
	case ITEM_TYPE::STONE_TABLET:
		m_pFrameKey = L"Sephirite_StoneTablet";
		m_tInfo.vSize = { 17.f * PIXEL_SCALE, 16.f * PIXEL_SCALE };
		break;
	default:
		break;
	}
}