#pragma once
#include "CUI.h"
class CStoneTabletToolTIp :
    public CUI
{
public:
    CStoneTabletToolTIp();
    ~CStoneTabletToolTIp();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
public:
    void Show() override;
    void Hide() override;
private:
    void Rotate();
};

