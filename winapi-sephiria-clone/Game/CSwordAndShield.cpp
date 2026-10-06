#include "pch.h"
#include "CSwordAndShield.h"

#include "CSword.h"
#include "CShield.h"
#include "CPlayer.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"
#include "CSoundMgr.h"
#include "CEffectMgr.h"

CSwordAndShield::CSwordAndShield()
	: CState(SWORD_AND_SHIELD_STATE::END, SWORD_AND_SHIELD_STATE::IDLE),
	m_pSword(nullptr), m_pShield(nullptr)
{
	// reserve가 아닌 이유는, 0으로 초기화해두기 위해서
	m_vecAtkRect.resize(toUType(SWORD_AND_SHIELD_ATK_RECT::END));
	m_vecDefRect.resize(toUType(SWORD_AND_SHIELD_DEF_RECT::END));
}

CSwordAndShield::~CSwordAndShield()
{
	Release();
}

int CSwordAndShield::Update()
{
	m_tInfo.vPoint = m_pTarget->GetInfo().vPoint;

	ApplyChange();
	AttackUpdate();
	CleaveUpdate();
	DefenseUpdate();
	
#ifdef _DEBUG
	PrintInfo();
#endif // _DEBUG

	return NOEVENT;
}

void CSwordAndShield::LateUpdate()
{
	CreateEffect();
}

void CSwordAndShield::Release()
{
	// 다른 무기와 다르게 소드와 실드를 따로 생성하므로
	// 소멸할 때 정리해줘야 함
	CObjMgr::GetInstance()->DeleteID(OBJID::WEAPON);
}

void CSwordAndShield::SetTarget(CObj* pObj)
{
	m_pTarget = pObj;
	m_pSword->SetTarget(pObj);
	m_pShield->SetTarget(pObj);
}

void CSwordAndShield::Attack()
{
	if (m_eCurState == SWORD_AND_SHIELD_STATE::DEFENSE)
	{
		m_eNextState = SWORD_AND_SHIELD_STATE::CLEAVE_READY; // 한손검 특수 기능 회전베기
		m_fAngle = m_pTarget->GetAngle();
		static_cast<CPlayer*>(m_pTarget)->RequestChange(PLAYER_STATE::HEAVY_ATTACK);
	}
	else
	{
		m_eNextState = SWORD_AND_SHIELD_STATE::ATTACK; 
		m_pSword->SetAngle(m_pTarget->GetAngle()); // 공격 키를 눌렀을 때의 마우스 방향 Angle을 기억시킴
		// 플레이어를 공격 상태로 만들어 줌
		static_cast<CPlayer*>(m_pTarget)->RequestChange(PLAYER_STATE::ATTACK);

		if (m_tAtk.iLevel != 0) // 첫 공격 입력이 아닐 경우에는 다음 연격 플래그를 true로 해줌
			m_tAtk.bNextAtk = true;
	}
}

void CSwordAndShield::SpecialAttack()
{
	// 공격 중일 때는 실드 누른다고 바로 실드되면 안됨
	if (m_eCurState == SWORD_AND_SHIELD_STATE::ATTACK)
		return;

	m_eNextState = SWORD_AND_SHIELD_STATE::DEFENSE;
}

void CSwordAndShield::AttackUpdate()
{
	if (m_eCurState != SWORD_AND_SHIELD_STATE::ATTACK)
		return;

	m_tAtk.dElapseTime += DT; // 시간 누적

	// 현재 공격 시간이 콤보 유지 시간보다 커짐
	if (m_tAtk.dElapseTime >= m_tAtk.dComboTime)
	{
		// IDLE 상태, ElapseTime은 0 초기화, 레벨도 0 초기화
		m_eNextState = SWORD_AND_SHIELD_STATE::IDLE;
		m_tAtk.dElapseTime = 0.;
		m_tAtk.iLevel = 0;

		// 플레이어를 IDLE 상태로 만들어 줌
		static_cast<CPlayer*>(m_pTarget)->RequestChange(PLAYER_STATE::IDLE);
	}
	// 아직 콤보 유지 시간 내임
	else
	{
		// 일단 공격은 끝남
		if (m_tAtk.dElapseTime > m_tAtk.dMaxTime)
		{
			// 연격키를 눌렀었음
			if (m_tAtk.bNextAtk)
			{
				++m_tAtk.iLevel;		 // 레벨 증가
				if (m_tAtk.iLevel > m_tAtk.iMaxLevel)
					return;

				m_tAtk.dElapseTime = 0.; // 공격 경과 시간 초기화
				m_tAtk.bNextAtk = false; // 연격 키 플래그 초기화
			}
		}
	}
}

void CSwordAndShield::DefenseUpdate()
{
	if (m_eCurState != SWORD_AND_SHIELD_STATE::DEFENSE)
		return;

	if (KEY_UP(VK_RBUTTON))
	{
		m_eNextState = SWORD_AND_SHIELD_STATE::IDLE;
		SetRect(&m_vecDefRect[toUType(SWORD_AND_SHIELD_DEF_RECT::DEFENSE)], 0, 0, 0, 0);
		return;
	}
}

void CSwordAndShield::CleaveUpdate()
{
	if (m_eCurState != SWORD_AND_SHIELD_STATE::CLEAVE_READY
		&& m_eCurState != SWORD_AND_SHIELD_STATE::CLEAVE)
		return;

	m_tAtk.dElapseTime += DT;

	if (m_eCurState == SWORD_AND_SHIELD_STATE::CLEAVE_READY)
	{
		if (m_tAtk.dElapseTime <= m_tAtk.dMaxTime)
		{
			float fSpeed = 1000.f;
			VEC vOffset = { fSpeed * cosf(m_fAngle) * (float)DT, fSpeed * sinf(m_fAngle) * (float)DT };
			m_pTarget->AddPos(vOffset);
		}
		else
		{
			m_tAtk.dElapseTime = 0;
			m_eNextState = SWORD_AND_SHIELD_STATE::CLEAVE;
		}
	}
	else
	{
		if (m_tAtk.dElapseTime > m_tAtk.dMaxTime)
		{
			m_tAtk.dElapseTime = 0;
			m_eNextState = SWORD_AND_SHIELD_STATE::IDLE;
			static_cast<CPlayer*>(m_pTarget)->RequestChange(PLAYER_STATE::IDLE);
		}
	}
}

#ifdef _DEBUG
void CSwordAndShield::PrintInfo()
{
	m_dPrintInterval -= DT;
	if (m_dPrintInterval <= 0)
	{
		cout << "공격 상태 : " << m_tAtk.iLevel << ", " << m_tAtk.bNextAtk << ", " << m_tAtk.dElapseTime << ", " << m_tAtk.dMaxTime << ", " << m_tAtk.iLevel << ", " << m_tAtk.iMaxLevel << endl;
		cout << "현재 상태 : " << toUType(m_eCurState) << endl;
		cout << "검 위치 : " << m_pSword->GetInfo().vPoint.fX << " " << m_pSword->GetInfo().vPoint.fY << endl;
		m_dPrintInterval = 3.;
	}
}
#endif // _DEBUG