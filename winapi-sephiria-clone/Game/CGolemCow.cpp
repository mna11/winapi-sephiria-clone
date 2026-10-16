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
	m_dChargeInterval(0.), m_dChargeReadyTime(0.), m_dDownTime(0.), m_dStateTime(0.), m_dSummonTime(0.), m_dDustInterval(0.), m_dDustElapsedTime(0.),
	m_pFootStep(nullptr)
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
	float fOffset(0.f);
	if (m_bHit && m_dIframeTime > 0.)
	{
		// 살짝 위로 길어졌다가 돌아오기
		float fRatio = clamp(static_cast<float>(m_dHitElapseTime / m_dIframeTime), 0.f, 1.f);
		float fAmplitude = vCellSize.fY * 0.5f * PIXEL_SCALE;
		float fDecay = (1.f - fRatio) * (1.f - fRatio);

		fOffset = -fAmplitude * sinf(fRatio * PI) * fDecay;
	}
	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX - fOffset * 0.5f,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY + fOffset,
					vImgSize.fX + fOffset,
					vImgSize.fY - fOffset };

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
	if (m_pFootStep != nullptr)
	{
		m_pFootStep->stop();
		m_pFootStep = nullptr;
	}
}

void CGolemCow::HitDamage(int iDamage, CObj* pObj, HIT_SOURCE eHit, bool bCritical)
{
	// 공격 상태와 사망 상태에서는 데미지를 받지 않는다.
	if (m_eCurState == GOLEM_COW_STATE::DOWN || m_eCurState == GOLEM_COW_STATE::SUMMON)
		return;

	CMonster::HitDamage(iDamage, pObj, eHit, bCritical);
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

		// 발소리 끄기
		if (m_pFootStep != nullptr)
		{
			m_pFootStep->stop();
			m_pFootStep = nullptr;
		}
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

void CGolemCow::Down()
{
	m_eNextState = GOLEM_COW_STATE::DOWN;
}

void CGolemCow::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case GOLEM_COW_STATE::SUMMON:
			CSoundMgr::GetInstance()->PlaySound(L"MonsterSummon1.wav", CHANNEL_GROUPID::SFX, 1.f);
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
			CSoundMgr::GetInstance()->PlaySound(L"GolemCowAttackReady.wav", CHANNEL_GROUPID::SFX, 1.f, 1.f);
			
			break;
		case GOLEM_COW_STATE::CHARGE:
			m_dDustElapsedTime = 0.;
			if (m_pFootStep == nullptr)
				m_pFootStep = CSoundMgr::GetInstance()->PlayLoopSFX(L"GolemCowFootStep.wav", 5.f, 1.f);
			else if (m_pFootStep != nullptr)
			{
				m_pFootStep->stop();
				m_pFootStep = nullptr;
			}
			SetFrame(0, 3, 4, 0.2);
			break;
		case GOLEM_COW_STATE::AIR:
			SetFrame(0, 3, 5, 0.2);
			break;
		case GOLEM_COW_STATE::DOWN:
			SetFrame(0, 0, 6, m_dDownTime);
			// 충돌 렉트 끄기
			m_tInfo.vSize = { 0.f, 0.f };	
			// 달리다가 죽을 수도 있으니깐 발소리 끄기
			if (m_pFootStep != nullptr)
			{
				m_pFootStep->stop();
				m_pFootStep = nullptr;
			}
			break;
		}

		m_eCurState = m_eNextState;
		m_dStateTime = 0.;
	}
}
