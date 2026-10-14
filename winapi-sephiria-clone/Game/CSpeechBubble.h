#pragma once
#include "CUI.h"
#include "CKeyGuide.h"

// UIMgr를 통해 ShowUI(UIID::SpeechBubble) 해주고, SetText와 SetOffset해줘야 함

class CSpeechBubble 
    : public CUI
{
public:
    CSpeechBubble();
    ~CSpeechBubble();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;

public:
    // 타겟 + 말풍선 텍스트들 + 나타낼 시간 + 사이즈
    void SetText(CObj* pTarget, wstring strName, wstring strText, double dAliveTime, float fWidth, float Height = 24.f);
    void Show() override;
    // 숨기기
    void Hide() override;

    // 오프셋 설정
    void SetOffset(VEC vOffset) { m_vOffset = vOffset; }
    // 말풍선이 종료되었는가
    bool IsFinished() const { return !m_bView; }

private:
    void UpdateTime();

private:
    wstring m_strName;      // 말하는 Obj 이름
    wstring m_strText;      // 대화 내용

    double m_dElapseTime;   // 현재 흐른 시간
    double m_dAliveTime;    // 말풍선이 남아있는 시간

    float m_fWidth;         // 말풍선 Width
    float m_fHeight;        // 말풍선 Height

    VEC m_vOffset;          // 말하는 Obj 위치 기준 Offset
};