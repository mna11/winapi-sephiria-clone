#pragma once
#include "CStage.h"
#include "CState.h"

class CGolem;
class CErma;

enum class BOSS_STAGE_STATE
{
    WAIT,
    INTRO,
    BATTLE,
    FINAL,
    CLEAR,
    END
};

enum class BOSS_STAGE_INTRO_STATE
{
    CAMERA_MOVE,
    DIALOG1,
    EXCLAMTION,
    DIALOG2,
    DIALOG3,
    ENTER_GOLEM,
    FINISH,
    END
};

enum class BOSS_STAGE_FINAL_STATE
{
    CAMERA_MOVE1,
    DIALOG1,
    ENTER_GOLEM,
    DIALOG2,
    DIALOG3,
    BROKEN1,
    BROKEN2,
    DOWN,
    CAMERA_MOVE2,
    DIALOG4,
    FINISH,
    END
};

class CBossStage :
    public CStage, public CState<BOSS_STAGE_STATE>
{
public:
    CBossStage();
    ~CBossStage();
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;

public:
    void Init_CreateObj() override;
    void ApplyChange() override;

private:
    void InitializeRooms() override;
    void SpawnMonster(int iRoomIdx) override;
    void HandleCollision() override;

private:
    void UpdateTime();
    void UpdateIntroTime();
    void UpdateFinalTime();
    void ChangeIntroState(BOSS_STAGE_INTRO_STATE eState);
    void ChangeFinalState(BOSS_STAGE_FINAL_STATE eState);

private:
    CGolem* m_pGolem;
    CErma*  m_pErma;

    double                  m_dIntroTime;
    BOSS_STAGE_INTRO_STATE  m_eIntroState;

    double                  m_dFinalTime;
    BOSS_STAGE_FINAL_STATE  m_eFinalState;
};

