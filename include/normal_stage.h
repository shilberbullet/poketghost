#ifndef NORMAL_STAGE_H
#define NORMAL_STAGE_H

#include "stage_types.h"

void initNormalStage(StageInfo* stage, int stageNumber);
void generateNormalStageEnemies(StageInfo* stage);
void handleNormalStageClear(void);

#endif // NORMAL_STAGE_H 