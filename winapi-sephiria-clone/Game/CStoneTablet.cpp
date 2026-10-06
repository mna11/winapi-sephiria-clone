#include "pch.h"
#include "CStoneTablet.h"

CStoneTablet::CStoneTablet()
{
}

CStoneTablet::~CStoneTablet()
{
	Release();
}

void CStoneTablet::Initialize()
{
}

int CStoneTablet::Update()
{
	if (m_bDead)
		return DEAD;

	return NOEVENT;
}

void CStoneTablet::LateUpdate()
{
}

void CStoneTablet::Render(Graphics*)
{
}

void CStoneTablet::Release()
{
}