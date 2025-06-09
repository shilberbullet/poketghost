#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boss_stage.h"
#include "text.h"
#include "yokai.h"
#include "party.h"
#include "savefile.h"
#include "hp_system.h"
#include "region.h"
#include "../core/state.h"

// 보스 스테이지 초기화 함수
void initBossStage(StageInfo* stage, int stageNumber) {
    stage->stageNumber = stageNumber;      // 스테이지 번호 설정
    stage->isBossStage = true;             // 보스 스테이지 플래그 설정
    // stage->hour = 0;  // 시간 초기화 제거
    
    // 현재 지역 설정
    strcpy(stage->region, getCurrentRegion());
    
    // 레벨 범위 계산
    calculateLevelRange(stageNumber, &stage->minLevel, &stage->maxLevel);
    
    // 보스 스테이지는 1마리
    stage->enemyCount = 1;
    
    // 적 요괴 생성
    generateBossStageEnemies(stage);
}

// 보스 스테이지 적 요괴 생성 함수
void generateBossStageEnemies(StageInfo* stage) {
    int level = stage->minLevel + (rand() % (stage->maxLevel - stage->minLevel + 1));
    stage->enemies[0] = createRandomBossYokaiWithLevel(level);
}

// 보스 스테이지 클리어 처리 함수
void handleBossStageClear(void) {
    // 보스 스테이지 클리어 시 처리
    resetAllYokaiPP();
    printTextAndWait("\n모든 동료 요괴의 기술 PP가 초기화되었습니다!");
    
    // 모든 동료 요괴의 HP와 상태 회복
    for (int i = 0; i < partyCount; i++) {
        float maxHP = calculateHP(&party[i]);
        party[i].currentHP = maxHP;
        party[i].status = YOKAI_NORMAL;
    }
    printTextAndWait("\n모든 동료 요괴의 HP와 상태가 회복되었습니다!");
    
    saveGame();
} 