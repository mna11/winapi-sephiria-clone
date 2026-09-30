#pragma once
#include "CUI.h"
class CItemToolTip :
    public CUI
{
public:
    CItemToolTip();
    ~CItemToolTip();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void SetMiddleSize(float fMiddleSize) { m_fMiddleSize = fMiddleSize; }

private:
    float m_fMiddleSize;    // Base 이미지의 중앙 길이
                            // 유니티의 9-slicing-sprite처럼 상중하로 나눈 3-slicing-sprite 방식으로 구현
};

