#pragma once
#include "CUI.h"

class CKeyGuide :
    public CUI
{
public:
    CKeyGuide();
    ~CKeyGuide();
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
    void SetKeyGuideInfo(int vKey, wstring strGuide, VEC vOffset, VEC vSize);
    void SetGuideString(wstring strGuide) { m_strGuide = strGuide; }
    void SetGuideKey(int vKey);
    void SetOffset(VEC vOffset) { m_vOffset = vOffset; }
    void SetSize(VEC vSize) { m_tInfo.vSize = vSize; }
    void SetFollow(bool bFollow) { m_bFollow = bFollow; }

public:
    void ShowGuide(CObj* pObj);
    void HideGuide(CObj* pObj);

private:
    VEC     m_vKeyImgPoint;
    wstring m_strGuide;
    VEC     m_vOffset;

    bool    m_bFollow;
};

