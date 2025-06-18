#ifndef STAGE_H
#define STAGE_H

#include "stage_types.h"

// 현재 스테이지 정보는 state 모듈에서 관리

void initStage(StageInfo* stage, int stageNumber);
void generateStageEnemies(StageInfo* stage);
void printStageInfo(const StageInfo* stage);
void showStageInfo();
void showBattleInterface();

// 현재 지형 이름을 반환하는 함수
const char* getCurrentTerrain(void);

#define FINAL_TERRAIN_COUNT 5
#define FINAL_TERRAIN_SEQUENCE_LENGTH 10
extern const char* finalTerrainNames[];
extern const int terrainSequence[];

#endif // STAGE_H 