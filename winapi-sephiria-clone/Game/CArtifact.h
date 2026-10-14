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
    ITEM_INFO               GetItemInfo() override { return CArtifactData::GetInstance()->FindItemInfo(m_iID); }
    const ARTIFACT_INFO*    GetArtifactInfo() const { return CArtifactData::GetInstance()->FindArtifactInfo(m_iID); }
public:
    void InitializeData(int m_iID, ITEM_TYPE eType) override;
public:
    void ChangeLevel(int iLevel);
};

