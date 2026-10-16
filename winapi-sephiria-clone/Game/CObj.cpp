#include "pch.h"
#include "CObj.h"

#include "CTimeMgr.h"
#include "CEffectMgr.h"

int CObj::s_iObjID = 0;

CObj::CObj()
	:
	m_fSpeed(0.f),
	m_fAngle(0.f),
	m_bDead(false),
	m_pTarget(nullptr),
	m_pFrameKey(L""),
	m_eRender(RENDERID::END),
	m_iRenderLayer(0),
	m_bHit(false),
	m_dIframeTime(0.),
	m_dHitElapseTime(0.),
	m_bWallCollisionEnable(true),
	m_iObjID(0)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_vPrePoint, sizeof(VEC));
	ZeroMemory(&m_tStat, sizeof(STAT));

	m_iObjID = s_iObjID++;
}

CObj::~CObj()
{
}

void CObj::SetFrame(int iStart, int iEnd, int iMotion, double dFrameSpeed)
{
	m_tFrame = { iStart, iEnd, iMotion, dFrameSpeed, 0.};
}

void CObj::HitDamage(int iDamage, CObj* pObj, HIT_SOURCE eHit, bool bCritical)
{
	if (m_bHit)
		return;

	m_bHit = true;		
	m_dHitElapseTime = 0;

	m_tStat.iHp -= iDamage;

	if (m_tStat.iHp <= 0)
		m_bDead = true;
}

void CObj::UpdateRect()
{
	m_tRect.left =		LONG(m_tInfo.vPoint.fX - (m_tInfo.vSize.fX / 2.f));
	m_tRect.top =		LONG(m_tInfo.vPoint.fY - (m_tInfo.vSize.fY / 2.f));
	m_tRect.right =		LONG(m_tInfo.vPoint.fX + (m_tInfo.vSize.fX / 2.f));
	m_tRect.bottom =	LONG(m_tInfo.vPoint.fY + (m_tInfo.vSize.fY / 2.f));
}

void CObj::UpdateFrame()
{
	m_tFrame.dFrameElapsedTime += DT;
	if (m_tFrame.dFrameSpeed <= m_tFrame.dFrameElapsedTime)
	{
		++m_tFrame.iStart;
		m_tFrame.dFrameElapsedTime -= m_tFrame.dFrameSpeed;

		if (m_tFrame.iStart > m_tFrame.iEnd)
			m_tFrame.iStart = 0;
	}
}