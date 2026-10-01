#pragma once
#include "CUI.h"
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
    void SetMsgBoxID(MSG_BOX_ID eID) { m_eID = eID; }
    void SetMsgBoxQuestionID(MSG_BOX_QUESTION_ID eID) { m_eQuestionID = eID; }

public:
    const MSG_BOX_ID& GetMsgBoxID() const { return m_eID; }
    const MSG_BOX_QUESTION_ID& GetMsgBoxQuestionID() const { return m_eQuestionID; }

private:
    wstring m_string;
    MSG_BOX_ID m_eID;
    MSG_BOX_QUESTION_ID m_eQuestionID;
};

