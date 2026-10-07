#pragma once
#include "CItem.h"
#include "CArtifactData.h"

class CArtifact :
    public CItem
{
public:
    CArtifact();
    ~CArtifact();
public:
    void Initialize() override;
    int  Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    ITEM_INFO               GetItemInfo() override  { return ITEM_INFO{ m_iID, ITEM_TYPE::ARTIFACT, CArtifactData::GetInstance()->FindArtifactInfo(m_iID)->strName, CArtifactData::GetInstance()->FindArtifactInfo(m_iID)->strImg, CArtifactData::GetInstance()->FindArtifactInfo(m_iID)->iLeaf }; }
    const ARTIFACT_INFO*    GetArtifactInfo() const { return CArtifactData::GetInstance()->FindArtifactInfo(m_iID); }

public:
    void ChangeLevel(int iLevel);

    // CItem을(를) 통해 상속됨
    void InitializeData(int m_iID, ITEM_TYPE eType) override;
};

