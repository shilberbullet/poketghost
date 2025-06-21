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
#include "event_system.h"
#include "logger.h"

// 일반 스테이지 초기화 함수
void initNormalStage(StageInfo* stage, int stageNumber) {
    LOG_FUNCTION_EXECUTION("initNormalStage");
    stage->stageNumber = stageNumber;      // 스테이지 번호 설정
    stage->isBossStage = false;            // 일반 스테이지 플래그 설정
    
    // 현재 지역 설정
    strcpy(stage->region, getCurrentRegion());
    
    // 랜덤 지형 설정 (차원의 균열 제외)
    stage->terrain = rand() % (TERRAIN_COUNT - 1);
    strcpy(stage->terrainName, terrainNames[stage->terrain]);
    
    // 레벨 범위 계산
    calculateLevelRange(stageNumber, &stage->minLevel, &stage->maxLevel);
    
    // 이벤트 발생 확률 체크
    if (shouldTriggerEvent()) {
        // 이벤트 발생 시 요괴 없이 이벤트만 실행
        stage->enemyCount = 0;
        Event* event = generateRandomEvent();
        if (event) {
            handleEvent(event);
        }
    } else {
        // 일반 스테이지는 3-5마리
        stage->enemyCount = rand() % 3 + 3;
        // 적 요괴 생성
        generateNormalStageEnemies(stage);
    }
}

// 일반 스테이지 적 요괴 생성 함수
void generateNormalStageEnemies(StageInfo* stage) {
    LOG_FUNCTION_EXECUTION("generateNormalStageEnemies");
    for (int i = 0; i < stage->enemyCount; i++) {
        int level = stage->minLevel + (rand() % (stage->maxLevel - stage->minLevel + 1));
        stage->enemies[i] = createRandomYokaiWithLevel(level);
    }
}

// 일반 스테이지 클리어 처리 함수
void handleNormalStageClear(void) {
    LOG_FUNCTION_EXECUTION("handleNormalStageClear");
    
    // 활성 이벤트가 있는지 확인하고 완료 조건 체크
    Event* currentEvent = getCurrentEvent();
    if (currentEvent && checkEventCompletion(currentEvent)) {
        completeEvent(currentEvent);
    }
    
    // 일반 스테이지 클리어 시 처리
} 