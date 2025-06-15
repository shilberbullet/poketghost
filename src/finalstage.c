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

// 패러독스 요괴 및 최종보스 이름 배열
// (paradoxYokaiNames 관련 코드 완전 삭제)
const int paradoxYokaiCount = 9;
const char* finalBossName = "차원의 군주";

// finalstage 초기화 함수 (스테이지 번호에 따라 해당 요괴 세팅)
void initFinalStage(FinalStageInfo* stage, int stageNumber) {
    stage->stageNumber = stageNumber;
    strcpy(stage->region, FINAL_STAGE_REGION);
    strcpy(stage->terrain, FINAL_STAGE_TERRAIN);
    stage->hour = (stageNumber - 1) % 24;  // 시간 설정
    stage->money = 0;
    stage->enemyCount = 1;
    
    // 레벨 범위 계산
    calculateLevelRange(stageNumber, &stage->minLevel, &stage->maxLevel);
    int level = stage->minLevel + (rand() % (stage->maxLevel - stage->minLevel + 1));
    
    // 보스 스테이지(10의 배수)에서는 최종보스 출현
    if (stageNumber % 10 == 0) {
        Yokai* y = findYokaiByName(finalBossName);
        if (y) {
            stage->enemies[0] = *y;
            stage->enemies[0].level = level;  // 최종보스 레벨 설정
        } else {
            // 요괴를 찾지 못했을 때 yokai.txt를 다시 로드하여 찾기
            loadYokaiFromFile("data/yokai.txt");
            y = findYokaiByName(finalBossName);
            if (y) {
                stage->enemies[0] = *y;
                stage->enemies[0].level = level;  // 최종보스 레벨 설정
            }
        }
    } else {
        // 일반 스테이지에서는 패러독스 요괴 출현
        Yokai y = createRandomParadoxYokaiWithLevel(level);
        stage->enemies[0] = y;
    }
}

// finalstage 전투 시작 함수 (1마리씩 10스테이지)
void startFinalStageSequence(int startStage) {
    printText("\n=== 이계의 심연: 차원의 균열 ===\n");
    printText("이곳에서는 오직 패러독스 요괴와 최종보스만이 출현합니다!\n");
    fastSleep(1000);

    for (int i = startStage; i < startStage + paradoxYokaiCount + 1; i++) {
        FinalStageInfo stage;
        initFinalStage(&stage, i);
        
        // 전투 상태 변수 초기화
        turnCount = 1;
        lastYokaiIdx = 0;
        participatedCount = 0;
        
        // 스테이지 정보 표시
        char buffer[256];
        system("cls");  // 화면 지우기
        printText("=== 스테이지 정보 ===\n\n");
        sprintf(buffer, "스테이지: %d\n", i);
        printText(buffer);
        sprintf(buffer, "지역: %s\n", FINAL_STAGE_REGION);
        printText(buffer);
        sprintf(buffer, "지형: %s\n", FINAL_STAGE_TERRAIN);
        printText(buffer);
        sprintf(buffer, "시간: %02d시\n", stage.hour);
        printText(buffer);
        sprintf(buffer, "보유 전: %d전\n\n", gPlayer.money);
        printText(buffer);

        int result = startBattle(&stage.enemies[0]);
        if (result == 101 || result == 102) {
            printText("\n승리했습니다!\n");
            fastSleep(1000);
        } else if (result == 103) {
            printText("\n도망쳤습니다.\n");
            fastSleep(1000);
            return;
        } else if (result == 104) {
            printText("\n패배했습니다.\n");
            fastSleep(1000);
            return;
        }
    }
    handleFinalStageClear();
}

// finalstage 실행 조건 확인 함수
int canEnterFinalStage(void) {
    // 모든 지역을 방문했는지 확인
    if (!isAllRegionsVisited()) {
        return 0;
    }
    return 1;
}

// finalstage 진입 함수
void enterFinalStage(void) {
    if (!canEnterFinalStage()) {
        printText("\n아직 모든 지역을 방문하지 않았습니다.\n");
        return;
    }
    printText("\n=== 이계의 심연으로 진입합니다 ===\n");
    printText("모든 지역을 방문한 당신은 이제 이계의 심연에 도달했습니다.\n");
    printText("이곳에서 최종 전투가 시작됩니다!\n");
    fastSleep(2000);
    startFinalStageSequence(gStage.stageNumber);  // 현재 스테이지 번호 사용
}

// finalstage 클리어 처리 함수
void handleFinalStageClear(void) {
    printText("\n=== 축하합니다! ===\n");
    printText("당신은 모든 지역을 방문하고 이계의 심연을 정복했습니다!\n");
    printText("게임 클리어를 축하합니다!\n");
    fastSleep(2000);
    
    // 게임 종료 처리
    exit(0);
} 