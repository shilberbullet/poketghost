#ifndef STAGE_H
#define STAGE_H

#include <stdbool.h>
#include "yokai.h"

#define REGION_NAME_MAX 32
#define MAX_ENEMIES 5

typedef enum {
    TERRAIN_MOUNTAIN,
    TERRAIN_RIVER,
    TERRAIN_SEA,
    TERRAIN_FIELD,
    TERRAIN_VILLAGE,
    TERRAIN_COUNT
} TerrainType;

typedef struct {
    int stageNumber;
    bool isBossStage;
    int minLevel;
    int maxLevel;
    int enemyCount;
    Yokai enemies[MAX_ENEMIES];
    char region[REGION_NAME_MAX];
    TerrainType terrain;
    int hour;
} StageInfo;

extern StageInfo currentStage;

void initStage(StageInfo* stage, int stageNumber);
void generateStageEnemies(StageInfo* stage);
void printStageInfo(const StageInfo* stage);
void showStageInfo();
void showBattleInterface();

#endif // STAGE_H 