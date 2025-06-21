/**
 * @file stage_types.h
 * @brief 포켓요괴 게임의 스테이지 타입 정의 헤더 파일
 * @details 지형 타입, 스테이지 정보 구조체 등의 정의
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef STAGE_TYPES_H
#define STAGE_TYPES_H

#include <stdbool.h> // 불린 타입 사용을 위한 헤더
#include "yokai.h" // 요괴 관련 헤더

#define REGION_NAME_MAX 32 // 지역 이름의 최대 길이
#define MAX_ENEMIES 5 // 스테이지당 최대 적 수
#define REGION_COUNT 30  // 지역 개수 정의

/**
 * @brief 지형 타입 열거형
 * @details 게임에서 사용되는 다양한 지형들을 정의
 */
typedef enum {
    TERRAIN_MOUNTAIN, // 산 지형
    TERRAIN_RIVER, // 강 지형
    TERRAIN_SEA, // 바다 지형
    TERRAIN_FIELD, // 들판 지형
    TERRAIN_VILLAGE, // 마을 지형
    TERRAIN_DIMENSIONAL_RIFT,  // 차원의 균열 지형 추가
    TERRAIN_COUNT // 지형 타입 개수
} TerrainType;

/**
 * @brief 스테이지 정보 구조체
 * @details 스테이지의 모든 정보를 담는 구조체
 */
typedef struct {
    int stageNumber; // 스테이지 번호
    bool isBossStage; // 보스 스테이지 여부
    int minLevel; // 최소 레벨
    int maxLevel; // 최대 레벨
    int enemyCount; // 적 수
    Yokai enemies[MAX_ENEMIES]; // 적 요괴 배열
    char region[REGION_NAME_MAX]; // 지역명
    TerrainType terrain; // 지형 타입
    char terrainName[32];  // 지형 이름 저장용
    int hour; // 시간
} StageInfo;

// 지역 이름 배열 선언
extern const char* regions[REGION_COUNT]; // 모든 지역 이름 배열
extern const char* terrainNames[]; // 모든 지형 이름 배열

#endif // STAGE_TYPES_H 