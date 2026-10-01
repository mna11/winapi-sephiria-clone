#include "pch.h"
#include "CMsgBox.h"

CMsgBox::CMsgBox()
{
}

CMsgBox::~CMsgBox()
{
	Release();
}

void CMsgBox::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;
	m_tInfo = { 0.f, 0.f, 0.f, 0.f };

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 5;
}

// 메세지 박스는 몇번째 버튼을 클릭했는지를 반환해줌 - 0은 아무것도 클릭 안함
int CMsgBox::Update()
{
	if (!m_bView)
		return NOEVENT;




	return NOEVENT;
}

void CMsgBox::LateUpdate()
{
	if (!m_bView)
		return;
}

void CMsgBox::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;
}

void CMsgBox::Release()
{
}
