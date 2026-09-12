#include "pch.h"
#include "CCamera.h"

#include "CObjMgr.h"
#include "CTimeMgr.h"

#include "CCameraMgr.h"

CCamera::CCamera()
	: m_eCurState(STATE::NORMAL), m_ePreState(STATE::END)
{
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
	return NOEVENT;
}

void CCamera::LateUpdate()
{
	if (m_pTarget)
	{
		CVector vec = { m_pTarget->GetInfo().fX - m_tInfo.fX,
						m_pTarget->GetInfo().fY - m_tInfo.fY };
		float fAngle = vec.GetAngle();


		

		CCameraMgr::GetInstance()->SetCameraPos(m_tInfo.fX + m_fSpeed * cosf(fAngle * PI / 180.f) * DT, 
												m_tInfo.fY + m_fSpeed * sinf(fAngle * PI / 180.f) * DT);
	}

	__super::UpdateRect();
}

void CCamera::Render(Graphics* pGraphics)
{
}

void CCamera::Release()
{
}

void CCamera::SetTarget(CObj* pObj)
{
	if (nullptr != pObj)
	{
		// 카메라 이동용 임시 객체였을 때는 삭제 후 바꿔줘야하므로
		if (0 == pObj->GetInfo().fCX)
			CObjMgr::GetInstance()->DeleteID(OBJID::CAMERA_TARGET);
		m_pTarget = pObj;
	}
}
