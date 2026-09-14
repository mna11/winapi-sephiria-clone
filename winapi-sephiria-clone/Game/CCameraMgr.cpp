#include "pch.h"
#include "CCameraMgr.h"
#include "CCamera.h"

#include "CObjMgr.h"
#include "CAbstractFactory.h"

CCameraMgr* CCameraMgr::m_pInstance = nullptr;

CCameraMgr::CCameraMgr()
	:m_pCamera(nullptr), m_vScroll{0.f, 0.f}
{
}

CCameraMgr::~CCameraMgr()
{
	Release();
}

void CCameraMgr::Initialize()
{
	Release();

	// 카메라 생성해서 넣기
	m_pCamera = new CCamera;
	m_pCamera->Initialize();
	CObjMgr::GetInstance()->AddObject(OBJID::CAMERA, m_pCamera);
}

void CCameraMgr::Update()
{
	m_vPreCameraPos = m_pCamera->GetInfo().vPoint;
}

void CCameraMgr::LateUpdate()
{
	VEC vMove = m_pCamera->GetInfo().vPoint - m_vPreCameraPos;
	m_vScroll -= vMove;
}

void CCameraMgr::Release()
{
	CObjMgr::GetInstance()->DeleteID(OBJID::CAMERA);
}

void CCameraMgr::SetCameraPos(VEC vPoint)
{
	m_pCamera->SetPos(vPoint); // 스크롤 반영은 어차피 Update - LateUpdate에서 해주므로 위치만 변경
}

void CCameraMgr::SetCameraTarget(CObj* pObj)
{
	if (nullptr == pObj)
		return;

	// 움직이는 상태와 목표 대상을 변경
	m_pCamera->SetState(CCamera::STATE::NORMAL);
	m_pCamera->SetTarget(pObj);
}

void CCameraMgr::MoveCamera(VEC vPoint)
{
	// 움직이는 상태와 목표 위치를 변경
	m_pCamera->SetState(CCamera::STATE::MOVE_TO);
	m_pCamera->SetTargetPoint(vPoint);
}

void CCameraMgr::CameraShaking(int iStrength, double dDuration)
{
	m_pCamera->Shaking(iStrength, dDuration);
}

// 추후 프레임 떨어질 때, 이거 먼저 고려
//bool CCameraMgr::Culling(CObj* pObj)
//{
//	RECT rc{};
//	if (IntersectRect(&rc, &m_pCamera->GetRect(), &pObj->GetRect()))
//	{
//		return true;
//	}
//	return false;
//}
