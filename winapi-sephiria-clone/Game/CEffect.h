#pragma once
#include "CObj.h"
class CEffect :
    public CObj
{
public:
    CEffect();
    ~CEffect();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void UpdateFrame() override;

public:
    void SetAlpha(float fAlpha) { m_fAlpha = fAlpha; }
    void SetDirVec(VEC vDir)    { m_vDir = vDir; }
    void SetPhrase(wstring str) { m_strPhrase = str; m_iRenderLayer = 5; }
    void SetColor(Color tColor) { m_tColor = tColor; }

private:
    float   m_fAlpha;
    VEC     m_vDir;
    wstring m_strPhrase;
    Color   m_tColor;
};

