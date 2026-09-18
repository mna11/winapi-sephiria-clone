#include "pch.h"
#include "CFluffy.h"

#include "CBullet.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CTimeMgr.h"


CFluffy::CFluffy()
	: CState(FLUFFY_STATE::END, FLUFFY_STATE::SUMMON), m_dAtkInterval(4.), m_dAtkElapseTime(0.)
{
}

CFluffy::~CFluffy()
{
	Release();
}

void CFluffy::Initialize()
{
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 3;

	m_iHp = 500; // 임시

	// 기초 정보 초기화
	m_tInfo = { WINCX >> 1, WINCY >> 1, 100.f, 100.f };
	m_eRender = RENDERID::GAMEOBJECT;
	// 렌더 레이어 - 0 ~ 5 사이
	m_iRenderLayer = 3;

	// 스프라이트 시트 Insert 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Fluffy/Ghost_Fluffy_LEFT.png", L"Fluffy_L");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Fluffy/Ghost_Fluffy_RIGHT.png", L"Fluffy_R");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Fluffy/Ghost_Fluffy_Summon.png", L"Fluffy_Summon");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Fluffy/Fluffy_Bullet.png", L"Fluffy_Bullet");


	// 애니메이션 프레임 초기화
	m_pFrameKey = L"Fluffy_Summon";
	SetFrame(0, 13, 0, 0.2);

	// iframe 세팅
	m_dHitElapseTime = 0.;
	m_dIframeTime = 0.3;   // 피격 후 무적시간 0.3초
	m_bHit = false;
}

int CFluffy::Update()
{
	if (m_bDead)
		return DEAD;

	UpdateTime();

	ApplyChange();

	Move();

	Attack();

	__super::UpdateRect();
	UpdateFrame();

	return NOEVENT;
}

void CFluffy::LateUpdate()
{
}

void CFluffy::Render(Graphics* pGraphics)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

#ifdef _DEBUG
	SolidBrush blackBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&blackBrush, (int)(m_tRect.left + vScroll.fX),
		(int)(m_tRect.top + vScroll.fY),
		(int)m_tInfo.vSize.fX,
		(int)m_tInfo.vSize.fY);
#endif // _DEBUG

	Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;


	VEC vCellSize{};
	if (m_pFrameKey == L"Fluffy_L" || m_pFrameKey == L"Fluffy_R")
		vCellSize = { 23.f, 21.f };
	else if (m_pFrameKey == L"Fluffy_Summon")
		vCellSize = { 65.f, 35.f };

	VEC vImgSize = vCellSize * PIXEL_SCALE;

	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY,
					vImgSize.fX,
					vImgSize.fY };

	ImageAttributes* pImgAttr = m_bHit ? &m_imgAttrHit : nullptr;
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
			SetFrame(0, 13, 0, 0.2);				// 프레임 키가 다름 - Fluffy_Summon
			break;
		case FLUFFY_STATE::IDLE:
			SetFrame(0, 6, 0, 0.2);
			break;
		case FLUFFY_STATE::WALK:
			SetFrame(0, 6, 1, 0.2);
			break;
		case FLUFFY_STATE::ATTACK:
			SetFrame(0, 9, 2, 0.05);
			break;
		case FLUFFY_STATE::STUN:
			SetFrame(0, 2, 3, 0.2);
			break;
		case FLUFFY_STATE::DOWN:
			SetFrame(0, 0, 0, 2.); 
			break;
		default:
			break;
		}

		m_eCurState = m_eNextState;
	}
}


void CFluffy::UpdateTime()
{
	// 피해 유효 근거용 시간
	if (m_bHit)
	{
		m_dHitElapseTime += DT;

		if (m_dHitElapseTime >= m_dIframeTime)
		{
			m_bHit = false;
			m_dHitElapseTime = 0.;
			if (m_eCurState == FLUFFY_STATE::STUN)
				m_eNextState = FLUFFY_STATE::IDLE;
		}
	}

	m_dAtkElapseTime += DT;
}

void CFluffy::UpdateFrame()
{
	if (m_tFrame.dFrameSpeed <= CTimeMgr::GetInstance()->GetTime(m_tFrame.iFrameTime))
	{
		++m_tFrame.iStart;
		QueryPerformanceCounter(&m_tFrame.iFrameTime);

		if (m_tFrame.iStart > m_tFrame.iEnd)
		{
			if (m_eCurState == FLUFFY_STATE::DOWN)
			{
				m_bDead = true;
			}
			else if (m_eCurState == FLUFFY_STATE::SUMMON)
			{
				m_pFrameKey = L"Fluffy_L";
				m_eNextState = FLUFFY_STATE::IDLE;
			}
			else if (m_eCurState == FLUFFY_STATE::ATTACK)
			{
				m_eNextState = FLUFFY_STATE::IDLE;
			}
			else
			{
				m_tFrame.iStart = 0;
			}
		}
	}
}

void CFluffy::Move()
{

}

void CFluffy::Attack()
{
	if (m_dAtkElapseTime > m_dAtkInterval)
	{
		if (m_eCurState == FLUFFY_STATE::SUMMON)
			return;

		m_eNextState = FLUFFY_STATE::ATTACK;
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER_BULLET, CAbstractFactory<CBullet>::CreateBullet(m_tInfo.vPoint, L"Fluffy_Bullet", m_pTarget));
		m_dAtkElapseTime = 0.;
	}
}

void CFluffy::SetDamage(int iDamage, CObj* pObj)
{
	if (m_bHit)
		return;

	m_bHit = true;		// m_bHit은 m_bHit이 된지 경과한 시간이 iframeTime을 넘으면 false가 된다.
	m_iHp -= iDamage;

	m_eNextState = FLUFFY_STATE::STUN;

	if (m_iHp <= 0)
	{
		m_pFrameKey = L"Fluffy_L";
		m_eNextState = FLUFFY_STATE::DOWN;
	}

	// 나중에 피해량 이펙트로 띄우기
	// 경험치 pObj에게 넘기기 등을 하면 된다.
}
