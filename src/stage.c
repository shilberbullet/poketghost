// 표준 입출력, 메모리, 시간, 문자열, 게임 관련 헤더
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "stage.h"
#include "normal_stage.h"
#include "boss_stage.h"
#include "text.h"
#include "input.h"
#include "battle.h"
#include "savefile.h"
#include "party.h"
#include "yokai.h"
#include "game.h"
#include "region.h"
#include "item.h"
#include "../core/state.h"

// 지형 이름 배열
const char* terrainNames[] = {
    "산", "강", "바다", "논", "마을"
};

// 스테이지 정보는 state 모듈에서 관리

// 스테이지 초기화 함수
void initStage(StageInfo* stage, int stageNumber) {
    if (stageNumber % 10 == 0) {
        initBossStage(stage, stageNumber);  // 10의 배수 스테이지는 보스 스테이지
    } else {
        initNormalStage(stage, stageNumber);  // 그 외는 일반 스테이지
    }
    
    // 새 게임일 경우 시간을 0으로 초기화
    if (gGameState.isNewGame) {
        stage->hour = 0;
        gGameState.isNewGame = 0;  // 플래그 초기화
    }
    
    // 랜덤 지형 설정
    stage->terrain = rand() % TERRAIN_COUNT;
}

// 다음 스테이지로 진행하는 함수
void nextStage() {
    gStage.stageNumber++;  // 스테이지 번호 증가
    turnCount = 0;              // 턴 카운트 초기화
    gStage.hour = (gStage.hour + 1) % 24;  // 시간 증가 (24시간 주기)
    gStage.terrain = rand() % TERRAIN_COUNT;     // 랜덤 지형 설정
    
    // 10스테이지마다 지역 변경
    if (gStage.stageNumber % 10 == 1) {  // 1, 11, 21... 스테이지에서 지역 변경
        int hasMap = 0;
        int mapIdx = -1;
        for (int i = 0; i < inventoryCount; i++) {
            if (strcmp(inventory[i].item.name, "지도") == 0 && inventory[i].count > 0) {
                hasMap = 1;
                mapIdx = i;
                break;
            }
        }
        if (hasMap) {
            if (moveToNextRegionWithMap()) {
                char buffer[256];
                sprintf(buffer, "\n지도 아이템을 사용하여 %s로 이동했습니다.\n", getCurrentRegion());
                printTextAndWait(buffer);
                displayConnectedRegions();
            }
        } else {
            if (moveToNextRegion()) {
                char buffer[256];
                sprintf(buffer, "\n%s로 이동했습니다.\n", getCurrentRegion());
                printTextAndWait(buffer);
                displayConnectedRegions();
            }
        }
    }
    
    // 새로운 스테이지 초기화 (hour, terrain은 유지)
    initStage(&gStage, gStage.stageNumber);
    
    // 5의 배수 스테이지 완료 시 자동 저장 (수동 저장이 아닌 경우에만)
    if ((gStage.stageNumber - 1) % 5 == 0 && !gGameState.isLoadedGame && !gGameState.isManualSave) {
        saveGame();
        char buffer[128];
        sprintf(buffer, "\n%d스테이지 완료! 게임이 자동 저장되었습니다.\n", gStage.stageNumber - 1);
        printTextAndWait(buffer);
    }
}

// 스테이지 정보를 표시하는 함수
void showStageInfo() {
    char buffer[256];
    system("cls");  // 화면 지우기
    printText("=== 스테이지 정보 ===\n\n");
    sprintf(buffer, "스테이지: %d\n", gStage.stageNumber);  // 스테이지 번호
    printText(buffer);
    sprintf(buffer, "지역: %s\n", getCurrentRegion());  // 현재 지역명 출력
    printText(buffer);
    sprintf(buffer, "지형: %s\n", terrainNames[gStage.terrain]);  // 지형 이름
    printText(buffer);
    int hour = (gStage.stageNumber - 1) % 24;  // 현재 시간
    sprintf(buffer, "시간: %02d시\n", hour);
    printText(buffer);
    sprintf(buffer, "보유 전: %d전\n\n", gPlayer.money);  // 보유 금액
    printText(buffer);
}

// 전투 인터페이스를 표시하고 전투를 시작하는 함수
void showBattleInterface() {
    int minLevel, maxLevel;
    calculateLevelRange(gStage.stageNumber, &minLevel, &maxLevel);  // 레벨 범위 계산
    int randomLevel = minLevel + rand() % (maxLevel - minLevel + 1);      // 랜덤 레벨 선택

    Yokai enemy;
    if (gGameState.isLoadedGame) {
        enemy = currentEnemy;  // 이어하기 시 저장된 적 요괴 사용
        gGameState.isLoadedGame = 0;  // 플래그 초기화
    } else if (gStage.isBossStage) {
        enemy = gStage.enemies[0];  // 보스 스테이지의 경우 첫 번째 적 요괴 사용
    } else {
        enemy = gStage.enemies[rand() % gStage.enemyCount];  // 랜덤 적 요괴 선택
    }

    int battleResult = startBattle(&enemy);  // 전투 시작

    if (gStage.isBossStage) {
        if (battleResult == 101 || battleResult == 102) {  // 승리한 경우
            handleBossStageClear();  // 보스 스테이지 클리어 처리
            nextStage();             // 다음 스테이지로 진행
        }
    } else {
        handleNormalStageClear();  // 일반 스테이지 클리어 처리
        nextStage();               // 다음 스테이지로 진행
    }
} 