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
#include "event_system.h"
#include "../core/state.h"
#include "logger.h"
#include <windows.h>
#include "statistics.h"
#include "dialogue.h"

// 지형 이름 배열
const char* terrainNames[] = {
    "산", "강", "바다", "논", "마을", "차원의 균열"
};

// 파이널 스테이지 지형 순서 정의
const int terrainSequence[] = {
    0, 0,  // 백두산 입구 (2개)
    1, 1,  // 백두산 숲길 (2개)
    2, 2,  // 백두산 계곡 (2개)
    3, 3, 3,  // 백두산 중턱 (3개)
    4  // 백두산 천지 (1개)
};

const char* finalTerrainNames[] = {
    "백두산 입구",
    "백두산 숲길",
    "백두산 계곡",
    "백두산 중턱",
    "백두산 천지"
};

// 스테이지 정보는 state 모듈에서 관리

// 스테이지 초기화 함수
void initStage(StageInfo* stage, int stageNumber) {
    LOG_FUNCTION_EXECUTION("initStage");
    if (stageNumber >= 81) {
        // 파이널 스테이지 초기화
        stage->stageNumber = stageNumber;
        stage->isBossStage = false;
        strcpy(stage->region, "백두산");
        
        // 스테이지 번호에 따른 지형 설정 (81부터 시작)
        int terrainIndex = stageNumber - 81;
        if (terrainIndex >= 0 && terrainIndex < FINAL_TERRAIN_SEQUENCE_LENGTH) {
            stage->terrain = terrainSequence[terrainIndex];
            strcpy(stage->terrainName, finalTerrainNames[terrainSequence[terrainIndex]]);
        } else {
            stage->terrain = FINAL_TERRAIN_COUNT - 1;
            strcpy(stage->terrainName, finalTerrainNames[FINAL_TERRAIN_COUNT - 1]);
        }
        
        // 레벨 범위 계산
        calculateLevelRange(stageNumber, &stage->minLevel, &stage->maxLevel);
        int level = stage->minLevel + (rand() % (stage->maxLevel - stage->minLevel + 1));
        
        // 보스 스테이지(90스테이지)에서는 최종보스 출현
        if (stageNumber == 90) {
            // 적 배열 전체 초기화
            memset(stage->enemies, 0, sizeof(stage->enemies));
            stage->enemyCount = 1;
            Yokai* y = findYokaiByName("두억시니");
            if (y) {
                stage->enemies[0] = *y;
                stage->enemies[0].level = level;
                // HP 계산 및 설정
                stage->enemies[0].currentHP = calculateHP(&stage->enemies[0]);
                
                // 최종보스는 모든 기술을 사용할 수 있도록 설정
                stage->enemies[0].moveCount = stage->enemies[0].learnableMoveCount;
                if (stage->enemies[0].moveCount > MAX_MOVES) {
                    stage->enemies[0].moveCount = MAX_MOVES;
                }
                
                // 모든 기술을 moves 배열에 복사
                for (int i = 0; i < stage->enemies[0].moveCount; i++) {
                    stage->enemies[0].moves[i].move = stage->enemies[0].learnableMoves[i];
                    stage->enemies[0].moves[i].currentPP = stage->enemies[0].learnableMoves[i].pp;
                }
                
                printf("[DEBUG] 최종보스 생성 성공: %s, 레벨: %d, HP: %.1f, 기술 수: %d\n", 
                       stage->enemies[0].name, stage->enemies[0].level, stage->enemies[0].currentHP, stage->enemies[0].moveCount);
            } else {
                printf("[DEBUG] 두억시니를 찾을 수 없음, yokai.txt 재로드 시도\n");
                loadYokaiFromFile("data/yokai.txt");
                y = findYokaiByName("두억시니");
                if (y) {
                    stage->enemies[0] = *y;
                    stage->enemies[0].level = level;
                    // HP 계산 및 설정
                    stage->enemies[0].currentHP = calculateHP(&stage->enemies[0]);
                    
                    // 최종보스는 모든 기술을 사용할 수 있도록 설정
                    stage->enemies[0].moveCount = stage->enemies[0].learnableMoveCount;
                    if (stage->enemies[0].moveCount > MAX_MOVES) {
                        stage->enemies[0].moveCount = MAX_MOVES;
                    }
                    
                    // 모든 기술을 moves 배열에 복사
                    for (int i = 0; i < stage->enemies[0].moveCount; i++) {
                        stage->enemies[0].moves[i].move = stage->enemies[0].learnableMoves[i];
                        stage->enemies[0].moves[i].currentPP = stage->enemies[0].learnableMoves[i].pp;
                    }
                    
                    printf("[DEBUG] 최종보스 생성 성공(재로드 후): %s, 레벨: %d, HP: %.1f, 기술 수: %d\n", 
                           stage->enemies[0].name, stage->enemies[0].level, stage->enemies[0].currentHP, stage->enemies[0].moveCount);
                } else {
                    printf("[DEBUG] 두억시니를 여전히 찾을 수 없음\n");
                }
            }
        } else {
            // 일반 스테이지에서는 패러독스 요괴 출현
            stage->enemyCount = 1;
            stage->enemies[0] = createRandomParadoxYokaiWithLevel(level);
        }
    } else if (stageNumber % 10 == 0) {
        initBossStage(stage, stageNumber);  // 10의 배수 스테이지는 보스 스테이지
    } else {
        initNormalStage(stage, stageNumber);  // 그 외는 일반 스테이지
    }
    
    // 새 게임일 경우 시간을 0으로 초기화
    if (gGameState.isNewGame) {
        stage->hour = 0;
        gGameState.isNewGame = 0;  // 플래그 초기화
    }
}

// 다음 스테이지로 진행하는 함수
void nextStage() {
    LOG_FUNCTION_EXECUTION("nextStage");
    gStage.stageNumber++;  // 스테이지 번호 증가
    gPlayer.stats.stages_completed++; // 클리어한 스테이지 수 증가
    turnCount = 0;              // 턴 카운트 초기화
    gStage.hour = (gStage.hour + 1) % 24;  // 시간 증가 (24시간 주기)
    
    // 10스테이지마다 지역 변경
    if (gStage.stageNumber % 10 == 1) {  // 1, 11, 21... 스테이지에서 지역 변경
        // 81스테이지부터 파이널 스테이지로 진입
        if (gStage.stageNumber >= 81) {
            strcpy(gStage.region, "백두산");
            initStage(&gStage, gStage.stageNumber);
            return;
        }

        int hasMap = 0;
        for (int i = 0; i < inventoryCount; i++) {
            if (strcmp(inventory[i].item.name, "지도") == 0 && inventory[i].count > 0) {
                hasMap = 1;
                break;
            }
        }
        if (hasMap) {
            if (moveToNextRegionWithMap()) {
                char buffer[256];
                sprintf(buffer, "\n지도 아이템을 사용하여 %s로 이동했습니다.\n", getCurrentRegion());
                printText(buffer);
                displayConnectedRegions();
            }
        } else {
            if (moveToNextRegion()) {
                char buffer[256];
                sprintf(buffer, "\n%s로 이동했습니다.\n", getCurrentRegion());
                printText(buffer);
                displayConnectedRegions();
            }
        }
    }
    
    // 새로운 스테이지 초기화
    initStage(&gStage, gStage.stageNumber);
    
    // 5의 배수 스테이지 완료 시 자동 저장 (수동 저장이 아닌 경우에만)
    if ((gStage.stageNumber - 1) % 5 == 0 && !gGameState.isLoadedGame && !gGameState.isManualSave) {
        saveGame();
        char buffer[128];
        sprintf(buffer, "\n%d스테이지 완료! 게임이 자동 저장되었습니다.\n", gStage.stageNumber - 1);
        printText(buffer);
        fastSleep(500);
    }
}

// 스테이지 정보를 표시하는 함수
void showStageInfo() {
    LOG_FUNCTION_EXECUTION("showStageInfo");
    char buffer[256];
    system("cls");  // 화면 지우기
    printText("=== 스테이지 정보 ===\n\n");
    sprintf(buffer, "스테이지: %d\n", gStage.stageNumber);  // 스테이지 번호
    printText(buffer);
    sprintf(buffer, "지역: %s\n", gStage.region);  // 현재 지역명 출력
    printText(buffer);
    sprintf(buffer, "지형: %s\n", gStage.terrainName);  // 지형 이름
    printText(buffer);
    int hour = (gStage.stageNumber - 1) % 24;  // 현재 시간
    sprintf(buffer, "시간: %02d시\n", hour);
    printText(buffer);
    sprintf(buffer, "보유 전: %d전\n\n", gPlayer.money);  // 보유 금액
    printText(buffer);
}

// 전투 인터페이스를 표시하고 전투를 시작하는 함수
void showBattleInterface() {
    LOG_FUNCTION_EXECUTION("showBattleInterface");
    
    // 이벤트가 발생한 경우(enemyCount가 0) 전투를 건너뛰고 다음 스테이지로 진행
    if (gStage.enemyCount == 0) {
        handleNormalStageClear();  // 일반 스테이지 클리어 처리
        nextStage();               // 다음 스테이지로 진행
        // 이벤트 처리 후에는 showStageInfo를 호출하지 않음 (이미 이벤트 UI에서 출력됨)
        return;
    }
    
    int minLevel, maxLevel;
    calculateLevelRange(gStage.stageNumber, &minLevel, &maxLevel);  // 레벨 범위 계산

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
            nextStage();  // 다음 스테이지로 진행
        }
    } else if (gStage.stageNumber >= 81) {  // 파이널 스테이지
        if (battleResult == 101 || battleResult == 102) {  // 승리한 경우
            if (gStage.stageNumber == 90) {  // 최종 보스 클리어
                startDialogue(3000);
                printText("게임 클리어를 축하합니다!\n");
                gPlayer.stats.games_cleared++; // 게임 클리어 횟수 증가
                update_total_statistics_on_save(); // 통계 저장
                // 잡은 요괴 데이터 저장
                saveCaughtYokaiData();
                fastSleep(2000);
                resetGameAfterClear();  // 게임 데이터 초기화
                exit(0);
            }
            nextStage();  // 다음 스테이지로 진행
        }
    } else {
        handleNormalStageClear();  // 일반 스테이지 클리어 처리
        nextStage();               // 다음 스테이지로 진행
    }
}

// 현재 스테이지의 지형 이름을 반환하는 함수
const char* getCurrentTerrain(void) {
    LOG_FUNCTION_EXECUTION("getCurrentTerrain");
    return gStage.terrainName;
} 