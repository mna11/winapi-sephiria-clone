#pragma once
#include "CUI.h"

class CButton;
class CStageChangePlayerIcon;

class CStageChange :
    public CUI
{
public:
    CStageChange();
    ~CStageChange();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void Show() override;
    void Hide() override;

public:
    void SetCurrentNode(int iNodeID);

private:
    void InitializeNode();
    void InitializePlayerIcon();
public:
    void UpdateScroll();
    void UpdateNode();

private:
    NODE_INFO CreateNode(int iID);
    CButton*  CreateButton(int iID);


private:
    const int                   m_iNodeNum;         // 노드 개수
    float                       m_fHeight;          // 화면 전체 높이
    vector<NODE_INFO>           m_vecNode;          // 노드들
    vector<pair<int, int>>      m_vecEdge;          // 노드 간선들
    VEC                         m_vWheelScroll;     // 휠 스크롤 오프셋

    int                         m_iCurStep;         // 현재 스텝 (단계)
    int                         m_iCurNode;         // 현재 노드 아이디

    CStageChangePlayerIcon*     m_pPlayerIcon;      // 플레이어 아이콘
};

