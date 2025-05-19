#ifndef STAGE_H
#define STAGE_H

#include "types.h"

#define TERRAIN_COUNT 5

typedef struct {
    int stageNumber;
    char region[32];
    int terrain;
    int hour;
} StageInfo;

extern StageInfo currentStage;

void initStage();
void nextStage();
void showStageInfo();
void showBattleInterface();
void resetAllYokaiPP();
void clearStage();

#endif // STAGE_H 