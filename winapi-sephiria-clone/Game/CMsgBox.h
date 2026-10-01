#pragma once
#include "CUI.h"

// 구성 요소이므로 CUIMgr의 영향을 받지 않는다.

class CButton;

class CMsgBox :
    public CUI
{
public:
    CMsgBox();
    ~CMsgBox();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void SetButtonNumber(int iNum);
    void SetLayout(MSG_BOX_LAYOUT eLayout) { m_eLayout = eLayout; }
    void SetString(wstring wstr) { m_string = wstr; }
    void SetPrintRect(RectF rcRect) { m_rcPrint = rcRect; }
public:
    vector<CButton*>& GetButtons() { return m_vecButton; }
public:
    void UpdateLayout();    // 세팅 정보를 기반으로 메세지 박스 레이아웃을 업데이트함

private:
    wstring m_string;

    vector<CButton*> m_vecButton; // 버튼들
    RectF            m_rcPrint;   // 메세지 박스 출력 위치 크기
    RectF            m_rcStringPrint; // 문자열 박스 출력 위치 크기
    MSG_BOX_LAYOUT   m_eLayout;   // 메세지 박스 레이아웃
};

