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
	// 한손검 회전베기
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword_TurnSwin.png", L"Cleave");

	// 보스 몬스터 에르마
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Alert_Square.png", L"Erma_Alert_Square");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Golem_Hand_Shadow_LEFT.png", L"Erma_Hand_Shadow_L");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Golem_Hand_Shadow_RIGHT.png", L"Erma_Hand_Shadow_R");

	// 가고일
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Monster/Gargoyle/Gargoyle_Attack.png", L"Gargoyle_Attack");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Monster/Gargoyle/Gargoyle_ShockWave.png", L"Gargoyle_ShockWave");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Monster/Gargoyle/Gargoyle_ShockWave_Targeting.png", L"Gargoyle_Targeting");
}


CObj* CEffectMgr::CreateEffect(const TCHAR* pFrameKey, VEC vPoint, int iOption, float fFactor, double dFrameSpeed, CObj* pObj, VEC vDir, wstring wstr, Color tColor)
{
	CEffect* pEffect = static_cast<CEffect*>(CAbstractFactory<CEffect>::CreateObj(vPoint.fX, vPoint.fY));
	pEffect->SetFrameKey(pFrameKey);
 	if (!lstrcmpW(pFrameKey, L"RunDust"))
	{
		pEffect->SetFrame(0, 8, 0, 0.2);
		pEffect->SetSize({ 7.f, 7.f });
	}
	else if (!lstrcmpW(pFrameKey, L"DashDust"))
	{
		if (fFactor < 0) // PI / 8를 더한 이유는, 더하니깐 잘 되더라...
			fFactor += 2 * PI + PI / 8;

		int iDirection = (int)(fFactor / (PI / 4));
		pEffect->SetFrame(0, 5, iDirection, 0.2);
		pEffect->SetSize({ 37, 23 });
	}
	else if (!lstrcmpW(pFrameKey, L"DashTrail"))
	{
		pEffect->SetFrame(0, 5, 0, 0.05);
		pEffect->SetSize({ 15.f, 16.f });
		pEffect->SetAlpha(fFactor);
	}
	else if (!lstrcmpW(pFrameKey, L"SwordSwing1"))
	{
		pEffect->SetFrame(0, 1, 0, 0.1);
		pEffect->SetSize({ 37.f, 22.f });
		pEffect->SetAngle(fFactor);
	}
	else if (!lstrcmpW(pFrameKey, L"SwordSwing2"))
	{
		pEffect->SetFrame(0, 1, 0, 0.1);
		pEffect->SetSize({ 30.f, 30.f });
		pEffect->SetAngle(fFactor);
	}
	else if (!lstrcmpW(pFrameKey, L"SwordSwing3"))
	{
		pEffect->SetFrame(0, 3, 0, 0.1);
		pEffect->SetSize({ 32.f, 43.f });
		pEffect->SetAngle(fFactor);
	}
	else if (!lstrcmpW(pFrameKey, L"Shield"))
	{
		// 이거 나중에 꼭 고쳐야함
		pEffect->SetFrame(0, 1, 0, 0.001);
		pEffect->SetSize({ 16, 32 });
		pEffect->SetAngle(fFactor);
	}
	else if (!lstrcmpW(pFrameKey, L"Cleave"))
	{
		pEffect->SetFrame(0, 7, 0, 0.1);
		pEffect->SetSize({ 142, 105 });
		pEffect->SetAngle(fFactor);
	}
	// 가고일
	else if (!lstrcmpW(pFrameKey, L"Gargoyle_Attack"))
	{
		pEffect->SetFrame(0, 1, 0, dFrameSpeed);
		pEffect->SetSize({ 43, 49 });
		pEffect->SetAngle(fFactor);
	}
	else if (!lstrcmpW(pFrameKey, L"Gargoyle_Targeting"))
	{
		pEffect->SetFrame(0, 19, 0, dFrameSpeed);
		pEffect->SetSize({ 96, 72 });
		pEffect->SetRenderOption(RENDERID::GAMEOBJECT, 1);
	}

	if (iOption & EFTMGR_IMAGE)
	{
		// 이미지 - 기본값
	}
	if (iOption & EFTMGR_STRING)
	{
		if (0 != wstr.compare(L""))
		{
			pEffect->SetFrame(0, 0, 0, dFrameSpeed);
			pEffect->SetPhrase(wstr);
			pEffect->SetColor(tColor);
			pEffect->SetSpeed(fFactor);
		}
	}
	if (iOption & EFTMGR_FIXED)
	{
		// 고정 - 기본값 
	}
	if (iOption & EFTMGR_FOLLOW || iOption & EFTMGR_FOLLOW_N_STOP)
	{
		if (nullptr != pObj)
			pEffect->SetTarget(pObj);
	}
	if (iOption & EFTMGR_FOLLOW_N_STOP)
	{
		pEffect->SetStopTime(fFactor);
	}
	if (iOption & EFTMGR_MOVE)
	{
		if (vDir.fX != 0.f && vDir.fY != 0.f)
			pEffect->SetDirVec(vDir);
	}


	CObjMgr::GetInstance()->AddObject(OBJID::EFFECT, pEffect);
	return pEffect;
}