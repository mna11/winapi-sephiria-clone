#include "pch.h"
#include "CBullet.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"
#include "CEffectMgr.h"

CBullet::CBullet()
	: m_fDamage(0.), m_pOwner(nullptr), m_dAliveTime(0.), m_bWallCol(false)
{
	ZeroMemory(&m_vCellSize, sizeof(VEC));
	ZeroMemory(&m_vWallNormal, sizeof(VEC));
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

	m_dAliveTime = -1;

	// 나중에 CObjMgr Init으로 옮기자
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Bullet/MagicBullet.png", L"MagicBullet");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Bullet/MagicBulletBig.png", L"MagicBulletBig");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Bullet/FireBullet.png", L"FireBullet");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Bullet/AirBullet.png", L"AirBullet");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Bullet/ShieldBullet.png", L"ShieldBullet");
}

int CBullet::Update()
{
	if (m_bDead)
	{
		CreateDisapperEffect();
		return DEAD;
	}

	m_vPrePoint = m_tInfo.vPoint;

	UpdateTime();

	Move();

	__super::UpdateRect();
	__super::UpdateFrame();

	return NOEVENT;
}

void CBullet::LateUpdate()
{
	HandleCollision();
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


	float fDegree = m_fAngle * 180.f / PI;
	Matrix matRot;
	matRot.RotateAt(fDegree, { m_tInfo.vPoint.fX + vScroll.fX, m_tInfo.vPoint.fY + vScroll.fY });
	pGraphics->SetTransform(&matRot); // DC 회전

	pGraphics->DrawImage(
		pImg, rcDest,
		m_vCellSize.fX * m_tFrame.iStart,
		m_vCellSize.fY * m_tFrame.iMotion,
		m_vCellSize.fX,
		m_vCellSize.fY,
		UnitPixel
	);

	pGraphics->ResetTransform();
}

void CBullet::Release()
{
}


void CBullet::UpdateTime()
{
	m_dAliveTime -= DT;

	if (m_dAliveTime <= 0)
		m_bDead = true;

	if (!lstrcmp(m_pFrameKey, L"ShieldBullet"))
	{

	}
}

void CBullet::UpdateInfo()
{
	if (!lstrcmp(m_pFrameKey, L"MagicBullet"))
	{
		SetCellSize({ 7.f, 7.f });
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
	else if (!lstrcmp(m_pFrameKey, L"AirBullet"))
	{
		SetCellSize({ 16.f, 6.f });
		SetFrame(0, 3, 0, 0.1);
	}
	else if (!lstrcmp(m_pFrameKey, L"ShieldBullet"))
	{
		SetCellSize({ 20.f, 20.f });
		SetFrame(0, 7, 0, 0.05);
	}
	else
		return; 


	m_tInfo.vSize = m_vCellSize * PIXEL_SCALE;
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
		CEffectMgr::GetInstance()->CreateEffect(L"FireBullet_Disapper_Effect", m_tInfo.vPoint, EFTMGR_FIXED | EFTMGR_IMAGE | EFTMGR_SCROLL, m_fAngle, 0.03f);
	}
	else if (!lstrcmp(m_pFrameKey, L"AirBullet"))
	{
		CEffectMgr::GetInstance()->CreateEffect(L"AirBullet_Disapper_Effect", m_tInfo.vPoint, EFTMGR_FIXED | EFTMGR_IMAGE | EFTMGR_SCROLL, m_fAngle, 0.05f);
	}
	else if (!lstrcmp(m_pFrameKey, L"ShieldBullet"))
	{
		VEC vDir{ cosf(m_fAngle), sinf(m_fAngle) };
		CEffectMgr::GetInstance()->CreateEffect(L"ShieldBullet_Disapper_Effect", m_tInfo.vPoint, EFTMGR_MOVE | EFTMGR_IMAGE | EFTMGR_SCROLL, 200.f, 0.05f, nullptr, vDir);
	}
}

void CBullet::OnHitObj(CObj* pObj)
{
	bool bCritical =  (uniform_real_distribution<float>(0.f, 1.f)(g_engine) < m_pOwner->GetStat().fCriticalChange);
	if (bCritical)
		m_fDamage = static_cast<int>(m_fDamage * (1.f + m_pOwner->GetStat().fCriticalDamage));

	pObj->HitDamage(m_fDamage, m_pOwner, HIT_SOURCE::BULLET, bCritical);
	SetDead(true);
}

void CBullet::HandleCollision()
{
	if (m_bWallCol)
		SetDead(true);
}