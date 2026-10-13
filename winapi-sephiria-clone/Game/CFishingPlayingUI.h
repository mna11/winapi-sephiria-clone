#pragma once
#include "CUI.h"

class CFishingPlayingUI :
    public CUI
{
public:
    CFishingPlayingUI();
    ~CFishingPlayingUI();
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
    const float& GetGauge() const { return m_fCatchGauge; }

private:
    void UpdateTime();
    void Move();
    void UpdateRect() override;

private:
    void MoveFish();
    void MoveNet();

private:
    void UpdateFishRect();
    void UpdateNetRect();

private:
    void HandleCollision();

private:
    float       m_fFishSpeed; // 물고기 속도 
    float       m_fNetSpeed;  // 네트 속도

    VEC         m_vFish;       // 물고기 위치
    VEC         m_vNet;        // 네트 위치

    RECT        m_rcFish;      // 물고기 충돌 박스
    RECT        m_rcNet;       // 네트 박스
    
    float       m_fCatchGauge; // 게이지

    double      m_dFishChangeMoveElapseTime;    // 물고기가 방향과 속도를 바꾼 뒤 흐른 시간
    double      m_dFishChangeMoveInterval;      // 물고기가 방향과 속도를 바꾸는 인터벌
    bool        m_bFishChangeMove;              // 이번 프레임에 물고기가 방향과 속도를 바꿔야하는지 여부

    ImageAttributes m_imgAttrAlpha;             // Panel 반투명
};

