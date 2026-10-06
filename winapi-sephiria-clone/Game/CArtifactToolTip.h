#pragma once
#include "CUI.h"
class CArtifactToolTip :
    public CUI
{
public:
    CArtifactToolTip();
    ~CArtifactToolTip();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;                      
    
    // 유니티의 9-slicing-sprite처럼 상중하로 나눈 3-slicing-sprite 방식으로 구현
};

