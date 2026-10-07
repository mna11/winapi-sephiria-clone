#include "pch.h"
#include "CArtifact.h"

CArtifact::CArtifact()
{
}

CArtifact::~CArtifact()
{
	Release();
}

void CArtifact::Initialize()
{
}

int CArtifact::Update()
{
	if (m_bDead)
		return DEAD;

	return NOEVENT;
}

void CArtifact::LateUpdate()
{
}

void CArtifact::Render(Graphics*)
{
}

void CArtifact::Release()
{
}

void CArtifact::ChangeLevel(int iLevel)
{
	// 최대 레벨보다 클 수도 있지만, 스탯이 높아지지는 않고
	// 0보다 작아질 수 있지만 스탯 적용이 안됨
	m_iLevel = iLevel;

	m_pTarget->AddStat(m_tStat * -1);

	const ARTIFACT_INFO* pArtifactInfo = CArtifactData::GetInstance()->FindArtifactInfo(m_iID);
	int iMaxLevel = pArtifactInfo->vecStat.size() - 1; // 3렙까지 (0 1 2 3)
	if (iLevel > iMaxLevel)
		m_tStat = pArtifactInfo->vecStat[iMaxLevel];
	else if (iLevel < 0)
		m_tStat = STAT{};
	else
		m_tStat = pArtifactInfo->vecStat[iLevel];

	m_pTarget->AddStat(m_tStat);
}

void CArtifact::InitializeData(int iID, ITEM_TYPE eType)
{
	m_iLevel = 0;
	m_iID = iID;
	m_eItemType = eType;

	const ARTIFACT_INFO* pArtifactInfo = CArtifactData::GetInstance()->FindArtifactInfo(m_iID);
	m_tStat = pArtifactInfo->vecStat[m_iLevel];
}
