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
#include <windows.h>
#define SAVE_FILE "data/save.dat"

// battle.c에서 선언된 변수들을 extern으로 선언
extern int turnCount;
extern int participatedCount;
extern int participatedIdx[];
extern int lastYokaiIdx;

// 게임 저장 함수
void saveGame() {
    FILE* file = fopen(SAVE_FILE, "wb");  // 바이너리 쓰기 모드로 파일 열기
    if (!file) return;
    
    if (gameSettings.debugMode) {
    char buffer[128];
    sprintf(buffer, "[DEBUG] 저장 시 turnCount: %d\n", turnCount);
    printTextAndWait(buffer);
    fastSleep(500);
    }
    
    // 스테이지 정보 저장
    fwrite(&gStage, sizeof(StageInfo), 1, file);
    
    // 동료 요괴 수 저장
    fwrite(&gPartyCount, sizeof(int), 1, file);
    
    // 동료 요괴 정보 저장
    for (int i = 0; i < gPartyCount; i++) {
        // 요괴 기본 정보 저장
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
        fwrite(&gParty[i].desc, sizeof(char), 128, file);
        fwrite(&gParty[i].moveCount, sizeof(int), 1, file);
        
        // 현재 배운 기술 정보 저장
        for (int j = 0; j < gParty[i].moveCount; j++) {
            fwrite(&gParty[i].moves[j].move, sizeof(Move), 1, file);
            fwrite(&gParty[i].moves[j].currentPP, sizeof(int), 1, file);
        }
        
        // 배운 기술 목록 저장
        fwrite(&gParty[i].learnedMoveCount, sizeof(int), 1, file);
        for (int j = 0; j < gParty[i].learnedMoveCount; j++) {
            fwrite(&gParty[i].learnedMoves[j], sizeof(Move), 1, file);
        }
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
    
    // 지역 데이터 저장
    saveRegionData(file);
    
    fclose(file);
}

// 게임 데이터 불러오기 함수
int loadGameData() {
    FILE* file = fopen(SAVE_FILE, "rb");  // 바이너리 읽기 모드로 파일 열기
    if (!file) return 0;
    
    // 스테이지 정보 불러오기
    fread(&gStage, sizeof(StageInfo), 1, file);
    
    // 동료 요괴 수 불러오기
    fread(&gPartyCount, sizeof(int), 1, file);
    
    // 동료 요괴 정보 불러오기
    for (int i = 0; i < gPartyCount; i++) {
        // 요괴 기본 정보 불러오기
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
        fread(&gParty[i].desc, sizeof(char), 128, file);
        fread(&gParty[i].moveCount, sizeof(int), 1, file);
        
        // 현재 배운 기술 정보 불러오기
        for (int j = 0; j < gParty[i].moveCount; j++) {
            fread(&gParty[i].moves[j].move, sizeof(Move), 1, file);
            fread(&gParty[i].moves[j].currentPP, sizeof(int), 1, file);
        }
        
        // 배운 기술 목록 불러오기
        fread(&gParty[i].learnedMoveCount, sizeof(int), 1, file);
        for (int j = 0; j < gParty[i].learnedMoveCount; j++) {
            fread(&gParty[i].learnedMoves[j], sizeof(Move), 1, file);
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
    char buffer[128];
    sprintf(buffer, "[DEBUG] 로드 시 turnCount: %d\n", turnCount);
    printTextAndWait(buffer);
    fastSleep(500);
    }
    
    // 전투에 참여한 요괴 수 불러오기
    fread(&participatedCount, sizeof(int), 1, file);
    
    // 전투에 참여한 요괴 목록 불러오기
    fread(participatedIdx, sizeof(int), participatedCount, file);
    
    // 마지막으로 사용한 요괴 인덱스 불러오기
    fread(&lastYokaiIdx, sizeof(int), 1, file);
    
    // 지역 데이터 불러오기
    loadRegionData(file);
    
    fclose(file);
    gGameState.isLoadedGame = 1; // 이어하기 플래그 설정
    return 1;
}

// 저장 파일 삭제 함수
void removeSaveFile() {
    remove(SAVE_FILE);
} 