#include "pch.h"
#include "CBullet.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"
#include "CEffectMgr.h"

CBullet::CBullet()
	: m_fDamage(0.), m_pOwner(nullptr)
{
	ZeroMemory(&m_vCellSize, sizeof(VEC));
	ZeroMemory(&m_vDir, sizeof(VEC));
}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
	m_tInfo = { 0, 0, 30.f, 30.f };
	m_fSpeed = 0.f;

	m_eRender = RENDERID::GAMEOBJECT; 
	m_iRenderLayer = 5;

	// 나중에 CObjMgr Init으로 옮기자
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Bullet/MagicBullet.png", L"MagicBullet");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Bullet/MagicBulletBig.png", L"MagicBulletBig");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Bullet/FireBullet.png", L"FireBullet");
}

int CBullet::Update()
{
	if (m_bDead)
	{
		CreateDisapperEffect();
		return DEAD;
	}

	m_vPrePoint = m_tInfo.vPoint;

	Move();

	__super::UpdateRect();
	__super::UpdateFrame();

	return NOEVENT;
}

void CBullet::LateUpdate()
{

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

	VEC vImgSize = m_vCellSize * PIXEL_SCALE;

	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY,
					vImgSize.fX,
					vImgSize.fY };

	pGraphics->DrawImage(
		pImg, rcDest,
		m_vCellSize.fX * m_tFrame.iStart,
		m_vCellSize.fY * m_tFrame.iMotion,
		m_vCellSize.fX,
		m_vCellSize.fY,
		UnitPixel
	);
}

void CBullet::Release()
{
}

void CBullet::UpdateInfo()
{
	if (!lstrcmp(m_pFrameKey, L"MagicBullet"))
	{
		SetCellSize({7.f, 7.f});
		SetFrame(0, 4, 0, 0.1);
	}
	else if (!lstrcmp(m_pFrameKey, L"MagicBulletBig"))
	{
		SetCellSize({ 13.f, 13.f });
		SetFrame(0, 5, 0, 0.1);
	}
	else if (!lstrcmp(m_pFrameKey, L"FireBullet"))
	{
		SetCellSize({ 10.f, 10.f });
		SetFrame(0, 7, 0, 0.1);
	}
}

void CBullet::Move()
{
	m_tInfo.vPoint += { float(m_fSpeed* cosf(m_fAngle)* DT), float(m_fSpeed* sinf(m_fAngle)* DT) };
}

void CBullet::CreateDisapperEffect()
{
	if (!lstrcmp(m_pFrameKey, L"MagicBullet"))
	{
	}
	else if (!lstrcmp(m_pFrameKey, L"MagicBulletBig"))
	{
	}
	else if (!lstrcmp(m_pFrameKey, L"FireBullet"))
	{
		CEffectMgr::GetInstance()->CreateEffect(L"FireBullet_Disapper_Effect", m_tInfo.vPoint, EFTMGR_FIXED | EFTMGR_IMAGE | EFTMGR_SCROLL, 0.f, 0.03f);
	}
}
