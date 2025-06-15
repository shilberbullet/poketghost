#ifndef FINALSTAGE_H
#define FINALSTAGE_H

#include "yokai.h"

// finalstage 스테이지 정보 구조체
typedef struct {
    int stageNumber;
    char region[32];
    char terrain[32];
    int hour;
    int money;
    int enemyCount;
    Yokai enemies[4];
} FinalStageInfo;

void initFinalStage(FinalStageInfo* stage, int stageNumber);
void startFinalStageBattle(FinalStageInfo* stage);

#endif // FINALSTAGE_H 