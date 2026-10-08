#include "pch.h"
#include "CGolemCow.h"

#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CTimeMgr.h"
#include "CImgMgr.h"
#include "CEffectMgr.h"
#include "CCameraMgr.h"
#include "CFontMgr.h"
#include "CObjMgr.h"
#include "CSoundMgr.h"

CGolemCow::CGolemCow()
    : CState(GOLEM_COW_STATE::END, GOLEM_COW_STATE::SUMMON),
	m_dChargeInterval(0.), m_dChargeReadyTime(0.), m_dDownTime(0.), m_dStateTime(0.), m_dSummonTime(0.), m_dDustInterval(0.)
{
    m_vecAtkRect.resize(toUType(GOLEM_COW_ATK_RECT::END));
	ZeroMemory(&m_vDir, sizeof(VEC));
}

CGolemCow::~CGolemCow()
{
    Release();
}

void CGolemCow::Initialize()
{
	// 기초 정보 초기화
	m_tInfo = { WINCX >> 1, WINCY >> 1, 125.f, 125.f };
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 4;
	m_fSpeed = 200.f;

	// 스프라이트 시트 Insert
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/GolemCow/Golem_Cow_LEFT.png", L"GolemCow_L");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/GolemCow/Golem_Cow_RIGHT.png", L"GolemCow_R");

	// 스프라이트 시트 선택
	m_pFrameKey = L"GolemCow_L";

	// 스탯 초기화
	m_tStat = { 200, 200, 30 };

	// 시간 초기화
	m_dStateTime = 0.;
	m_dSummonTime = 2.;
	m_dDownTime = 1.;

	m_dChargeReadyTime = 1.;
	m_dChargeInterval = 3.;

	m_dDustInterval = 0.3;
	m_dDustElapsedTime = 0.;
}

int CGolemCow::Update()
{
	if (m_bDead)
		return DEAD;

	m_vPrePoint = m_tInfo.vPoint;

	ApplyChange();
	UpdateTime();
	Move();
	Attack();

	__super::UpdateRect();
	__super::UpdateFrame();
    return NOEVENT;
}

void CGolemCow::LateUpdate()
{
}

void CGolemCow::Render(Graphics* pGraphics)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

#ifdef _DEBUG
	// 피격 충돌 박스 렌더링
	SolidBrush WhiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&WhiteBrush, (int)(m_tRect.left + vScroll.fX),
		(int)(m_tRect.top + vScroll.fY),
		(int)m_tInfo.vSize.fX,
		(int)m_tInfo.vSize.fY);

	// 공격 충돌 박스 렌더링
	SolidBrush BlackBrush(Color(255, 0, 0, 0));
	for (int i = 0; i < toUType(GOLEM_COW_ATK_RECT::END); ++i)
	{
		pGraphics->FillRectangle(&BlackBrush, (int)(m_vecAtkRect[i].left + vScroll.fX),
			(int)(m_vecAtkRect[i].top + vScroll.fY),
			(int)(m_vecAtkRect[i].right - m_vecAtkRect[i].left),
			(int)(m_vecAtkRect[i].bottom - m_vecAtkRect[i].top));
	}
#endif // _DEBUG

	Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

	VEC vCellSize{ 64.f, 64.f };
	VEC vImgSize = vCellSize * PIXEL_SCALE;

	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY,
					vImgSize.fX,
					vImgSize.fY };

	ImageAttributes* pImgAttr = m_bHit ? &m_imgAttrHit : nullptr;
	pImgAttr = m_eCurState == GOLEM_COW_STATE::DOWN ? &m_imgAttrDown : pImgAttr;
	pGraphics->DrawImage(
		pImg, rcDest,
		vCellSize.fX * m_tFrame.iStart,
		vCellSize.fY * m_tFrame.iMotion,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel,
		pImgAttr
	);
}

void CGolemCow::Release()
{
}

void CGolemCow::HitDamage(int iDamage, CObj* pObj, HIT_SOURCE eHit)
{
	if (m_bHit)
		return;

	if (m_eCurState == GOLEM_COW_STATE::DOWN || m_eCurState == GOLEM_COW_STATE::SUMMON)
		return;

	m_bHit = true;		// m_bHit은 m_bHit이 된지 경과한 시간이 iframeTime을 넘으면 false가 된다.

	m_tStat.iHp -= iDamage;

	if (m_tStat.iHp <= 0)
	{
		m_eNextState = GOLEM_COW_STATE::DOWN;
		CObjMgr::GetInstance()->AddObject(OBJID::DROP, CAbstractFactory<CDrop>::CreateDrop(m_tInfo.vPoint.fX - m_tInfo.vSize.fX * 0.3f, m_tInfo.vPoint.fY + 20, pObj, DROP_TYPE::EXP, 40));
		CObjMgr::GetInstance()->AddObject(OBJID::DROP, CAbstractFactory<CDrop>::CreateDrop(m_tInfo.vPoint.fX + m_tInfo.vSize.fX * 0.3f, m_tInfo.vPoint.fY + 20, pObj, DROP_TYPE::LEAF, 1000));
	}

	CEffectMgr::GetInstance()->CreateEffect(L"", m_tInfo.vPoint, EFTMGR_STRING | EFTMGR_MOVE | EFTMGR_SCROLL, 100.f, 0.5f, nullptr, VEC{ 1.f, -1.f }.Normalize(), to_wstring(iDamage), Color{ 255, 255, 255, 255 });

	// 사운드
	switch (eHit)
	{
	case HIT_SOURCE::SLASH:
		CSoundMgr::GetInstance()->PlaySound(L"HitSword02.wav", CHANNEL_GROUPID::SFX, 1.f);
		break;
	case HIT_SOURCE::BULLET:
		CSoundMgr::GetInstance()->PlaySound(L"HitMagicFire02.wav", CHANNEL_GROUPID::SFX, 1.f);
		break;
	}
}

void CGolemCow::OnWallCollision()
{
	// 돌진하다가 벽에 부딪힘
	if (m_eCurState == GOLEM_COW_STATE::CHARGE)
	{
		// 소리 + IDLE 상태 전환 + 카메라 흔들림

		RequestChange(GOLEM_COW_STATE::IDLE);
		SetRect(&m_vecAtkRect[toUType(GOLEM_COW_ATK_RECT::CHARGE)], 0, 0, 0, 0);
		CCameraMgr::GetInstance()->CameraShaking(8, 0.2);
		CSoundMgr::GetInstance()->PlaySound(L"GolemCowWallCollide.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
}

void CGolemCow::UpdateTime()
{
	m_dStateTime += DT;

	// 피해 유효 근거용 시간
	if (m_bHit)
	{
		m_dHitElapseTime += DT;

		if (m_dHitElapseTime >= m_dIframeTime)
		{
			m_bHit = false;
			m_dHitElapseTime -= m_dIframeTime;
		}
	}

	if (m_eNextState == GOLEM_COW_STATE::DOWN && m_eCurState != GOLEM_COW_STATE::DOWN)
		return;

	switch (m_eCurState)
	{
	case GOLEM_COW_STATE::SUMMON:
		if (m_dStateTime >= m_dSummonTime)
			RequestChange(GOLEM_COW_STATE::IDLE);
		break;

	case GOLEM_COW_STATE::IDLE:
		if (m_dStateTime >= m_dChargeInterval)
			RequestChange(GOLEM_COW_STATE::READY);
		break;
	case GOLEM_COW_STATE::WALK:
		m_fSpeed = 200.f;
		if (m_dStateTime >= m_dChargeInterval)
			RequestChange(GOLEM_COW_STATE::READY);
		break;
	case GOLEM_COW_STATE::READY:
		if (nullptr != m_pTarget)
			m_vDir = (m_pTarget->GetInfo().vPoint - m_tInfo.vPoint).Normalize();

		if (m_dStateTime >= m_dChargeReadyTime)
			RequestChange(GOLEM_COW_STATE::CHARGE);
		break;
	case GOLEM_COW_STATE::CHARGE:
		m_fSpeed = 600.f;
		m_dDustElapsedTime += DT;
		break;
	case GOLEM_COW_STATE::DOWN:
		SetRect(&m_vecAtkRect[toUType(GOLEM_COW_ATK_RECT::CHARGE)], 0, 0, 0, 0);
		if (m_dStateTime >= m_dDownTime)
		{
			m_bDead = true;
		}
		break;
	}
}

void CGolemCow::Move()
{
	if (nullptr == m_pTarget)
		return;

	VEC vDir = (m_pTarget->GetInfo().vPoint - m_tInfo.vPoint).Normalize();
	if (m_eCurState != GOLEM_COW_STATE::CHARGE)
	{
		if (vDir.fX <= 0) m_pFrameKey = L"GolemCow_L";
		else m_pFrameKey = L"GolemCow_R";
	}

	if (m_eCurState == GOLEM_COW_STATE::IDLE)
		RequestChange(GOLEM_COW_STATE::WALK);

	if (m_eCurState == GOLEM_COW_STATE::WALK)
		m_tInfo.vPoint += vDir * m_fSpeed * DT;

	if (m_eCurState == GOLEM_COW_STATE::CHARGE)
	{
		m_tInfo.vPoint += m_vDir * m_fSpeed * DT;

		if (m_dDustElapsedTime >= m_dDustInterval)
		{
			m_dDustElapsedTime -= m_dDustInterval;
			VEC vDir = m_tInfo.vPoint - m_vPrePoint;
			CEffectMgr::GetInstance()->CreateEffect(L"DashDust", m_tInfo.vPoint, EFTMGR_IMAGE | EFTMGR_FIXED, atan2f(vDir.fY, vDir.fX));
		}
	}
}

void CGolemCow::Attack()
{
	if (m_pTarget == nullptr)
		return;

	if (m_eCurState == GOLEM_COW_STATE::CHARGE)
		SetRect(&m_vecAtkRect[toUType(GOLEM_COW_ATK_RECT::CHARGE)], m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

}

void CGolemCow::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case GOLEM_COW_STATE::SUMMON:
			SetFrame(0, 22, 0, m_dSummonTime / 23.);
			break;
		case GOLEM_COW_STATE::IDLE:
			SetFrame(0, 5, 1, 0.2);
			break;
		case GOLEM_COW_STATE::WALK:
			SetFrame(0, 7, 2, 0.2);
			break;
		case GOLEM_COW_STATE::READY:
			SetFrame(0, 12, 3, m_dChargeReadyTime / 13.);
			break;
		case GOLEM_COW_STATE::CHARGE:
			m_dDustElapsedTime = 0.;
			SetFrame(0, 3, 4, 0.2);
			break;
		case GOLEM_COW_STATE::AIR:
			SetFrame(0, 3, 5, 0.2);
			break;
		case GOLEM_COW_STATE::DOWN:
			SetFrame(0, 0, 6, m_dDownTime);
			break;
		}

		m_eCurState = m_eNextState;
		m_dStateTime = 0.;
	}
}