#include "pch.h"
#include "CCamera.h"

#include "CObjMgr.h"
#include "CTimeMgr.h"

#include "CCameraMgr.h"

CCamera::CCamera()
	: m_eCurState(STATE::NORMAL), m_ePreState(STATE::END),
	m_dShakingRunningTime(0.), m_bShaking(false)
{
	ZeroMemory(&m_vTargetPoint, sizeof(VEC));
	ZeroMemory(&m_vOffset, sizeof(VEC));
}

CCamera::~CCamera()
{
	Release();
}

void CCamera::Initialize()
{
	m_tInfo = { WINCX >> 1, WINCY >> 1, WINCX, WINCY };
	m_fSpeed = 200.f;
	m_eRender = RENDERID::CAMERA;
}

int CCamera::Update()
{
	ChangeState();

	return NOEVENT;
}

void CCamera::LateUpdate()
{
	HandleBehavior();			// 다른 Obj들의 Update가 끝난 뒤 실행
	__super::UpdateRect();		// 이동 후에 사각형 범위 업데이트
}

void CCamera::Render(Graphics* pGraphics)
{
	// 나중에 필터같은거 씌울 때 써보기
}

void CCamera::Release()
{
}

void CCamera::HandleBehavior()
{
	// FIXED는 아무것도 안하고, NORMAL이랑 MOVE_TO는 거의 비슷한데, 이거 통합시키고 싶음
	// 근데 마땅한게 안떠오르는 상태 - 일단 방치
	switch (m_eCurState)
	{
	case STATE::NORMAL:
		HandleNormal();
		break;
	case STATE::FIXED:
		HandleFixed();
		break;
	case STATE::MOVE_TO:
		HandleMoveTo();
		break;
	default:
		break;
	}

	if (m_bShaking)
	{
		m_dShakingRunningTime -= DT;

		if (m_dShakingRunningTime <= 0.)
		{
			m_bShaking = false;
			m_vOffset = { 0.f, 0.f };
			return; 
		}
		m_vOffset = { (float)m_disShaking(g_engine), (float)m_disShaking(g_engine) };
		m_tInfo.vPoint += m_vOffset;
	}
}

// m_pTarget을 따라다니는 처리
void CCamera::HandleNormal()
{
	if (nullptr == m_pTarget)
		return;

	VEC vMove = m_pTarget->GetInfo().vPoint - m_tInfo.vPoint;

	// 실제 거리와 이번 프레임에 이동 거리를 구해 비교
	float fDistance = vMove.Norm();
	float fCurFrameMove = m_fSpeed * DT;

	// 더 멀리가면 Target에 맞게 처리
	if (fDistance <= fCurFrameMove)
	{
		m_tInfo.vPoint = m_pTarget->GetInfo().vPoint;
		return; 
	}

	// 아직 그정도 아니면 더해주기
	m_tInfo.vPoint += vMove.Normalize() * fCurFrameMove;
}

void CCamera::HandleFixed()
{
	// 아무런 이동 X
}

void CCamera::HandleMoveTo()
{
	VEC vMove = m_vTargetPoint - m_tInfo.vPoint;

	// 실제 거리와 이번 프레임에 이동 거리를 구해 비교
	float fDistance = vMove.Norm();
	float fCurFrameMove = m_fSpeed * DT;

	// 더 멀리가면 Target에 맞게 처리
	if (fDistance <= fCurFrameMove)
	{
		m_tInfo.vPoint = m_vTargetPoint;
		m_eCurState = STATE::FIXED; // 원하는 위치까지 이동했으므로 상태 바꿔주기
		return;
	}

	// 아직 그정도 아니면 더해주기
	m_tInfo.vPoint += vMove.Normalize() * fCurFrameMove;
}

void CCamera::ChangeState()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
			// 이것저것 처리 - 아직 뭐할지 모르겠음
		case STATE::NORMAL:
			if (nullptr != m_pTarget)
			{
				// 스피드를 대상 물체와 동일하게
				m_fSpeed = m_pTarget->GetSpeed();
			}
			break;
		case STATE::MOVE_TO:
			m_fSpeed = 200.f;
			break;
		case STATE::FIXED:
			break;
		default:
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CCamera::Shaking(int iStrength, double dTime)
{
	m_bShaking = true;
	m_dShakingRunningTime = dTime;
	m_disShaking = uniform_int_distribution<int>(-iStrength, iStrength);
}
