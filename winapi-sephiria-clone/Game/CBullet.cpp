#include "pch.h"
#include "CBullet.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"

CBullet::CBullet()
{
}

CBullet::~CBullet()
{
}

void CBullet::Initialize()
{
	m_tInfo = { 0, 0, 30.f, 30.f };
	m_fSpeed = 100.f;

	m_eRender = RENDERID::GAMEOBJECT; 
	m_iRenderLayer = 5;
}

int CBullet::Update()
{
	if (m_bDead)
		return DEAD;

	m_vPrePoint = m_tInfo.vPoint;

	__super::UpdateRect();

	return NOEVENT;
}

void CBullet::LateUpdate()
{

	Move();
}

void CBullet::Render(Graphics* pGraphics)
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
	if (m_pFrameKey == L"Fluffy_Bullet")
		vCellSize = { 9.f, 9.f };

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

void CBullet::Release()
{
}

void CBullet::Move()
{
	VEC vTargetPoint = m_pTarget->GetInfo().vPoint;
	VEC vDir = vTargetPoint - m_tInfo.vPoint;

	float fAngle = atan2f(vDir.fY, vDir.fX);

	m_tInfo.vPoint += { float(m_fSpeed* cosf(fAngle)* DT), float(m_fSpeed* sinf(fAngle)* DT) };
}