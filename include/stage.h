#ifndef STAGE_H
#define STAGE_H

#include "stage_types.h"

// 현재 스테이지 정보는 state 모듈에서 관리

void initStage(StageInfo* stage, int stageNumber);
void generateStageEnemies(StageInfo* stage);
void printStageInfo(const StageInfo* stage);
void showStageInfo();
void showBattleInterface();

#endif // STAGE_H 