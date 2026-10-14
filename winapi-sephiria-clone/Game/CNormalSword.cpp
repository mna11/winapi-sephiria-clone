#include "pch.h"
#include "CNormalSword.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"

CNormalSword::CNormalSword()
{
	m_vCellSize = { 9.f, 16.f };
	m_pFrameKey = L"Normal_Sword_Shield_Sword";
}

CNormalSword::~CNormalSword()
{
}