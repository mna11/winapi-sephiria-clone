#include "pch.h"
#include "CSwordAndShield.h"

#include "CSword.h"
#include "CShield.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"

CSwordAndShield::CSwordAndShield()
	: CState(SWORD_AND_SHIELD_STATE::END, SWORD_AND_SHIELD_STATE::IDLE),
	m_pSword(nullptr), m_pShield(nullptr)
{
	ZeroMemory(m_tBarriorRect, sizeof(PointF) * 4); 
	ZeroMemory(&m_tSwordRect, sizeof(RECT));
}

CSwordAndShield::~CSwordAndShield()
{
	Release();
}

void CSwordAndShield::Initialize()
{
	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 0; // 아무거나 줘도 됨, 어차피 한손검은 직접 렌더 안함
						// 실드는 플레이어 앞에 있어야 하고, 검은 뒤에 있어야 해서
						// 실드랑 소드를 따로 생성해서 ObjMgr에 넣을거임

	// 검과 방패 생성 후 초기화 작업
	// 이후 ObjMgr에 넣어주기
	m_pSword	=  CAbstractFactory<CSword>::CreateSwordAndShield(&m_eCurState, &m_tAtk);
	m_pShield	= CAbstractFactory<CShield>::CreateSwordAndShield(&m_eCurState, &m_tAtk);
	CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pSword);
	CObjMgr::GetInstance()->AddObject(OBJID::WEAPON, m_pShield);

	// 공격 정보 초기화
	// 최대 3연격 가능, 공격 걸리는 시간 1초
	// 공격 상태 처음 진입 시 초기화하긴 하지만, 방어적으로 코드 작성
	SetAtk(0, 3, 0., 0.3);
}

int CSwordAndShield::Update()
{
	m_tInfo.vPoint = m_pTarget->GetInfo().vPoint;

	ApplyChange();
	AttackUpdate();

#ifdef _DEBUG
	PrintInfo();
#endif // _DEBUG

	return NOEVENT;
}

void CSwordAndShield::LateUpdate()
{
}

void CSwordAndShield::Render(Graphics* pGraphics)
{
	// 소드, 실드에서 각자 렌더링함
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
	if (m_eCurState == SWORD_AND_SHIELD_STATE::SHIELD)
		m_eNextState = SWORD_AND_SHIELD_STATE::CLEAVE; // 한손검 특수 기능 회전베기
	else
	{
		// 공격 중이었음
		if (m_tAtk.iLevel != 0 && m_tAtk.dElapseTime <= m_tAtk.dMaxTime)
		{
			m_tAtk.bNextAtk = true;
		}
	
		m_eNextState = SWORD_AND_SHIELD_STATE::ATTACK; 
		m_pSword->SetAngle(m_pTarget->GetAngle());
	}
}

void CSwordAndShield::SpecialAttack()
{
	// 공격 중일 때는 실드 누른다고 바로 실드되면 안됨
	if (m_eCurState == SWORD_AND_SHIELD_STATE::ATTACK)
		return;

	m_eNextState = SWORD_AND_SHIELD_STATE::SHIELD;
}

void CSwordAndShield::AttackUpdate()
{
	if (m_eCurState != SWORD_AND_SHIELD_STATE::ATTACK
		&& m_eCurState != SWORD_AND_SHIELD_STATE::CLEAVE)
		return;

	m_tAtk.dElapseTime += DT; // 시간 누적

	// 공격 시간이 끝남
	if (m_tAtk.dElapseTime >= m_tAtk.dMaxTime)
	{
		// 다음 연격 키를 눌렀음
		if (m_tAtk.bNextAtk)
		{
			// 레벨 증가 및 플래그 초기화
			++m_tAtk.iLevel;
			m_tAtk.bNextAtk = false;
			m_tAtk.dElapseTime = 0.;
			m_pSword->SetAngle(m_pTarget->GetAngle());
			// 최대 레벨을 넘지 않았으면 킵 고잉
			if (m_tAtk.iLevel <= m_tAtk.iMaxLevel)
				return;
		}

		// 다음 연격 키를 누르지 않았음 || 최대 연격 횟수를 넘어감
		// Attack에서 다른 상태로 빠져나가는 것은 이때만 가능함

		// 다시 IDLE 상태로 복귀하고 공격 경과시간과 현재 레벨 초기화
		m_eNextState = SWORD_AND_SHIELD_STATE::IDLE;
		m_tAtk.dElapseTime = 0.;
		m_tAtk.iLevel = 0.;
	}
}

void CSwordAndShield::ApplyChange()
{
	// 상태 첫 진입
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case SWORD_AND_SHIELD_STATE::IDLE:
			break;
		case SWORD_AND_SHIELD_STATE::ATTACK:
			SetAtk(1, 3, 0., 0.5); // 공격 처음 진입 시 초기화
			break;
		case SWORD_AND_SHIELD_STATE::CLEAVE:
			SetAtk(0, 3, 0., 1.); // 나중에 CLEAVE 구현할 때 여기 변경할 것
			break;
		default:
			break;
		}
		m_eCurState = m_eNextState;
	}
}



#ifdef _DEBUG
void CSwordAndShield::PrintInfo()
{
	m_dPrintInterval -= DT;
	if (m_dPrintInterval <= 0)
	{
		cout << "공격 상태 : " << m_tAtk.bNextAtk << "," << m_tAtk.dElapseTime << ", " << m_tAtk.dMaxTime << ", " << m_tAtk.iLevel << ", " << m_tAtk.iMaxLevel << endl;
		cout << "현재 상태 : " << toUType(m_eCurState) << endl;
		cout << "검 위치 : " << m_pSword->GetInfo().vPoint.fX << " " << m_pSword->GetInfo().vPoint.fY << endl;
		m_dPrintInterval = 0.2;
	}
}
#endif // _DEBUG