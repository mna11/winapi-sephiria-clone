#include "pch.h"
#include "CGargoyle.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"
#include "CTimeMgr.h"
#include "CEffectMgr.h"
#include "CFontMgr.h"

CGargoyle::CGargoyle()
	: CState(GARGOYLE_STATE::END, GARGOYLE_STATE::SUMMON),
	m_dStateTime(0.),
	m_dSummonTime(0.),
	m_dAtkTime(0.), m_dAtkCollisionTime(0.),
	m_dDownTime(0.),
	m_dFlyAtkTime(0.), m_dFlyAtkCycleTime(0.), m_dFlyAtkDownTime(0.), m_dFlyAtkCollisionTime(0.), m_dFlyAtkEndTime(0.),
	m_bEffectCreate(false),
	m_fAtkDistance(0.f), m_fFlyAtkDistance(0.f)
{
	// reserve가 아닌 이유는, 0으로 초기화해두기 위해서
	m_vecAtkRect.resize(toUType(GARGOYLE_ATK_RECT::END));

	ZeroMemory(&m_vAtkDir, sizeof(VEC));
	ZeroMemory(&m_vFlyAtkPoint, sizeof(VEC));
}

CGargoyle::~CGargoyle()
{
	Release();
}

void CGargoyle::Initialize()
{
	// 기초 정보 초기화
	m_tInfo = { WINCX >> 1, WINCY >> 1, 100.f, 100.f };
	m_eRender = RENDERID::GAMEOBJECT;
	// 렌더 레이어 - 0 ~ 5 사이 플레이어는 중간인 3
	m_iRenderLayer = 5;
	m_fSpeed = 200.f;

	// 스프라이트 시트 Insert 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Gargoyle/Gargoyle_LEFT.png", L"Gargoyle_L");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Gargoyle/Gargoyle_RIGHT.png", L"Gargoyle_R");

	// 스프라이트 시트 선택
	m_pFrameKey = L"Gargoyle_L";

	// iframe 세팅 
	m_dHitElapseTime = 0.;
	m_dIframeTime = 0.3;   // 피격 후 무적시간 0.3초
	m_bHit = false;

	// 스탯 초기화
	m_tStat = {
		100,			// HP
		100,			// MAX HP
		20,				// 공격력
	};

	// 플레이어와의 공격 거리
	m_fAtkDistance = 100.f; 
	m_fFlyAtkDistance = 400.f; // 400보다 멀어지면 공중 공격 진행

	// 시간 초기화
	m_dStateTime = 0.;
	m_dSummonTime = 2.;
	m_dDownTime = 1.;

	m_dAtkTime = 1.;
	m_dAtkCollisionTime = m_dAtkTime / 8. * 5; // 6번째 프레임부터 공격한다.

	m_dFlyAtkTime = 2.7;    
	m_dFlyAtkCycleTime = 0.5;
	m_dFlyAtkDownTime = 2.0;
	m_dFlyAtkEndTime = 2.2;
	m_dFlyAtkCollisionTime = 2.5;
}

int CGargoyle::Update()
{
	if (m_bDead)
		return DEAD;

	m_vPrePoint = m_tInfo.vPoint;

	// 시간 업데이트
	UpdateTime();
	// 상태 변경
	ApplyChange();
	// 이동
	Move();
	// 공격
	Attack();

	__super::UpdateRect();
	__super::UpdateFrame();

#ifdef _DEBUG
	PrintInfo();
#endif // _DEBUG

	return NOEVENT;
}

void CGargoyle::LateUpdate()
{
}

void CGargoyle::Render(Graphics* pGraphics)
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
	for (int i = 0; i < toUType(GARGOYLE_ATK_RECT::END); ++i)
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

	VEC vCellSize{ 41.f, 40.f };
	VEC vImgSize = vCellSize * PIXEL_SCALE;

	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY,
					vImgSize.fX,
					vImgSize.fY };

	ImageAttributes* pImgAttr = m_bHit ? &m_imgAttrHit : nullptr;
	pImgAttr = m_eCurState == GARGOYLE_STATE::DOWN ? &m_imgAttrDown : pImgAttr;
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

void CGargoyle::Release()
{
}

void CGargoyle::SetDamage(int iDamage, CObj* pObj)
{
	if (m_bHit)
		return;

	if (m_eCurState == GARGOYLE_STATE::DOWN || m_eCurState == GARGOYLE_STATE::SUMMON)
		return;

	m_bHit = true;		// m_bHit은 m_bHit이 된지 경과한 시간이 iframeTime을 넘으면 false가 된다.

	m_tStat.iHp -= iDamage;

	if (m_tStat.iHp <= 0)
	{
		m_eNextState = GARGOYLE_STATE::DOWN;
	}

	CEffectMgr::GetInstance()->CreateEffect(L"", m_tInfo.vPoint, EFTMGR_STRING | EFTMGR_MOVE, 100.f, 0.5f, nullptr, VEC{ 1.f, -1.f }.Normalize(), to_wstring(iDamage), Color{255, 255, 255, 255});

	// 경험치 pObj에게 넘기기 등을 하면 된다.
}

void CGargoyle::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case GARGOYLE_STATE::SUMMON:
			SetFrame(0, 20, 0, m_dSummonTime / 21.);
			break;
		case GARGOYLE_STATE::IDLE:
			SetFrame(0, 5, 1, 0.2);
			break;
		case GARGOYLE_STATE::WALK:
			SetFrame(0, 7, 2, 0.2);
			break;
		case GARGOYLE_STATE::ATK:
			SetFrame(0, 7, 3, m_dAtkTime / 8.);
			break;
		case GARGOYLE_STATE::FLY_ATK:
			SetFrame(0, 3, 4, m_dFlyAtkCycleTime / 4.f);
			break;
		/*case GARGOYLE_STATE::FLY_READY:
			SetFrame(0, 3, 4, 0.2);
			break;
		case GARGOYLE_STATE::FLY_CYCLE:
			SetFrame(0, 5, 5, 0.2);
			break;
		case GARGOYLE_STATE::FLY_DOWN:
			SetFrame(0, 1, 6, 0.2);
			break;
		case GARGOYLE_STATE::FLY_END:
			SetFrame(0, 3, 7, 0.2);
			break;*/
		case GARGOYLE_STATE::AIR:
			SetFrame(0, 0, 8, 1.0);
			break;
		case GARGOYLE_STATE::DOWN:
			SetFrame(0, 0, 9, m_dDownTime);
			break;
		}

		m_eCurState = m_eNextState;
		m_dStateTime = 0.;
		m_bEffectCreate = false; 
	}
}

void CGargoyle::UpdateTime()
{
	m_dStateTime += DT;

	switch (m_eCurState)
	{
	case GARGOYLE_STATE::SUMMON:
		if (m_dStateTime >= m_dSummonTime)
		{
			m_eNextState = GARGOYLE_STATE::IDLE;
		}
		break;

	case GARGOYLE_STATE::IDLE:
		break;
	case GARGOYLE_STATE::WALK:
		break;
	case GARGOYLE_STATE::ATK:
		if (m_dStateTime >= m_dAtkTime)
		{
			m_eNextState = GARGOYLE_STATE::IDLE;

			SetRect(&m_vecAtkRect[toUType(GARGOYLE_ATK_RECT::ATK)], 0, 0, 0, 0);
			m_vAtkDir = { 0.f, 0.f };
		}
		break;
	case GARGOYLE_STATE::FLY_ATK:
		if (m_dStateTime >= m_dFlyAtkTime)
		{
			m_eNextState = GARGOYLE_STATE::IDLE;

			SetRect(&m_vecAtkRect[toUType(GARGOYLE_ATK_RECT::FLY_ATK)], 0, 0, 0, 0);
			m_vFlyAtkPoint = { 0.f, 0.f };
		}
		else if (m_dStateTime >= m_dFlyAtkEndTime)
		{
			if (m_tFrame.iMotion != 7)
				SetFrame(0, 3, 7, (m_dFlyAtkTime - m_dFlyAtkEndTime) / 4. );
		}
		else if (m_dStateTime >= m_dFlyAtkDownTime)
		{
			if (m_tFrame.iMotion != 6)
			{
				SetFrame(0, 1, 6, (m_dFlyAtkEndTime - m_dFlyAtkDownTime) / 2.);
				m_vFlyAtkStartPoint = m_tInfo.vPoint;
			}
		}
		else if (m_dStateTime >= m_dFlyAtkCycleTime)
		{
			if (m_tFrame.iMotion != 5)
				SetFrame(0, 5, 5, (m_dFlyAtkDownTime - m_dFlyAtkCycleTime) / 6.);
		}
		break;
	case GARGOYLE_STATE::AIR:
		break;
	case GARGOYLE_STATE::DOWN:
		if (m_dStateTime >= m_dDownTime)
		{
			m_bDead = true;
		}
		break;
	}

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

}

void CGargoyle::Move()
{
	// 목적 대상이 없다면 이동하지 않음
	if (m_pTarget == nullptr)
		return;

	VEC vDir = (m_pTarget->GetInfo().vPoint - m_tInfo.vPoint).Normalize();
	if (m_eCurState != GARGOYLE_STATE::ATK)
	{
		if (vDir.fX <= 0) m_pFrameKey = L"Gargoyle_L";
		else m_pFrameKey = L"Gargoyle_R";
	}

	if (m_eCurState == GARGOYLE_STATE::IDLE)
	{
		m_eNextState = GARGOYLE_STATE::WALK;
	}

	if (m_eCurState == GARGOYLE_STATE::WALK)
	{
		m_tInfo.vPoint += vDir * m_fSpeed * DT;
	}

	if (m_eCurState == GARGOYLE_STATE::ATK)
	{
		if (m_vAtkDir.fX == 0.f && m_vAtkDir.fY == 0.f)
			m_vAtkDir = vDir;
		
		if (m_dStateTime >= m_dAtkCollisionTime)
			m_tInfo.vPoint += m_vAtkDir * m_fSpeed * 3 * DT;
	}

	if (m_eCurState == GARGOYLE_STATE::FLY_ATK)
	{
		if (m_dStateTime < m_dFlyAtkDownTime)
		{
			m_vFlyAtkPoint = m_pTarget->GetInfo().vPoint;
		}
		else if (m_dStateTime < m_dFlyAtkEndTime)
		{
			float fRatio = (m_dStateTime - m_dFlyAtkDownTime) / (m_dFlyAtkEndTime - m_dFlyAtkDownTime); 
			m_tInfo.vPoint = m_vFlyAtkStartPoint * (1 - fRatio) + m_vFlyAtkPoint * fRatio;
		}
	}
}

void CGargoyle::Attack()
{
	// 목적 대상이 없다면 공격하지 않음
	if (m_pTarget == nullptr)
		return;

	// 일반 공격
	if (m_eCurState == GARGOYLE_STATE::ATK)
	{
		// 공격 충돌 박스 활성화
		if (m_dStateTime >= m_dAtkCollisionTime)
		{
			float fDistance = 50.f;
			float fDirAngle = atan2f(m_vAtkDir.fY, m_vAtkDir.fX);
			float fSizeFactor = 50.f;

			VEC vRenderPoint = m_tInfo.vPoint + m_vAtkDir * fDistance;
			VEC vRectSize = { fSizeFactor + fSizeFactor * fabsf(sinf(fDirAngle)), fSizeFactor + fSizeFactor * fabsf(cosf(fDirAngle)) };


			SetRect(&m_vecAtkRect[toUType(GARGOYLE_ATK_RECT::ATK)], vRenderPoint.fX - vRectSize.fX, vRenderPoint.fY - vRectSize.fY, vRenderPoint.fX + vRectSize.fX, vRenderPoint.fY + vRectSize.fY);

			if (!m_bEffectCreate)
			{
				CEffectMgr::GetInstance()->CreateEffect(L"Gargoyle_Attack", vRenderPoint, EFTMGR_IMAGE | EFTMGR_FOLLOW , fDirAngle, (m_dAtkTime - m_dAtkCollisionTime) / 2.f, this);
				m_bEffectCreate = true;
			}
		}
	}
	// 공중 공격
	else if (m_eCurState == GARGOYLE_STATE::FLY_ATK)
	{
		if (m_dStateTime >= m_dFlyAtkCycleTime)
		{
			if (!m_bEffectCreate)
			{
				CEffectMgr::GetInstance()->CreateEffect(L"Gargoyle_Targeting", m_pTarget->GetInfo().vPoint, EFTMGR_IMAGE | EFTMGR_FOLLOW_N_STOP, m_dFlyAtkEndTime - m_dFlyAtkCycleTime, (m_dFlyAtkEndTime - m_dFlyAtkCycleTime) / 20.f, m_pTarget);
				m_bEffectCreate = true;
			}
		}
	}
	// 공격이 아닐 때, 
	else
	{
		float fDistance = (m_pTarget->GetInfo().vPoint - m_tInfo.vPoint).Norm();

		if (fDistance <= m_fAtkDistance)
			m_eNextState = GARGOYLE_STATE::ATK;

		if (fDistance >= m_fFlyAtkDistance)
			m_eNextState = GARGOYLE_STATE::FLY_ATK;
	}
}

#ifdef _DEBUG
void CGargoyle::PrintInfo()
{
	m_dPrintInterval -= DT;
	if (m_dPrintInterval <= 0)
	{
		cout << "가고일 상태 : " << toUType(m_eCurState) << endl;
		m_dPrintInterval = 3.;
	}
}
#endif // _DEBUG