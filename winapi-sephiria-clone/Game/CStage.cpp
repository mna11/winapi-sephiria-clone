#include "pch.h"
#include "CStage.h"
#include "CPlayer.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(WINCX >> 1, WINCY >> 1));
}

void CStage::Update()
{
	CObjMgr::GetInstance()->Update();
}

void CStage::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();
}

void CStage::Render(Graphics* pGraphics)
{
	CObjMgr::GetInstance()->Render(pGraphics);
}

void CStage::Release()
{
}
