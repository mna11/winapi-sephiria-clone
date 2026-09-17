#include "pch.h"
#include "CEffectMgr.h"
#include "CEffect.h"

#include "CImgMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"

CEffectMgr* CEffectMgr::m_pInstance = nullptr;

CEffectMgr::CEffectMgr()
{
}

CEffectMgr::~CEffectMgr()
{
}

void CEffectMgr::Initialize()
{	 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Run/RunDust.png", L"RunDust");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Dash/DashDust.png", L"DashDust");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Dash/DashTrail.png", L"DashTrail");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword0_1.png", L"SwordSwing1");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword0_2.png", L"SwordSwing2");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword0_3.png", L"SwordSwing3");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword0_TurnSwing.png", L"SwordTurnSwing");
}

void CEffectMgr::CreateEffect(const TCHAR* pFrameKey, VEC vPoint, float fFactor)
{
	CObj* pObj = CAbstractFactory<CEffect>::CreateObj(vPoint.fX, vPoint.fY);
	pObj->SetFrameKey(pFrameKey);

	if (pFrameKey == L"RunDust")
	{
		pObj->SetFrame(0, 8, 0, 0.2);
		pObj->SetSize({ 7.f, 7.f });
	}
	else if (pFrameKey == L"DashDust")
	{
		if (fFactor < 0) // PI / 8를 더한 이유는, 더하니깐 잘 되더라...
			fFactor += 2 * PI + PI / 8;

		int iDirection = (int)(fFactor / (PI / 4));
		pObj->SetFrame(0, 5, iDirection, 0.2);
		pObj->SetSize({37, 23});
	}
	else if (pFrameKey == L"DashTrail")
	{
		pObj->SetFrame(0, 5, 0, 0.05);
		pObj->SetSize({ 15.f, 16.f });
		static_cast<CEffect*>(pObj)->SetAlpha(fFactor);
	}
	else if (pFrameKey == L"SwordSwing1")
	{
		pObj->SetFrame(0, 1, 0, 0.1);
		pObj->SetSize({ 22.f, 37.f });
	}
	else if (pFrameKey == L"SwordSwing2")
	{
		pObj->SetFrame(0, 1, 0, 0.1);
		pObj->SetSize({ 30.f, 30.f });
	}
	else if (pFrameKey == L"SwordSwing3")
	{
		pObj->SetFrame(0, 3, 0, 0.1);
		pObj->SetSize({ 43.f, 32.f });
	}

	CObjMgr::GetInstance()->AddObject(OBJID::EFFECT, pObj);
}