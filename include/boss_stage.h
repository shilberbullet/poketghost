#ifndef BOSS_STAGE_H
#define BOSS_STAGE_H

#include "stage_types.h"

void initBossStage(StageInfo* stage, int stageNumber);
void generateBossStageEnemies(StageInfo* stage);
void handleBossStageClear(void);

#endif // BOSS_STAGE_H 