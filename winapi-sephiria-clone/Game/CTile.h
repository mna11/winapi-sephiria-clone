#pragma once
#include "CObj.h"
class CTile :
    public CObj
{
public:
    CTile();
    virtual ~CTile();

public:
    void Initialize() override;
    int  Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;

public:
    int GetDrawID() const { return m_iDrawID; }
    int GetOption() const { return m_iOption; }
public:
    void SetDrawID(int iDrawID) { m_iDrawID = iDrawID; }
    void SetOption(int iOption) { m_iOption = iOption; }

private:
    int     m_iDrawID;
    int     m_iOption;
};