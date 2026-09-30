#include "pch.h"
#include "CBaba.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"
#include "CObjMgr.h"
#include "CUIMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"

#include "CCollisionMgr.h"

CBaba::CBaba()
{
	ZeroMemory(&m_tInteractRect, sizeof(RECT));
}

CBaba::~CBaba()
{
	Release();
}

void CBaba::Initialize()
{
	// 기초 정보 초기화
	m_tInfo = { 0.f, 0.f, 120.f, 120.f };
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 1;

	// 스프라이트 시트 Insert 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Shop/Baba/Baba.png", L"Baba");

	// 애니메이션 프레임 초기화
	m_pFrameKey = L"Baba";
	SetFrame(0, 5, 0, 0.2);

	// iframe 세팅 
	//m_dHitElapseTime = 0.;
	//m_dIframeTime = 0.5;   // 피격 후 무적시간 0.5초
	//m_bHit = false;
}

int CBaba::Update()
{

	HandleInteraction();

	UpdateInteractRect();

	__super::UpdateRect(); 
	__super::UpdateFrame();
	return NOEVENT;
}

void CBaba::LateUpdate()
{
}

void CBaba::Render(Graphics* pGraphics)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

#ifdef _DEBUG
	// 상호작용 렉트
	SolidBrush blackBrush(Color(255, 0, 0, 0));
	pGraphics->FillRectangle(&blackBrush, (int)(m_tInteractRect.left + vScroll.fX),
		(int)(m_tInteractRect.top + vScroll.fY),
		(int)(m_tInteractRect.right - m_tInteractRect.left),
		(int)(m_tInteractRect.bottom - m_tInteractRect.top)
	);
	// 충돌 렉트
	SolidBrush whiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&whiteBrush, (int)(m_tRect.left + vScroll.fX),
		(int)(m_tRect.top + vScroll.fY),
		(int)m_tInfo.vSize.fX,
		(int)m_tInfo.vSize.fY);
#endif // _DEBUG

	Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

	VEC vCellSize{ 75.f, 52.f };
	VEC vImgSize = vCellSize * PIXEL_SCALE;

	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY,
					vImgSize.fX,
					vImgSize.fY };

	pGraphics->DrawImage(
		pImg, rcDest,
		vCellSize.fX * m_tFrame.iStart,
		vCellSize.fY * m_tFrame.iMotion,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);
}

void CBaba::Release()
{
}

void CBaba::HandleInteraction()
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	// 상호작용 렉트에 들어온 상태
	if (CCollisionMgr::CollisionRect(pPlayer->GetRect(), m_tInteractRect))
	{
		if (KEY_DOWN('F'))
		{
			CSceneMgr::GetInstance()->RequestChange(SCENEID::SHOP);
		}
	}
}

void CBaba::UpdateInteractRect()
{
	VEC vSize{ 120.f, 120.f };
	SetRect(&m_tInteractRect, m_tInfo.vPoint.fX - vSize.fX, m_tInfo.vPoint.fY - vSize.fY,
		m_tInfo.vPoint.fX + vSize.fX, m_tInfo.vPoint.fY + vSize.fY);
}
