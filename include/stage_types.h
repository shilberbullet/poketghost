#ifndef STAGE_TYPES_H
#define STAGE_TYPES_H

#include <stdbool.h>
#include "yokai.h"

#define REGION_NAME_MAX 32
#define MAX_ENEMIES 5
#define REGION_COUNT 30  // 지역 개수 정의

typedef enum {
    TERRAIN_MOUNTAIN,
    TERRAIN_RIVER,
    TERRAIN_SEA,
    TERRAIN_FIELD,
    TERRAIN_VILLAGE,
    TERRAIN_DIMENSIONAL_RIFT,  // 차원의 균열 지형 추가
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
    char terrainName[32];  // 지형 이름 저장용
    int hour;
} StageInfo;


// 지역 이름 배열 선언
extern const char* regions[REGION_COUNT];
extern const char* terrainNames[];

#endif // STAGE_TYPES_H 