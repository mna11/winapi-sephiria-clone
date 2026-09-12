#pragma once
#include "CObj.h"
class CCamera :
    public CObj
{
public:
    enum class STATE { NORMAL, HIT, END };

public:
    CCamera();
    ~CCamera();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void	SetTarget(CObj* pObj) override;

private:
    STATE m_eCurState;
    STATE m_ePreState; 
};

