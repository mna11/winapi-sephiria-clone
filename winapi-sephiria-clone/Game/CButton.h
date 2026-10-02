#pragma once
#include "CUI.h"

// 버튼은 단독으로 사용할 수 있다.
// 무조건 버튼은 초기화 시에 화면 어디에 출력할 것인지 SetPrintRect를 해줘야한다.
// 이미지 키를 변경하지 않을 경우, 기본 이미지가 출력된다.
// 만약 이미지 키를 변경할 경우, 맞는 셀 사이즈로 SetCellSize를 해줘야 한다.

// 버튼은 UI이긴 하나, UIMgr의 영향을 받지 않는다. (UIMgr는 하나의 전체적인 UI를 다루는 느낌)
// InventorySlot이나 ShopTableSlot처럼 구성 요소의 역할이 강하다고 생각해서 그리 처리하였다.
// 생성과 소멸의 책임은 호출자에게 있다. -> 다만 이때 소멸은 delete가 아닌 아래에 기재한 방식이다.

// 생성은 CAbstractFactory로 생성하여 Update, LateUpdate, Render는 CObjMgr에게 맡긴다.
// 직접 호출자에서 업데이트나 렌더를 하지 않는 이유는 Render Layer를 지키기 위해서이다.
// 소멸은 생성시 멤버 변수로 저장해둔 것을 SetDead(true)하면 된다.

// [사용 정리]
// CAbstractFactory<CButton>::CreateButton(...)을 불러 버튼 멤버 변수에 저장시킨다.
// OnClick 콜백 함수를 SetOnClick(...)으로 지정해준다.
// CObjMgr::GetInstance()->AddObject(...)로 넣어준다.
// Release()에서 버튼 멤버 변수를 SetDead(true)로 지정 후 nullptr로 바꿔준다.

class CButton :
    public CUI
{
public:
    CButton();
    ~CButton();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
public:
    void UpdateRect() override;

public:
    // 콜백 함수 등록
    void SetOnClick(function<void()> callBack)  { m_onClick = callBack; }
    // 버튼 출력 문자열 등록
    void SetString(wstring str)                 { m_strBtn = move(str); }
    // 버튼 위치 및 크기 등록
    void SetPrintRect(RectF rcPrint)            { m_rcPrint = move(rcPrint); }
    // 이미지 Cell 크기 등록
    void SetCellSize(VEC vCellSize)             { m_vCellSize = move(vCellSize);}
    // 버튼 사용 가능
    void SetEnable(bool bEnable)                { m_bEnable = bEnable; }

public:
    void Click();

private:
    function<void()> m_onClick;   // 함수를 저장할 멤버 변수
    wstring          m_strBtn;    // 버튼 
    bool             m_bCol;      // 마우스 충돌 여부

    VEC              m_vCellSize; // 버튼 이미지 사이즈 
    RectF            m_rcPrint;   // 버튼 출력 위치/크기

    bool             m_bEnable;
};

