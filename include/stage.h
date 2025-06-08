#ifndef STAGE_H
#define STAGE_H

#include "stage_types.h"

extern StageInfo currentStage;

void initStage(StageInfo* stage, int stageNumber);
void generateStageEnemies(StageInfo* stage);
void printStageInfo(const StageInfo* stage);
void showStageInfo();
void showBattleInterface();

#endif // STAGE_H 