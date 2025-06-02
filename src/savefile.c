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

#define SAVE_FILE "data/save.dat"

void saveGame() {
    FILE* file = fopen(SAVE_FILE, "wb");
    if (!file) return;
    
    // 스테이지 정보 저장
    fwrite(&currentStage, sizeof(StageInfo), 1, file);
    
    // 동료 요괴 수 저장
    fwrite(&partyCount, sizeof(int), 1, file);
    
    // 동료 요괴 정보 저장
    for (int i = 0; i < partyCount; i++) {
        // 요괴 기본 정보 저장
        fwrite(&party[i].name, sizeof(char), YOKAI_NAME_MAX, file);
        fwrite(&party[i].level, sizeof(int), 1, file);
        fwrite(&party[i].exp, sizeof(int), 1, file);
        fwrite(&party[i].attack, sizeof(int), 1, file);
        fwrite(&party[i].defense, sizeof(int), 1, file);
        fwrite(&party[i].stamina, sizeof(int), 1, file);
        fwrite(&party[i].speed, sizeof(int), 1, file);
        fwrite(&party[i].currentHP, sizeof(float), 1, file);
        fwrite(&party[i].type, sizeof(YokaiType), 1, file);
        fwrite(&party[i].status, sizeof(YokaiStatus), 1, file);
        fwrite(&party[i].desc, sizeof(char), 128, file);
        fwrite(&party[i].moveCount, sizeof(int), 1, file);
        
        // 요괴의 기술 정보 저장
        for (int j = 0; j < party[i].moveCount; j++) {
            fwrite(&party[i].moves[j].move, sizeof(Move), 1, file);
            fwrite(&party[i].moves[j].currentPP, sizeof(int), 1, file);
        }
        
        // 배울 수 있는 기술 정보 저장
        fwrite(&party[i].learnableMoveCount, sizeof(int), 1, file);
        for (int j = 0; j < party[i].learnableMoveCount; j++) {
            fwrite(&party[i].learnableMoves[j], sizeof(Move), 1, file);
        }
    }
    
    // 플레이어 정보 저장
    fwrite(&player, sizeof(Player), 1, file);
    
    // 인벤토리 개수 저장
    fwrite(&inventoryCount, sizeof(int), 1, file);
    
    // 인벤토리 아이템 정보 저장
    for (int i = 0; i < inventoryCount; i++) {
        fwrite(&inventory[i], sizeof(InventoryItem), 1, file);
    }
    
    // 현재 전투 중인 상대 요괴 정보 저장
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
    
    fclose(file);
}

int loadGameData() {
    FILE* file = fopen(SAVE_FILE, "rb");
    if (!file) return 0;
    
    // 스테이지 정보 불러오기
    fread(&currentStage, sizeof(StageInfo), 1, file);
    
    // 동료 요괴 수 불러오기
    fread(&partyCount, sizeof(int), 1, file);
    
    // 동료 요괴 정보 불러오기
    for (int i = 0; i < partyCount; i++) {
        // 요괴 기본 정보 불러오기
        fread(&party[i].name, sizeof(char), YOKAI_NAME_MAX, file);
        fread(&party[i].level, sizeof(int), 1, file);
        fread(&party[i].exp, sizeof(int), 1, file);
        fread(&party[i].attack, sizeof(int), 1, file);
        fread(&party[i].defense, sizeof(int), 1, file);
        fread(&party[i].stamina, sizeof(int), 1, file);
        fread(&party[i].speed, sizeof(int), 1, file);
        fread(&party[i].currentHP, sizeof(float), 1, file);
        fread(&party[i].type, sizeof(YokaiType), 1, file);
        fread(&party[i].status, sizeof(YokaiStatus), 1, file);
        fread(&party[i].desc, sizeof(char), 128, file);
        fread(&party[i].moveCount, sizeof(int), 1, file);
        
        // 요괴의 기술 정보 불러오기
        for (int j = 0; j < party[i].moveCount; j++) {
            fread(&party[i].moves[j].move, sizeof(Move), 1, file);
            fread(&party[i].moves[j].currentPP, sizeof(int), 1, file);
        }
        
        // 배울 수 있는 기술 정보 불러오기
        fread(&party[i].learnableMoveCount, sizeof(int), 1, file);
        for (int j = 0; j < party[i].learnableMoveCount; j++) {
            fread(&party[i].learnableMoves[j], sizeof(Move), 1, file);
        }
    }
    
    // 플레이어 정보 불러오기
    fread(&player, sizeof(Player), 1, file);
    
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
    
    fclose(file);
    gameState.isLoadedGame = 1; // 이어하기 플래그 설정
    return 1;
}

void removeSaveFile() {
    remove(SAVE_FILE);
} 