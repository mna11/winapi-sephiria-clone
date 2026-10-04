#include "pch.h"
#include "CDrop.h"

#include "CPlayer.h"

#include "CTimeMgr.h"
#include "CEffectMgr.h"
#include "CObjMgr.h"
#include "CImgMgr.h"
#include "CCameraMgr.h"
#include "CCollisionMgr.h"

CDrop::CDrop()
	: m_iAmount(0), m_eDropType(DROP_TYPE::END), m_bBattleEnd(false)
{
	ZeroMemory(&m_tInteractRect, sizeof(RECT));
}

CDrop::~CDrop()
{
	Release();
}

void CDrop::Initialize()
{
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Drop/Exp.png", L"Drop_Exp");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Drop/Leaf.png", L"Drop_Leaf");

	// 기초 정보 초기화
	m_tInfo = { 0.f, 0.f, 0.f, 0.f };
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 4; // 몬스터 바로 앞

	m_fSpeed = 350.f;
}

int CDrop::Update()
{
	if (m_bDead)
		return DEAD;

	UpdateInteractRect();

	__super::UpdateRect();
	__super::UpdateFrame();
	return NOEVENT;
}

void CDrop::LateUpdate()
{
	Magnet();
}

void CDrop::Render(Graphics* pGraphics)
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

void CDrop::Release()
{
}

void CDrop::SetDropType(DROP_TYPE eDropType)
{
	m_eDropType = eDropType;

	if (eDropType == DROP_TYPE::EXP)
	{
		m_tInfo.vSize = VEC{ 5.f, 5.f } * PIXEL_SCALE;
		m_pFrameKey = L"Drop_Exp";
		SetFrame(0, 11, 0, 0.1);
	}
	else if (eDropType == DROP_TYPE::LEAF)
	{
		m_tInfo.vSize = VEC{ 5.f, 5.f } *PIXEL_SCALE;
		m_pFrameKey = L"Drop_Leaf";
		SetFrame(0, 11, 0, 0.1);
	}
}

void CDrop::Magnet()
{
	if (nullptr == m_pTarget)
		return;

	// 플레이어가 일정 거리에 접근한 경우, 플레이어 쪽으로 빨려 간다.
	if (m_bBattleEnd || CCollisionMgr::CollisionRect(m_pTarget->GetRect(), m_tInteractRect))
	{
		VEC vDir = (m_pTarget->GetInfo().vPoint - m_tInfo.vPoint).Normalize();
		m_tInfo.vPoint += vDir * m_fSpeed * DT;
	}

	// 실제 플레이어와 닿는 경우
	if (CCollisionMgr::CollisionRect(m_pTarget->GetRect(), m_tRect))
	{
		if (DROP_TYPE::EXP == m_eDropType)
		{
			static_cast<CPlayer*>(m_pTarget)->AddExp(m_iAmount);
			CEffectMgr::GetInstance()->CreateEffect(L"Exp_Effect", VEC{ m_tInfo.vPoint.fX, m_tInfo.vPoint.fY }, EFTMGR_IMAGE | EFTMGR_FOLLOW | EFTMGR_SCROLL, 0.f, 0.1f, m_pTarget);
		}
		else if (DROP_TYPE::LEAF == m_eDropType)
		{
			static_cast<CPlayer*>(m_pTarget)->AddLeaf(m_iAmount);
			CEffectMgr::GetInstance()->CreateEffect(L"Leaf_Effect", VEC{ m_tInfo.vPoint.fX, m_tInfo.vPoint.fY }, EFTMGR_IMAGE | EFTMGR_FOLLOW | EFTMGR_SCROLL, 0.f, 0.1f, m_pTarget);
		}

		m_bDead = true;
	}
}

void CDrop::UpdateInteractRect()
{
	VEC vSize{ 200.f, 200.f };
	SetRect(&m_tInteractRect, m_tInfo.vPoint.fX - vSize.fX, m_tInfo.vPoint.fY - vSize.fY,
		m_tInfo.vPoint.fX + vSize.fX, m_tInfo.vPoint.fY + vSize.fY);
}