#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "savefile.h"
#include "stage.h"
#include "party.h"
#include "yokai.h"
#include "game.h"
#include "settings.h"
#include "item.h"
#include "battle.h"
#include "text.h"
#include "region.h"
#include "../core/state.h"
#include "logger.h"
#include "statistics.h"
#include <windows.h>
#define SAVE_FILE "data/save.dat"

// battle.c에서 선언된 변수들을 extern으로 선언
extern int turnCount;
extern int participatedCount;
extern int participatedIdx[];
extern int lastYokaiIdx;

char currentRegion[32]; // 현재 지역명을 저장할 변수

// 게임 저장 함수
void saveGame() {
    LOG_FUNCTION_EXECUTION("saveGame");
    
    // 통계 업데이트
    update_total_statistics_on_save();

    FILE* file = fopen(SAVE_FILE, "wb");  // 바이너리 쓰기 모드로 파일 열기
    if (!file) {
        printText("저장 실패!\n");
        return;
    }
    
    if (gameSettings.debugMode) {
    char buffer[256];
    sprintf(buffer, "[DEBUG] 저장 시 turnCount: %d\n지역: %s\n지형: %s\n", turnCount, gStage.region, gStage.terrainName);
    printTextAndWait(buffer);
    fastSleep(500);
    }
    
    // 지역 데이터 먼저 저장
    saveRegionData(file);
    
    // 스테이지 정보 저장
    fwrite(&gStage, sizeof(StageInfo), 1, file);
    
    // 동료 요괴 수 저장
    fwrite(&gPartyCount, sizeof(int), 1, file);
    
    // 동료 요괴 정보 저장
    for (int i = 0; i < gPartyCount; i++) {
        fwrite(&gParty[i].id, sizeof(unsigned long long), 1, file);
        fwrite(&gParty[i].name, sizeof(char), YOKAI_NAME_MAX, file);
        fwrite(&gParty[i].level, sizeof(int), 1, file);
        fwrite(&gParty[i].exp, sizeof(int), 1, file);
        fwrite(&gParty[i].attack, sizeof(int), 1, file);
        fwrite(&gParty[i].defense, sizeof(int), 1, file);
        fwrite(&gParty[i].stamina, sizeof(int), 1, file);
        fwrite(&gParty[i].speed, sizeof(int), 1, file);
        fwrite(&gParty[i].currentHP, sizeof(float), 1, file);
        fwrite(&gParty[i].type, sizeof(YokaiType), 1, file);
        fwrite(&gParty[i].status, sizeof(YokaiStatus), 1, file);
        fwrite(&gParty[i].desc, sizeof(char), YOKAI_DESC_MAX, file);
        fwrite(&gParty[i].moveCount, sizeof(int), 1, file);
        
        // 기술 정보 저장
        for (int j = 0; j < gParty[i].moveCount; j++) {
            fwrite(&gParty[i].moves[j].move, sizeof(Move), 1, file);
            fwrite(&gParty[i].moves[j].currentPP, sizeof(int), 1, file);
        }
        
        // 배울 수 있는 기술 목록 저장
        fwrite(&gParty[i].learnableMoveCount, sizeof(int), 1, file);
        for (int j = 0; j < gParty[i].learnableMoveCount; j++) {
            fwrite(&gParty[i].learnableMoves[j], sizeof(Move), 1, file);
        }
        
        // 잊은 기술 목록 저장
        fwrite(&gParty[i].forgottenMoveCount, sizeof(int), 1, file);
        for (int j = 0; j < gParty[i].forgottenMoveCount; j++) {
            fwrite(&gParty[i].forgottenMoves[j], sizeof(Move), 1, file);
        }
        
        // 요괴 인벤토리 저장
        fwrite(&gParty[i].yokaiInventoryCount, sizeof(int), 1, file);
        for (int j = 0; j < gParty[i].yokaiInventoryCount; j++) {
            fwrite(&gParty[i].yokaiInventory[j], sizeof(InventoryItem), 1, file);
        }
        
        // 돋보기 개수 저장
        fwrite(&gParty[i].magnifierCount, sizeof(int), 1, file);
    }
    
    // 플레이어 정보 저장
    fwrite(&gPlayer, sizeof(Player), 1, file);
    
    // 인벤토리 개수 저장
    fwrite(&inventoryCount, sizeof(int), 1, file);
    
    // 인벤토리 아이템 정보 저장
    for (int i = 0; i < inventoryCount; i++) {
        fwrite(&inventory[i], sizeof(InventoryItem), 1, file);
    }
    
    // 수동 저장이 아닌 경우에만 다음 스테이지의 상대 요괴 정보 저장
    if (!gGameState.isManualSave) {
        Yokai nextEnemy;
        if (gStage.isBossStage) {
            nextEnemy = gStage.enemies[0];  // 보스 스테이지의 경우 첫 번째 적 요괴 사용
        } else {
            nextEnemy = gStage.enemies[rand() % gStage.enemyCount];  // 랜덤 적 요괴 선택
        }
        
        fwrite(&nextEnemy.name, sizeof(char), YOKAI_NAME_MAX, file);
        fwrite(&nextEnemy.level, sizeof(int), 1, file);
        fwrite(&nextEnemy.exp, sizeof(int), 1, file);
        fwrite(&nextEnemy.attack, sizeof(int), 1, file);
        fwrite(&nextEnemy.defense, sizeof(int), 1, file);
        fwrite(&nextEnemy.stamina, sizeof(int), 1, file);
        fwrite(&nextEnemy.speed, sizeof(int), 1, file);
        fwrite(&nextEnemy.currentHP, sizeof(float), 1, file);
        fwrite(&nextEnemy.type, sizeof(YokaiType), 1, file);
        fwrite(&nextEnemy.status, sizeof(YokaiStatus), 1, file);
        fwrite(&nextEnemy.desc, sizeof(char), 128, file);
        fwrite(&nextEnemy.moveCount, sizeof(int), 1, file);
        
        // 상대 요괴의 기술 정보 저장
        for (int j = 0; j < nextEnemy.moveCount; j++) {
            fwrite(&nextEnemy.moves[j].move, sizeof(Move), 1, file);
            fwrite(&nextEnemy.moves[j].currentPP, sizeof(int), 1, file);
        }
    } else {
        // 수동 저장 시 현재 전투 중인 상대 요괴 정보 저장
    fwrite(&currentEnemy.name, sizeof(char), YOKAI_NAME_MAX, file);
    fwrite(&currentEnemy.level, sizeof(int), 1, file);
    fwrite(&currentEnemy.exp, sizeof(int), 1, file);
    fwrite(&currentEnemy.attack, sizeof(int), 1, file);
    fwrite(&currentEnemy.defense, sizeof(int), 1, file);
    fwrite(&currentEnemy.stamina, sizeof(int), 1, file);
    fwrite(&currentEnemy.speed, sizeof(int), 1, file);
    fwrite(&currentEnemy.currentHP, sizeof(float), 1, file);
    fwrite(&currentEnemy.type, sizeof(YokaiType), 1, file);
    fwrite(&currentEnemy.status, sizeof(YokaiStatus), 1, file);
    fwrite(&currentEnemy.desc, sizeof(char), 128, file);
    fwrite(&currentEnemy.moveCount, sizeof(int), 1, file);
    
    // 상대 요괴의 기술 정보 저장
    for (int j = 0; j < currentEnemy.moveCount; j++) {
        fwrite(&currentEnemy.moves[j].move, sizeof(Move), 1, file);
        fwrite(&currentEnemy.moves[j].currentPP, sizeof(int), 1, file);
        }
    }
    
    // 턴 카운트 저장
    fwrite(&turnCount, sizeof(int), 1, file);
    
    // 전투에 참여한 요괴 수 저장
    fwrite(&participatedCount, sizeof(int), 1, file);
    
    // 전투에 참여한 요괴 목록 저장
    fwrite(participatedIdx, sizeof(int), participatedCount, file);
    
    // 마지막으로 사용한 요괴 인덱스 저장
    fwrite(&lastYokaiIdx, sizeof(int), 1, file);
    
    fclose(file);
    printText("게임이 저장되었습니다.\n");
}

// 게임 데이터 불러오기 함수
int loadGameData() {
    LOG_FUNCTION_EXECUTION("loadGameData");
    FILE* file = fopen(SAVE_FILE, "rb");  // 바이너리 읽기 모드로 파일 열기
    if (!file) {
        printText("저장 파일이 없습니다.\n");
        return 0;
    }
    
    // 지역 데이터 먼저 불러오기
    loadRegionData(file);
    
    // 스테이지 정보 불러오기
    fread(&gStage, sizeof(StageInfo), 1, file);
    
    // 이어하기 시 지역명이 바뀌는 문제 해결: currentRegion을 gStage.region으로 덮어씁니다.
    strcpy(currentRegion, gStage.region);
    
    // 파이널 스테이지일 경우 지형 이름 복원
    if (gStage.stageNumber >= 81) {
        int terrainIndex = gStage.stageNumber - 81;
        if (terrainIndex >= 0 && terrainIndex < FINAL_TERRAIN_SEQUENCE_LENGTH) {
            strcpy(gStage.terrainName, finalTerrainNames[terrainSequence[terrainIndex]]);
        } else {
            strcpy(gStage.terrainName, finalTerrainNames[FINAL_TERRAIN_COUNT - 1]);
        }
    }
    
    // 동료 요괴 수 불러오기
    fread(&gPartyCount, sizeof(int), 1, file);
    
    // 동료 요괴 정보 불러오기
    for (int i = 0; i < gPartyCount; i++) {
        fread(&gParty[i].id, sizeof(unsigned long long), 1, file);
        fread(&gParty[i].name, sizeof(char), YOKAI_NAME_MAX, file);
        fread(&gParty[i].level, sizeof(int), 1, file);
        fread(&gParty[i].exp, sizeof(int), 1, file);
        fread(&gParty[i].attack, sizeof(int), 1, file);
        fread(&gParty[i].defense, sizeof(int), 1, file);
        fread(&gParty[i].stamina, sizeof(int), 1, file);
        fread(&gParty[i].speed, sizeof(int), 1, file);
        fread(&gParty[i].currentHP, sizeof(float), 1, file);
        fread(&gParty[i].type, sizeof(YokaiType), 1, file);
        fread(&gParty[i].status, sizeof(YokaiStatus), 1, file);
        fread(&gParty[i].desc, sizeof(char), YOKAI_DESC_MAX, file);
        fread(&gParty[i].moveCount, sizeof(int), 1, file);
        
        // 기술 정보 불러오기
        for (int j = 0; j < gParty[i].moveCount; j++) {
            fread(&gParty[i].moves[j].move, sizeof(Move), 1, file);
            fread(&gParty[i].moves[j].currentPP, sizeof(int), 1, file);
        }
        
        // 배울 수 있는 기술 목록 불러오기
        fread(&gParty[i].learnableMoveCount, sizeof(int), 1, file);
        for (int j = 0; j < gParty[i].learnableMoveCount; j++) {
            fread(&gParty[i].learnableMoves[j], sizeof(Move), 1, file);
        }
        
        // 잊은 기술 목록 불러오기
        fread(&gParty[i].forgottenMoveCount, sizeof(int), 1, file);
        for (int j = 0; j < gParty[i].forgottenMoveCount; j++) {
            fread(&gParty[i].forgottenMoves[j], sizeof(Move), 1, file);
        }
        
        // 요괴 인벤토리 불러오기
        fread(&gParty[i].yokaiInventoryCount, sizeof(int), 1, file);
        for (int j = 0; j < gParty[i].yokaiInventoryCount; j++) {
            fread(&gParty[i].yokaiInventory[j], sizeof(InventoryItem), 1, file);
        }
        
        // 돋보기 개수 불러오기
        fread(&gParty[i].magnifierCount, sizeof(int), 1, file);
        
        // 복숭아 개수 제한 확인 (5개 초과 시 5개로 제한)
        for (int j = 0; j < gParty[i].yokaiInventoryCount; j++) {
            if (strcmp(gParty[i].yokaiInventory[j].item.name, "복숭아") == 0) {
                if (gParty[i].yokaiInventory[j].count > 5) {
                    gParty[i].yokaiInventory[j].count = 5;
                }
                break;
            }
        }
    }
    
    // 플레이어 정보 불러오기
    fread(&gPlayer, sizeof(Player), 1, file);
    
    // 인벤토리 개수 불러오기
    fread(&inventoryCount, sizeof(int), 1, file);
    
    // 인벤토리 아이템 정보 불러오기
    for (int i = 0; i < inventoryCount; i++) {
        fread(&inventory[i], sizeof(InventoryItem), 1, file);
    }
    
    // 현재 전투 중인 상대 요괴 정보 불러오기
    fread(&currentEnemy.name, sizeof(char), YOKAI_NAME_MAX, file);
    fread(&currentEnemy.level, sizeof(int), 1, file);
    fread(&currentEnemy.exp, sizeof(int), 1, file);
    fread(&currentEnemy.attack, sizeof(int), 1, file);
    fread(&currentEnemy.defense, sizeof(int), 1, file);
    fread(&currentEnemy.stamina, sizeof(int), 1, file);
    fread(&currentEnemy.speed, sizeof(int), 1, file);
    fread(&currentEnemy.currentHP, sizeof(float), 1, file);
    fread(&currentEnemy.type, sizeof(YokaiType), 1, file);
    fread(&currentEnemy.status, sizeof(YokaiStatus), 1, file);
    fread(&currentEnemy.desc, sizeof(char), 128, file);
    fread(&currentEnemy.moveCount, sizeof(int), 1, file);
    
    // 상대 요괴의 기술 정보 불러오기
    for (int j = 0; j < currentEnemy.moveCount; j++) {
        fread(&currentEnemy.moves[j].move, sizeof(Move), 1, file);
        fread(&currentEnemy.moves[j].currentPP, sizeof(int), 1, file);
    }
    
    // 턴 카운트 불러오기
    fread(&turnCount, sizeof(int), 1, file);
    
    if (gameSettings.debugMode) {
        char buffer[256];
    sprintf(buffer, "[DEBUG] 로드 시 turnCount: %d\n", turnCount);
        printTextAndWait(buffer);
        sprintf(buffer, "[DEBUG] 로드 시 스테이지 번호: %d\n", gStage.stageNumber);
        printTextAndWait(buffer);
        sprintf(buffer, "[DEBUG] 로드 시 지역: %s\n", getCurrentRegion());
        printTextAndWait(buffer);
        sprintf(buffer, "[DEBUG] 로드 시 지형: %s\n", getCurrentTerrain());
        printTextAndWait(buffer);
        fastSleep(500);
    }
    
    // 전투에 참여한 요괴 수 불러오기
    fread(&participatedCount, sizeof(int), 1, file);
    
    // 전투에 참여한 요괴 목록 불러오기
    fread(participatedIdx, sizeof(int), participatedCount, file);
    
    // 마지막으로 사용한 요괴 인덱스 불러오기
    fread(&lastYokaiIdx, sizeof(int), 1, file);
    
    fclose(file);
    gGameState.isLoadedGame = 1; // 이어하기 플래그 설정
    return 1;
}

// 저장 파일 삭제 함수
void removeSaveFile() {
    LOG_FUNCTION_EXECUTION("removeSaveFile");
    if (remove(SAVE_FILE) == 0) {
        printText("저장 파일이 삭제되었습니다.\n");
    } else {
        printText("저장 파일을 삭제하지 못했습니다.\n");
    }
} 