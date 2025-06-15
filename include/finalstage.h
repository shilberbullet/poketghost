#ifndef FINALSTAGE_H
#define FINALSTAGE_H

#include "stage.h"
#include "yokai.h"
#include "battle.h"

// finalstage 정보 구조체
typedef struct {
    int stageNumber;
    char region[32];
    char terrain[32];
    int hour;
    int money;
    int enemyCount;
    Yokai enemies[4];  // 최대 4마리의 적 요괴
    int minLevel;
    int maxLevel;
} FinalStageInfo;

// finalstage 초기화
void initFinalStage(FinalStageInfo* stage, int stageNumber);

// finalstage 전투 시작
void startFinalStageBattle(FinalStageInfo* stage);

// finalstage 실행 조건 확인
int canEnterFinalStage(void);

// finalstage 진입
void enterFinalStage(void);

// finalstage 클리어 처리
void handleFinalStageClear(void);

#endif // FINALSTAGE_H 