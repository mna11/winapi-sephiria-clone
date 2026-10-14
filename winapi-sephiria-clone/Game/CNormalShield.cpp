#include "pch.h"
#include "CNormalShield.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"

CNormalShield::CNormalShield()
{
	m_vCellSize = { 9.f, 10.f };
	m_pFrameKey = L"Normal_Sword_Shield_Shield";
}

CNormalShield::~CNormalShield()
{
	Release();
}