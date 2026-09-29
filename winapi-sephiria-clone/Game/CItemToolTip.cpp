#include "pch.h"
#include "CItemToolTip.h"
#include "CImgMgr.h"

CItemToolTip::CItemToolTip()
{
}

CItemToolTip::~CItemToolTip()
{
	Release();
}

void CItemToolTip::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;

	// 이 UI는 중점 좌표보다는 오히려 왼쪽 상단 위치로 두는게 slot 배치시 코드가 더 깔끔할거 같아서
	// m_tInfo.vPoint가 중점이 아니라 LT임
	m_tInfo = { 0.f, 0.f, 0.f, 0.f };

	// 스프라이트 넣기 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/ItemToolTip/ItemToolTip_Base.png", L"ItemToolTip_Base");

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 4;
}

int CItemToolTip::Update()
{
	return 0;
}

void CItemToolTip::LateUpdate()
{
}

void CItemToolTip::Render(Graphics*)
{
}

void CItemToolTip::Release()
{
}
