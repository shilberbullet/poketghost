// 표준 입출력, 문자열, 게임 관련 헤더
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "normal_stage.h"
#include "text.h"
#include "yokai.h"
#include "party.h"
#include "savefile.h"
#include "region.h"
#include "stage.h"
#include "stage_types.h"

// 일반 스테이지 초기화 함수
void initNormalStage(StageInfo* stage, int stageNumber) {
    stage->stageNumber = stageNumber;      // 스테이지 번호 설정
    stage->isBossStage = false;            // 일반 스테이지 플래그 설정
    
    // 현재 지역 설정
    strcpy(stage->region, getCurrentRegion());
    
    // 랜덤 지형 설정 (차원의 균열 제외)
    stage->terrain = rand() % (TERRAIN_COUNT - 1);
    strcpy(stage->terrainName, terrainNames[stage->terrain]);
    
    // 레벨 범위 계산
    calculateLevelRange(stageNumber, &stage->minLevel, &stage->maxLevel);
    
    // 일반 스테이지는 3-5마리
    stage->enemyCount = rand() % 3 + 3;
    
    // 적 요괴 생성
    generateNormalStageEnemies(stage);
}

// 일반 스테이지 적 요괴 생성 함수
void generateNormalStageEnemies(StageInfo* stage) {
    for (int i = 0; i < stage->enemyCount; i++) {
        int level = stage->minLevel + (rand() % (stage->maxLevel - stage->minLevel + 1));
        stage->enemies[i] = createRandomYokaiWithLevel(level);
    }
}

// 일반 스테이지 클리어 처리 함수
void handleNormalStageClear(void) {
    // 일반 스테이지 클리어 시 처리
} 