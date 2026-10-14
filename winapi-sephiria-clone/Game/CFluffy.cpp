#include "pch.h"
#include "CFluffy.h"

#include "CBullet.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CTimeMgr.h"
#include "CEffectMgr.h"
#include "CSoundMgr.h"


CFluffy::CFluffy()
	: CState(FLUFFY_STATE::END, FLUFFY_STATE::SUMMON),
	m_dStateTime(0.), m_dSummonTime(0.), m_dAtkTime(0.), m_dDownTime(0.), m_dAtkElapse(0.), m_dAtkCycle(0.), m_dStunTime(0.)
{
}

CFluffy::~CFluffy()
{
	Release();
}

void CFluffy::Initialize()
{
	m_tInfo = { 0.f, 0.f, 100.f, 100.f };
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 1;
	m_fSpeed = 150.f;

	// 스프라이트 시트 Insert 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Fluffy/Ghost_Fluffy_LEFT.png", L"Fluffy_L");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Fluffy/Ghost_Fluffy_RIGHT.png", L"Fluffy_R");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Fluffy/Ghost_Fluffy_Summon.png", L"Fluffy_Summon");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Fluffy/Fluffy_Bullet.png", L"Fluffy_Bullet");

	// 애니메이션 프레임 초기화
	m_pFrameKey = L"Fluffy_Summon";
	SetFrame(0, 13, 0, 0.2);

	// HP 100, ATK 10
	m_tStat = { 
		100, 
		100, 
		10 
	};

	m_dStateTime = 0.;
	m_dSummonTime = 2.;
	m_dDownTime = 1.;

	m_dAtkTime = 1.;
	m_dAtkCycle = 4.;
	m_dAtkElapse = 3.;

	m_dStunTime = 0.3;
}

int CFluffy::Update()
{
	if (m_bDead)
		return DEAD;

	m_vPrePoint = m_tInfo.vPoint;
	
	// 상태 변경
	ApplyChange();
	// 시간 업데이트
	UpdateTime();
	// 이동
	Move();
	// 공격
	Attack();

	UpdateFrame();
	__super::UpdateRect();

	return NOEVENT;
}

void CFluffy::LateUpdate()
{
}

void CFluffy::Render(Graphics* pGraphics)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

#ifdef _DEBUG
	SolidBrush WhiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&WhiteBrush, (int)(m_tRect.left + vScroll.fX),
		(int)(m_tRect.top + vScroll.fY),
		(int)m_tInfo.vSize.fX,
		(int)m_tInfo.vSize.fY);
#endif // _DEBUG

	Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

	VEC vCellSize{};
	if (!lstrcmp(m_pFrameKey, L"Fluffy_L") || !lstrcmp(m_pFrameKey, L"Fluffy_R"))
		vCellSize = { 23.f, 21.f };
	else if (!lstrcmp(m_pFrameKey, L"Fluffy_Summon"))
		vCellSize = { 65.f, 35.f };
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
	pImgAttr = m_eCurState == FLUFFY_STATE::DOWN ? &m_imgAttrDown : pImgAttr;
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

void CFluffy::Release()
{
}

void CFluffy::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case FLUFFY_STATE::SUMMON:
			CSoundMgr::GetInstance()->PlaySound(L"FluffySummon.wav", CHANNEL_GROUPID::SFX, 5.f, 1.f);
			SetFrame(0, 13, 0, m_dSummonTime / 14.);				// 프레임 키가 다름 - Fluffy_Summon
			break;
		case FLUFFY_STATE::IDLE:
			m_pFrameKey = L"Fluffy_L";
			SetFrame(0, 6, 0, 0.2);
			break;
		case FLUFFY_STATE::WALK:
			SetFrame(0, 6, 1, 0.2);
			break;
		case FLUFFY_STATE::ATK:
		{
			SetFrame(0, 9, 2, m_dAtkTime / 10.);

			if (nullptr == m_pTarget)
				return;

			VEC vDir = (m_pTarget->GetInfo().vPoint - m_tInfo.vPoint).Normalize();
			float fTargetAngle = atan2f(vDir.fY, vDir.fX);
			VEC vRenderPoint = m_tInfo.vPoint + vDir * 10.f;

			CEffectMgr::GetInstance()->CreateEffect(L"Fluffy_AirCircle", vRenderPoint, EFTMGR_IMAGE | EFTMGR_FIXED | EFTMGR_SCROLL, 0.f, 0.05);
			for (int y = 0; y <= 1; ++y)
			{
				for (int i = -1; i <= 1; ++i)
					CObjMgr::GetInstance()->AddObject(OBJID::MONSTER_BULLET, CAbstractFactory<CBullet>::CreateBullet(vRenderPoint - vDir * 50.f * y, L"AirBullet", fTargetAngle + PI * 0.1 * i, 500.f, m_tStat.iPhysicalAtk, 1.f, this));
			}
			break;
		}
		case FLUFFY_STATE::STUN:
			SetFrame(0, 2, 3, m_dStunTime / 3.);
			break;
		case FLUFFY_STATE::DOWN:
			SetFrame(0, 0, 4, m_dDownTime);
			break;
		default:
			break;
		}

		m_eCurState = m_eNextState;
		m_dStateTime = 0.;
	}
}


void CFluffy::UpdateTime()
{
	m_dStateTime += DT;

	if (m_eCurState == FLUFFY_STATE::IDLE || m_eCurState == FLUFFY_STATE::WALK)
	{
		m_dAtkElapse += DT;
		if (m_dAtkElapse >= m_dAtkCycle)
		{
			RequestChange(FLUFFY_STATE::ATK);
		}
	}

	// 피해 유효 근거용 시간
	if (m_bHit)
	{
		m_dHitElapseTime += DT;

		if (m_dHitElapseTime >= m_dIframeTime)
		{
			m_bHit = false;
			m_dHitElapseTime = 0.;
		}
	}

	switch (m_eCurState)
	{
	case FLUFFY_STATE::SUMMON:
		if (m_dStateTime >= m_dSummonTime)
			RequestChange(FLUFFY_STATE::IDLE);
		break;
	case FLUFFY_STATE::IDLE:
		break;
	case FLUFFY_STATE::WALK:
		break;
	case FLUFFY_STATE::ATK:
		if (m_dStateTime >= m_dAtkTime)
		{
			RequestChange(FLUFFY_STATE::IDLE);
			m_dAtkElapse = 0.;
		}
		break;
	case FLUFFY_STATE::STUN:
		if (m_dStateTime >= m_dStunTime)
			RequestChange(FLUFFY_STATE::DOWN);
		break;
	case FLUFFY_STATE::DOWN:
		if (m_dStateTime >= m_dDownTime)
			m_bDead = true;
		break;
	default:
		break;
	}
}

void CFluffy::Move()
{
	if (nullptr == m_pTarget)
		return;

	VEC vDir = (m_pTarget->GetInfo().vPoint - m_tInfo.vPoint).Normalize();
	if (m_eCurState == FLUFFY_STATE::IDLE || m_eCurState == FLUFFY_STATE::WALK)
	{
		if (vDir.fX <= 0) m_pFrameKey = L"Fluffy_L";
		else m_pFrameKey = L"Fluffy_R";
	}

	if (m_eCurState == FLUFFY_STATE::IDLE)
	{
		m_eNextState = FLUFFY_STATE::WALK;
	}

	if (m_eCurState == FLUFFY_STATE::WALK)
	{
		m_tInfo.vPoint += vDir * m_fSpeed * DT;
	}
}

void CFluffy::Attack()
{
}

void CFluffy::HitDamage(int iDamage, CObj* pObj, HIT_SOURCE eHit, bool bCritical)
{
	if (m_bDead || m_tStat.iHp <= 0)
		return;

	if (m_bHit && eHit != HIT_SOURCE::BULLET)
		return;

	if (m_eCurState == FLUFFY_STATE::DOWN || m_eCurState == FLUFFY_STATE::SUMMON)
		return;

	m_bHit = true;		// m_bHit은 m_bHit이 된지 경과한 시간이 iframeTime을 넘으면 false가 된다.

	m_tStat.iHp -= iDamage;

	if (m_tStat.iHp <= 0)
	{
		m_eNextState = FLUFFY_STATE::STUN;

		float fExp = 10.f * pObj->GetStat().fExperienceDrop;
		float fLeaf = 70.f * pObj->GetStat().fLeafDrop;

		CObjMgr::GetInstance()->AddObject(OBJID::DROP, CAbstractFactory<CDrop>::CreateDrop(m_tInfo.vPoint.fX - m_tInfo.vSize.fX * 0.3f, m_tInfo.vPoint.fY + 20, pObj, DROP_TYPE::EXP, fExp));
		CObjMgr::GetInstance()->AddObject(OBJID::DROP, CAbstractFactory<CDrop>::CreateDrop(m_tInfo.vPoint.fX + m_tInfo.vSize.fX * 0.3f, m_tInfo.vPoint.fY + 20, pObj, DROP_TYPE::LEAF, fLeaf));
		static_cast<CPlayer*>(pObj)->AddKill(1);
	}

	if (bCritical)
		CEffectMgr::GetInstance()->CreateEffect(L"", m_tInfo.vPoint, EFTMGR_STRING | EFTMGR_MOVE, 100.f, 0.5f, nullptr, VEC{ 1.f, -1.f }.Normalize(), to_wstring(iDamage), Color{ 255, 255, 210, 60 });
	else
		CEffectMgr::GetInstance()->CreateEffect(L"", m_tInfo.vPoint, EFTMGR_STRING | EFTMGR_MOVE, 100.f, 0.5f, nullptr, VEC{ 1.f, -1.f }.Normalize(), to_wstring(iDamage), Color{ 255, 255, 255, 255 });

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
