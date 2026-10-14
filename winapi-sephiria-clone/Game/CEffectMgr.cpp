#include "pch.h"
#include "CEffectMgr.h"
#include "CEffect.h"

#include "CImgMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CSoundMgr.h"

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
	// 기본 한손검 공격
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword_1.png", L"SwordSwing1");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword_2.png", L"SwordSwing2");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword_3.png", L"SwordSwing3");
	// 기본 한손검 회전베기
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Sword_TurnSwin.png", L"Cleave");

	// 매직 완드 공격
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/MagicWand/Wand_Swing.png", L"MagicWandSwing1");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/MagicWand/Wand_Swing_VerticalFlip.png", L"MagicWandSwing2");
	// 매직 완드 회전베기
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/MagicWand/Wand_Cleave.png", L"MagicWandCleave");

	// 한손검 방어
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Weapon/Shield.png", L"Shield");

	// 플레이어 공격 이펙트
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/PlayerAttack/AttackSlash.png", L"Attack_Slash");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/PlayerAttack/AttackBullet.png", L"Attack_Bullet");

	// 보스 몬스터 에르마
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Alert_Square.png", L"Erma_Alert_Square");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Golem_Hand_Shadow_LEFT.png", L"Erma_Hand_Shadow_L");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Golem_Hand_Shadow_RIGHT.png", L"Erma_Hand_Shadow_R");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Golem_Slam_Targeting.png", L"Golem_Targeting");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Golem_Slam.png", L"Golem_Slam");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Alert_Missile.png", L"Golem_Alert_Missle");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Missile_UP.png", L"Golem_Missile_U");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Missile_Down.png", L"Golem_Missile_D");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Boss/Golem_Core_Broken.png", L"Golem_Core_Broken");
	

	// 가고일
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Monster/Gargoyle/Gargoyle_Attack.png", L"Gargoyle_Attack");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Monster/Gargoyle/Gargoyle_ShockWave.png", L"Gargoyle_ShockWave");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Monster/Gargoyle/Gargoyle_ShockWave_Targeting.png", L"Gargoyle_Targeting");

	// 전투 알람
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Stage/ExclamationMark.png", L"Exclamation_Mark");
	// 전투 벽
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Stage/BattleWall.png", L"Battle_Wall");
	
	// 레벨업 이펙트
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/LevelUp/LevelUp_Aura.png", L"LevelUp_Effect");
	
	// 경험치 획득 이펙트
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Drop/Exp.png", L"Exp_Effect");
	// 리프 획득 이펙트
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Drop/Leaf.png", L"Leaf_Effect");

	// 총알 소멸 이펙트
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Bullet/MagicBall_Disapper.png", L"MagicBall_Disapper_Effect");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Bullet/MagicBallBig_Disapper.png", L"MagicBallBig_Disapper_Effect");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Bullet/FireBullet_Disapper.png", L"FireBullet_Disapper_Effect");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Bullet/AirBullet_Disapper.png", L"AirBullet_Disapper_Effect");

	// 이모지 느낌표
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Emoj/Exclamation/Exclamation.png", L"Emoj_Exclamation");
	
	// 플러피
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/Monster/Fluffy/Fluffy_AirCircle.png", L"Fluffy_AirCircle");
	
	// 포션 이펙트
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Effect/PotionUse/PotionUse.png", L"Potion_Use");
}


CObj* CEffectMgr::CreateEffect(const TCHAR* pFrameKey, VEC vPoint, int iOption, float fFactor, double dFrameSpeed, CObj* pObj, VEC vDir, wstring wstr, Color tColor)
{
	CEffect* pEffect = static_cast<CEffect*>(CAbstractFactory<CEffect>::CreateObj(vPoint.fX, vPoint.fY));
	pEffect->SetFrameKey(pFrameKey);
 	if (!lstrcmpW(pFrameKey, L"RunDust"))
	{
		pEffect->SetFrame(0, 8, 0, 0.2);
		pEffect->SetSize({ 7.f, 7.f });
		
		CSoundMgr::GetInstance()->PlaySound(L"FootStep.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"DashDust"))
	{
		if (fFactor < 0) // PI / 8를 더한 이유는, 더하니깐 잘 되더라...
			fFactor += 2 * PI + PI / 8;

		int iDirection = (int)(fFactor / (PI / 4));
		pEffect->SetFrame(0, 5, iDirection, 0.2);
		pEffect->SetSize({ 37, 23 });

		CSoundMgr::GetInstance()->PlaySound(L"FootStep.wav", CHANNEL_GROUPID::SFX, 1.f, 2.f);
	}
	else if (!lstrcmpW(pFrameKey, L"DashTrail"))
	{
		pEffect->SetFrame(0, 5, 0, 0.05);
		pEffect->SetSize({ 15.f, 16.f });
		pEffect->SetAlpha(fFactor);

		CSoundMgr::GetInstance()->PlaySound(L"Dash.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"SwordSwing1"))
	{
		pEffect->SetFrame(0, 1, 0, 0.1);
		pEffect->SetSize({ 37.f, 22.f });
		pEffect->SetAngle(fFactor);

		CSoundMgr::GetInstance()->PlaySound(L"SwingSwish01.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"SwordSwing2"))
	{
		pEffect->SetFrame(0, 1, 0, 0.1);
		pEffect->SetSize({ 30.f, 30.f });
		pEffect->SetAngle(fFactor);

		CSoundMgr::GetInstance()->PlaySound(L"SwingSwish01.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"SwordSwing3"))
	{
		pEffect->SetFrame(0, 3, 0, 0.1);
		pEffect->SetSize({ 32.f, 43.f });
		pEffect->SetAngle(fFactor);

		CSoundMgr::GetInstance()->PlaySound(L"SwingSwish02.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"MagicWandSwing1"))
	{
		pEffect->SetFrame(0, 6, 0, 0.1);
		pEffect->SetSize({ 28.f, 21.f });
		pEffect->SetAngle(fFactor);

		CSoundMgr::GetInstance()->PlaySound(L"SwingMagical01.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"MagicWandSwing2"))
	{
		pEffect->SetFrame(0, 6, 0, 0.05);
		pEffect->SetSize({ 28.f, 21.f });
		pEffect->SetAngle(fFactor);

		CSoundMgr::GetInstance()->PlaySound(L"SwingMagical02.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"MagicWandCleave"))
	{
		pEffect->SetFrame(0, 6, 0, 0.04);
		pEffect->SetSize({ 48, 38 });
		pEffect->SetAngle(fFactor);

		// 임시
		CSoundMgr::GetInstance()->PlaySound(L"0687_attackMagicalIce.wav", CHANNEL_GROUPID::SFX, 1.f);
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

		CSoundMgr::GetInstance()->PlaySound(L"Cleave.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	// 가고일
	else if (!lstrcmpW(pFrameKey, L"Gargoyle_Attack"))
	{
		pEffect->SetFrame(0, 1, 0, dFrameSpeed);
		pEffect->SetSize({ 43, 49 });
		pEffect->SetAngle(fFactor);

		CSoundMgr::GetInstance()->PlaySound(L"SwingSwish02.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"Gargoyle_Targeting"))
	{
		pEffect->SetFrame(0, 19, 0, dFrameSpeed);
		pEffect->SetSize({ 96, 72 });
		pEffect->SetRenderOption(RENDERID::GAMEOBJECT, 0);
	}
	// 전투
	else if (!lstrcmpW(pFrameKey, L"Exclamation_Mark"))
	{
		pEffect->SetFrame(0, 5, 0, 0.1);
		pEffect->SetSize({ 89, 82 });
		pEffect->SetScale(0.5);

		CSoundMgr::GetInstance()->PlaySound(L"BattleEntry.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"Battle_Wall"))
	{
		pEffect->SetFrame(0, 7, 0, 0.1);
		pEffect->SetSize({ 16.f, 16.f });
		pEffect->SetLoop(true);
	}
	// 레벨업
	else if (!lstrcmpW(pFrameKey, L"LevelUp_Effect"))
	{
		pEffect->SetFrame(0, 16, 0, dFrameSpeed);
		pEffect->SetSize({ 58.f, 60.f });

		CSoundMgr::GetInstance()->PlaySound(L"LevelUp.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	// 드랍 아이템 획득 
	else if (!lstrcmpW(pFrameKey, L"Exp_Effect"))
	{
		pEffect->SetFrame(0, 7, 0, dFrameSpeed);
		pEffect->SetSize({ 7.f, 15.f });

		CSoundMgr::GetInstance()->PlaySound(L"GetExp.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"Leaf_Effect"))
	{
		pEffect->SetFrame(0, 3, 0, dFrameSpeed);
		pEffect->SetSize({ 13.f, 11.f });
		
		CSoundMgr::GetInstance()->PlaySound(L"GetLeaf.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	// 총알 소멸 이펙트
	else if (!lstrcmpW(pFrameKey, L"MagicBullet_Disapper_Effect"))
	{
		pEffect->SetFrame(0, 5, 0, dFrameSpeed);
		pEffect->SetSize({ 7.f, 7.f });
		pEffect->SetAngle(fFactor);
	}
	else if (!lstrcmpW(pFrameKey, L"MagicBulletBig_Disapper_Effect"))
	{
		pEffect->SetFrame(0, 6, 0, dFrameSpeed);
		pEffect->SetSize({ 13.f, 13.f });
		pEffect->SetAngle(fFactor);
	}
	else if (!lstrcmpW(pFrameKey, L"FireBullet_Disapper_Effect"))
	{
		pEffect->SetFrame(0, 7, 0, dFrameSpeed);
		pEffect->SetSize({ 10.f, 10.f });
		pEffect->SetAngle(fFactor);
	}
	else if (!lstrcmpW(pFrameKey, L"AirBullet_Disapper_Effect"))
	{
		pEffect->SetFrame(0, 8, 0, dFrameSpeed);
		pEffect->SetSize({ 22.f, 15.f });
		pEffect->SetAngle(fFactor);
	}
	// 보스 몬스터
	else if (!lstrcmpW(pFrameKey, L"Golem_Targeting"))
	{
		pEffect->SetFrame(0, 19, 0, dFrameSpeed);
		pEffect->SetSize({ 52.f, 52.f });
		pEffect->SetRenderOption(RENDERID::GAMEOBJECT, 0);
	}
	else if (!lstrcmpW(pFrameKey, L"Golem_Slam"))
	{
		pEffect->SetFrame(0, 6, 0, dFrameSpeed);
		pEffect->SetSize({ 76.f, 39.f });
		pEffect->SetRenderOption(RENDERID::GAMEOBJECT, 0);

		CSoundMgr::GetInstance()->PlaySound(L"GolemSlam.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"Golem_Alert_Missle"))
	{
		pEffect->SetFrame(0, 17, 0, dFrameSpeed);
		pEffect->SetSize({ 66.f, 34.f });
		pEffect->SetRenderOption(RENDERID::GAMEOBJECT, 0);
	}
	else if (!lstrcmpW(pFrameKey, L"Golem_Missile_U"))
	{
		pEffect->SetFrame(0, 11, 0, dFrameSpeed);
		pEffect->SetSize({ 17.f, 106.f });
	}
	else if (!lstrcmpW(pFrameKey, L"Golem_Missile_D"))
	{
		pEffect->SetFrame(0, 16, 0, dFrameSpeed);
		pEffect->SetSize({ 51.f, 103.f });

		CSoundMgr::GetInstance()->PlaySound(L"GolemMissileDown.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"Golem_Core_Broken"))
	{
		pEffect->SetFrame(0, 7, 0, dFrameSpeed);
		pEffect->SetSize({ 128.f, 128.f });

		CSoundMgr::GetInstance()->PlaySound(L"0588_clangSword0.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	// 느낌표 이모지
	else if (!lstrcmpW(pFrameKey, L"Emoj_Exclamation"))
	{
		pEffect->SetFrame(0, 5, 0, dFrameSpeed);
		pEffect->SetSize({ 5.f, 11.f });

		CSoundMgr::GetInstance()->PlaySound(L"EmojiSurprise.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	// 플러피
	else if (!lstrcmpW(pFrameKey, L"Fluffy_AirCircle"))
	{
		pEffect->SetFrame(0, 24, 0, dFrameSpeed);
		pEffect->SetSize({20.f, 24.f});
	}
	// 포션 사용
	else if (!lstrcmpW(pFrameKey, L"Potion_Use"))
	{
		pEffect->SetFrame(0, 8, 0, dFrameSpeed);
		pEffect->SetSize({ 18.f, 30.f });

		CSoundMgr::GetInstance()->PlaySound(L"Drinking.wav", CHANNEL_GROUPID::SFX, 1.f);
		CSoundMgr::GetInstance()->PlaySound(L"SpellBuff.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	// 플레이어 공격 이펙트
	else if (!lstrcmpW(pFrameKey, L"Attack_Slash"))
	{
		pEffect->SetFrame(0, 6, 0, dFrameSpeed);
		pEffect->SetSize({ 5.f, 57.f });
		pEffect->SetAngle(fFactor);

		CSoundMgr::GetInstance()->PlaySound(L"HitSword02.wav", CHANNEL_GROUPID::SFX, 1.f);
	}
	else if (!lstrcmpW(pFrameKey, L"Attack_Bullet"))
	{
		pEffect->SetFrame(0, 5, 0, dFrameSpeed);
		pEffect->SetSize({ 46.f, 35.f });
		pEffect->SetAngle(fFactor);

		CSoundMgr::GetInstance()->PlaySound(L"HitMagicFire02.wav", CHANNEL_GROUPID::SFX, 1.f);
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
	if (iOption & EFTMGR_SCROLL)
	{
		// 스크롤 적용 - 기본값
	}
	if (iOption & EFTMGR_NO_SCROLL)
	{
		pEffect->SetScroll(false);
	}


	CObjMgr::GetInstance()->AddObject(OBJID::EFFECT, pEffect);
	return pEffect;
}