#include "pch.h"
#include "CSwordAndShield.h"

#include "CSword.h"
#include "CShield.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"
#include "CEffectMgr.h"

CSwordAndShield::CSwordAndShield()
	: CState(SWORD_AND_SHIELD_STATE::END, SWORD_AND_SHIELD_STATE::IDLE),
	m_pSword(nullptr), m_pShield(nullptr)
{
	ZeroMemory(&m_tAtkRect, sizeof(RECT));
	ZeroMemory(&m_tDefRect, sizeof(RECT));
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
	SetAtk(0, 3, 0., 0.2, 1.0);
}

int CSwordAndShield::Update()
{
	m_tInfo.vPoint = m_pTarget->GetInfo().vPoint;

	ApplyChange();
	AttackUpdate();
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

void CSwordAndShield::Render(Graphics* pGraphics)
{
	// 소드, 실드에서 각자 렌더링함
	
#ifdef _DEBUG
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
	SolidBrush blackBrush(Color(255, 0, 0, 0));

	pGraphics->FillRectangle(&blackBrush, (int)(m_tAtkRect.left + vScroll.fX),
		(int)(m_tAtkRect.top + vScroll.fY),
		(int)m_tAtkRect.right - m_tAtkRect.left,
		(int)m_tAtkRect.bottom - m_tAtkRect.top);

	pGraphics->FillRectangle(&blackBrush, (int)(m_tDefRect.left + vScroll.fX),
		(int)(m_tDefRect.top + vScroll.fY),
		(int)m_tDefRect.right - m_tDefRect.left,
		(int)m_tDefRect.bottom - m_tDefRect.top);
#endif // _DEBUG
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
		m_eNextState = SWORD_AND_SHIELD_STATE::CLEAVE; // 한손검 특수 기능 회전베기
	}
	else
	{
		m_eNextState = SWORD_AND_SHIELD_STATE::ATTACK; 
		m_pSword->SetAngle(m_pTarget->GetAngle()); // 공격 키를 눌렀을 때의 마우스 방향 Angle을 기억시킴
		
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


void CSwordAndShield::CreateEffect()
{
	VEC vRenderPoint{}; 
	VEC vRectSize{};

	float fTargetAngle = m_pTarget->GetAngle();
	float fDistance = 50.f;
	VEC vOffset{ fDistance * cosf(fTargetAngle), fDistance * sinf(fTargetAngle) };

	// 공격
	if (KEY_DOWN(VK_LBUTTON))
	{
		float fTargetAngle = m_pTarget->GetAngle();
		float fDistance = 50.f;
		VEC vOffset{ fDistance * cosf(fTargetAngle), fDistance * sinf(fTargetAngle) };

		vRenderPoint = m_tInfo.vPoint + vOffset;

		switch (m_tAtk.iLevel)
		{
		case 0:
			CEffectMgr::GetInstance()->CreateEffect(L"SwordSwing1", vRenderPoint, m_pTarget->GetAngle());
			vRectSize = { 50.f, 50.f };
			break;
		case 1:
			CEffectMgr::GetInstance()->CreateEffect(L"SwordSwing2", vRenderPoint, m_pTarget->GetAngle());
			vRectSize = { 60.f, 60.f };
			break;
		case 2:
			CEffectMgr::GetInstance()->CreateEffect(L"SwordSwing3", vRenderPoint, m_pTarget->GetAngle());
			vRectSize = { 80.f, 80.f };
			break;
		}
	}
	SetRect(&m_tAtkRect, vRenderPoint.fX - vRectSize.fX, vRenderPoint.fY - vRectSize.fY, vRenderPoint.fX + vRectSize.fX, vRenderPoint.fY + vRectSize.fY);
	
	// 방어
	vRenderPoint = { 0.f, 0.f };
	vRectSize	 = { 0.f, 0.f };
	if (KEY_HOLD(VK_RBUTTON))
	{
		float fTargetAngle = m_pTarget->GetAngle();
		float fDistance = 40.f;
		VEC vOffset{ fDistance * cosf(fTargetAngle), fDistance * sinf(fTargetAngle) };

		vRenderPoint = m_tInfo.vPoint + vOffset;
		float fSizeFactor = 27.f;
		vRectSize = { fSizeFactor + fSizeFactor * fabsf(sinf(fTargetAngle)), fSizeFactor + fSizeFactor * fabsf(cosf(fTargetAngle))};

		m_pDefenceEffect = CEffectMgr::GetInstance()->CreateEffect(L"Shield", vRenderPoint, m_pTarget->GetAngle());
		
		vRenderPoint -= vOffset * 0.2f;
		SetRect(&m_tDefRect, vRenderPoint.fX - vRectSize.fX, vRenderPoint.fY - vRectSize.fY, vRenderPoint.fX + vRectSize.fX, vRenderPoint.fY + vRectSize.fY);
	}
}


void CSwordAndShield::AttackUpdate()
{
	if (m_eCurState != SWORD_AND_SHIELD_STATE::ATTACK
		&& m_eCurState != SWORD_AND_SHIELD_STATE::CLEAVE)
		return;

	m_tAtk.dElapseTime += DT; // 시간 누적

	// 현재 공격 시간이 콤보 유지 시간보다 커짐
	if (m_tAtk.dElapseTime >= m_tAtk.dComboTime)
	{
		// IDLE 상태, ElapseTime은 0 초기화, 레벨도 0 초기화
		m_eNextState = SWORD_AND_SHIELD_STATE::IDLE;
		m_tAtk.dElapseTime = 0.;
		m_tAtk.iLevel = 0;
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
		SetRect(&m_tDefRect, 0, 0, 0, 0);
		return;
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
			SetAtk(1, 3, 0., 0.2, 1.); // 공격 처음 진입 시 초기화
			break;
		case SWORD_AND_SHIELD_STATE::DEFENSE:
			break;
		case SWORD_AND_SHIELD_STATE::CLEAVE:
			SetAtk(0, 3, 0., 1., 1.); // 나중에 CLEAVE 구현할 때 여기 변경할 것
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
		cout << "공격 상태 : " << m_tAtk.iLevel << ", " << m_tAtk.bNextAtk << ", " << m_tAtk.dElapseTime << ", " << m_tAtk.dMaxTime << ", " << m_tAtk.iLevel << ", " << m_tAtk.iMaxLevel << endl;
		cout << "현재 상태 : " << toUType(m_eCurState) << endl;
		cout << "검 위치 : " << m_pSword->GetInfo().vPoint.fX << " " << m_pSword->GetInfo().vPoint.fY << endl;
		m_dPrintInterval = 0.2;
	}
}
#endif // _DEBUG