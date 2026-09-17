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
	// 플레이어 이동
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Run/RunDust.png", L"RunDust");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Dash/DashDust.png", L"DashDust");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Dash/DashTrail.png", L"DashTrail");
	// 한손검 공격
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword_1.png", L"SwordSwing1");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword_2.png", L"SwordSwing2");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword_3.png", L"SwordSwing3");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword0_TurnSwing.png", L"SwordTurnSwing");
	// 한손검 방어
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Shield.png", L"Shield");
}

CObj* CEffectMgr::CreateEffect(const TCHAR* pFrameKey, VEC vPoint, float fFactor)
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
		pObj->SetSize({ 37.f, 22.f });
		pObj->SetAngle(fFactor);
	}
	else if (pFrameKey == L"SwordSwing2")
	{
		pObj->SetFrame(0, 1, 0, 0.1);
		pObj->SetSize({ 30.f, 30.f });
		pObj->SetAngle(fFactor);
	}
	else if (pFrameKey == L"SwordSwing3")
	{
		pObj->SetFrame(0, 3, 0, 0.1);
		pObj->SetSize({ 32.f, 43.f });
		pObj->SetAngle(fFactor);
	}
	else if (pFrameKey == L"Shield")
	{	
		// 이거 나중에 꼭 고쳐야함
		pObj->SetFrame(0, 1, 0, 0.001);
		pObj->SetSize({ 16, 32 });
		pObj->SetAngle(fFactor);
	}

	CObjMgr::GetInstance()->AddObject(OBJID::EFFECT, pObj);
	return pObj;
}