#ifndef BOSS_STAGE_H
#define BOSS_STAGE_H

#include "stage_types.h"

void initBossStage(StageInfo* stage, int stageNumber);
void generateBossStageEnemies(StageInfo* stage);
// 반환값: 1 = finalstage 진입 조건 충족, 0 = 일반 보스 클리어
int handleBossStageClear(void);

#endif // BOSS_STAGE_H 