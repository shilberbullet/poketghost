#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "stage.h"
#include "yokai.h"
#include "party.h"
#include "text.h"
#include "input.h"
#include "game.h"
#include "item.h"
#include "region.h"
#include "hp_system.h"
#include "savefile.h"
#include "finalstage.h"
#include "../core/state.h"
#include <windows.h>

// 새로운 지역 및 지형 정의
#define FINAL_STAGE_REGION "이계의 심연"
#define FINAL_STAGE_TERRAIN "차원의 균열"

// finalstage 전용 요괴 목록 (예시)
const char* finalStageYokaiNames[] = {
    "이계의 망령",
    "차원 파수꾼",
    "심연의 그림자",
    "운명의 조각자"
};
const int finalStageYokaiCount = 4;

// finalstage 초기화 함수
void initFinalStage(FinalStageInfo* stage, int stageNumber) {
    stage->stageNumber = stageNumber;
    strcpy(stage->region, FINAL_STAGE_REGION);
    strcpy(stage->terrain, FINAL_STAGE_TERRAIN);
    stage->hour = 0;
    stage->money = 0;
    stage->enemyCount = finalStageYokaiCount;
    // 전용 요괴 생성
    for (int i = 0; i < finalStageYokaiCount; i++) {
        Yokai* y = findYokaiByName(finalStageYokaiNames[i]);
        if (y) {
            stage->enemies[i] = *y;
        } else {
            memset(&stage->enemies[i], 0, sizeof(Yokai)); // 못 찾으면 빈 값
        }
    }
}

// finalstage 전투 시작 함수 (예시)
void startFinalStageBattle(FinalStageInfo* stage) {
    printText("\n=== 이계의 심연: 차원의 균열 ===\n");
    printText("이곳에서는 오직 이계의 요괴들만이 출현합니다!\n");
    for (int i = 0; i < stage->enemyCount; i++) {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "적 요괴: %s\n", stage->enemies[i].name);
        printText(buffer);
    }
    // 실제 전투 로직은 기존 battle 시스템과 연동 필요
}

// 실행 조건 및 진입 함수는 추후 구현 예정 