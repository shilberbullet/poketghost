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
#include "logger.h"
#include <windows.h>

// 보스 스테이지 초기화 함수
void initBossStage(StageInfo* stage, int stageNumber) {
    LOG_FUNCTION_EXECUTION("initBossStage");
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
    LOG_FUNCTION_EXECUTION("generateBossStageEnemies");
    int level = stage->minLevel + (rand() % (stage->maxLevel - stage->minLevel + 1));
    stage->enemies[0] = createRandomBossYokaiWithLevel(level);
}

// 보스 스테이지 클리어 처리 함수
// 반환값: 1 = finalstage 진입 조건 충족, 0 = 일반 보스 클리어
int handleBossStageClear(void) {
    LOG_FUNCTION_EXECUTION("handleBossStageClear");
    // 보스 스테이지 클리어 시 처리
    resetAllYokaiPP();
    printTextAndWait("\n모든 동료 요괴의 기술 PP가 초기화되었습니다!");
    // 모든 동료 요괴의 HP와 상태 회복
    for (int i = 0; i < gPartyCount; i++) {
        float maxHP = calculateHP(&gParty[i]);
        gParty[i].currentHP = maxHP;
        gParty[i].status = YOKAI_NORMAL;
    }
    printTextAndWait("\n모든 동료 요괴의 HP와 상태가 회복되었습니다!");
    fastSleep(500);
    saveGame();
    // 60스테이지 클리어 시 파이널 스테이지 진입 조건 체크 제거
    return 0;
} 