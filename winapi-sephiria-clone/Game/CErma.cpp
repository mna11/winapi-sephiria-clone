#include "pch.h"
#include "CErma.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"
#include "CTimeMgr.h"

CErma::CErma()
	: CState(ERMA_STATE::END, ERMA_STATE::IDLE)
{
}

CErma::~CErma()
{
}

void CErma::Initialize()
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
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_LEFT.png", L"Erma_L");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_RIGHT.png", L"Erma_R");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_GoToStone_LEFT.png", L"Erma_Stone_L");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_GoToStone_RIGHT.png", L"Erma_Stone_R");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_Stone_Shiny.png", L"Erma_Stone_Shiny");

	// 애니메이션 프레임 초기화
	m_pFrameKey = L"Erma_L";
	SetFrame(0, 5, 0, 0.2);

	// iframe 세팅
	m_dHitElapseTime = 0.;
	m_dIframeTime = 0.3;   // 피격 후 무적시간 0.3초
	m_bHit = false;
}

int CErma::Update()
{
	if (m_bDead)
		return DEAD;

	// Elapse 변수들 업데이트
	UpdateTime();

	// 이번 프레임 상태 결정
	ApplyChange();

	// 이동 
	Move();

	__super::UpdateRect();
	UpdateFrame();

	return NOEVENT;
}

void CErma::LateUpdate()
{
}

void CErma::Render(Graphics* pGraphics)
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
	if (m_pFrameKey == L"Erma_L" || m_pFrameKey == L"Erma_R")
		vCellSize = { 31.f, 25.f };
	else if (m_pFrameKey == L"Erma_Stone_L" || m_pFrameKey == L"Erma_Stone_R")
		vCellSize = { 38.f, 37.f };
	else if (m_pFrameKey == L"Erma_Stone_Shiny")
		vCellSize = { 25.f, 25.f };

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

void CErma::Release()
{
}

void CErma::UpdateFrame()
{
	if (m_tFrame.dFrameSpeed <= CTimeMgr::GetInstance()->GetTime(m_tFrame.iFrameTime))
	{
		++m_tFrame.iStart;
		QueryPerformanceCounter(&m_tFrame.iFrameTime);

		if (m_tFrame.iStart > m_tFrame.iEnd)
		{
			if (m_eCurState == ERMA_STATE::STONE)
			{
				m_bDead = true;
			}
			else
			{ 
				m_tFrame.iStart = 0;
			}
		}
	}
}

void CErma::ApplyChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case ERMA_STATE::IDLE:
			SetFrame(0, 5, 0, 0.1);
			break;
		case ERMA_STATE::WALK:
			SetFrame(0, 7, 1, 0.1);
			break;
		case ERMA_STATE::AIR:
			SetFrame(0, 7, 2, 0.05);
			break;
		case ERMA_STATE::STUN:
			SetFrame(0, 5, 3, 0.2);
			break;
		case ERMA_STATE::STONE:
			SetFrame(0, 18, 0, 0.1); // 프레임 키가 다름 - Erma_Stone
			break;
		case ERMA_STATE::LIGHT:
			SetFrame(0, 2, 0, 0.2); // 프레임 키가 다름 - Erma_Stone_Shiny
			break;
		default:
			break;
		}

		m_eCurState = m_eNextState;
	}
}

void CErma::SetDamage(int iDamage, CObj* pObj)
{
	if (m_bHit)
		return;

	m_bHit = true;		// m_bHit은 m_bHit이 된지 경과한 시간이 iframeTime을 넘으면 false가 된다.
	m_iHp -= iDamage;

	m_eNextState = ERMA_STATE::STUN;

	if (m_iHp <= 0)
	{
		m_pFrameKey = L"Erma_Stone_L";
		m_eNextState = ERMA_STATE::STONE;
	}

	// 나중에 피해량 이펙트로 띄우기
	// 경험치 pObj에게 넘기기 등을 하면 된다.
}

void CErma::UpdateTime()
{
	// 피해 유효 근거용 시간
	if (m_bHit)
	{
		m_dHitElapseTime += DT;

		if (m_dHitElapseTime >= m_dIframeTime)
		{
			m_bHit = false;
			m_dHitElapseTime = 0.;
			if (m_eCurState == ERMA_STATE::STUN)
				m_eNextState = ERMA_STATE::IDLE;
		}
	}
}

void CErma::Move()
{
}