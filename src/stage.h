#ifndef STAGE_H
#define STAGE_H

#define REGION_NAME_MAX 32

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
    char region[REGION_NAME_MAX];
    TerrainType terrain;
    int hour;
} StageInfo;

extern StageInfo currentStage;

void initStage();
void nextStage();
void showStageInfo();
void showBattleInterface();

#endif // STAGE_H 