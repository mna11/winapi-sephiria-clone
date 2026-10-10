#pragma once
#include "CUI.h"
class CStat :
    public CUI
{
public:
    CStat();
    ~CStat();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

private:
    void RenderAttackStat(Graphics* pGraphics, Image* pImg, VEC vOffset, int iImgCol, int iImgRow, int iAtk);
    void RenderCommonStat(Graphics* pGraphics, Image* pImg, VEC vOffset, float fWidth, int iImgCol, int iImgRow, wstring strTitle, wstring strInfo);
};

