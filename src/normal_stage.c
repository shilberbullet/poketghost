#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "normal_stage.h"
#include "text.h"
#include "yokai.h"
#include "party.h"
#include "savefile.h"

void initNormalStage(StageInfo* stage, int stageNumber) {
    stage->stageNumber = stageNumber;
    stage->isBossStage = false;
    // 지역 초기화
    size_t regionIndex = (stageNumber - 1) / 10;
    if (regionIndex < REGION_COUNT) {
        strcpy(stage->region, regions[regionIndex]);
    }
    
    // 레벨 범위 계산
    calculateLevelRange(stageNumber, &stage->minLevel, &stage->maxLevel);
    
    // 일반 스테이지는 3-5마리
    stage->enemyCount = rand() % 3 + 3;
    
    // 적 요괴 생성
    generateNormalStageEnemies(stage);
}

void generateNormalStageEnemies(StageInfo* stage) {
    for (int i = 0; i < stage->enemyCount; i++) {
        int level = stage->minLevel + (rand() % (stage->maxLevel - stage->minLevel + 1));
        stage->enemies[i] = createRandomYokaiWithLevel(level);
    }
}

void handleNormalStageClear(void) {
    // 일반 스테이지 클리어 시 처리
    // 5의 배수 스테이지 완료 시 자동 저장
    if ((currentStage.stageNumber - 1) % 5 == 0) {
        saveGame();
    }
} 