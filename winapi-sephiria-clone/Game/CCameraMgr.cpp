#include "pch.h"
#include "CCameraMgr.h"
#include "CCamera.h"
#include "CCameraTarget.h"

#include "CObjMgr.h"
#include "CAbstractFactory.h"

CCameraMgr* CCameraMgr::m_pInstance = nullptr;

CCameraMgr::CCameraMgr()
	: m_iScrollX(0), m_iScrollY(0), m_pCamera(nullptr)
{
}

CCameraMgr::~CCameraMgr()
{
	Release();
}

void CCameraMgr::Initialize()
{
	Release();

	m_pCamera = new CCamera;
	m_pCamera->Initialize();
	CObjMgr::GetInstance()->AddObject(OBJID::CAMERA, m_pCamera);
}

void CCameraMgr::Release()
{
	CObjMgr::GetInstance()->DeleteID(OBJID::CAMERA);
}

void CCameraMgr::SetCameraPos(float fX, float fY)
{
	INFO tCamera = m_pCamera->GetInfo();
	float fDx = tCamera.fX - fX;
	float fDy = tCamera.fY - fY;

	m_iScrollX -= fDx;
	m_iScrollY -= fDy;

	m_pCamera->SetPos(fX, fY);
}

void CCameraMgr::SetCameraTarget(CObj* pObj)
{
	if (nullptr == pObj)
		return;

	m_pCamera->SetTarget(pObj);
}

void CCameraMgr::MoveCamera(const CVector& rhs, float fSpeed)
{
	CObj* pObj = CAbstractFactory<CCameraTarget>::CreateObj(rhs.m_fX, rhs.m_fY);
	CObjMgr::GetInstance()->AddObject(OBJID::CAMERA_TARGET, pObj);
	m_pCamera->SetTarget(pObj);
}

// 추후 프레임 떨어질 때, 이거 먼저 고려
bool CCameraMgr::Culling(CObj* pObj)
{
	RECT rc{};
	if (IntersectRect(&rc, &m_pCamera->GetRect(), &pObj->GetRect()))
	{
		return true;
	}
	return false;
}
