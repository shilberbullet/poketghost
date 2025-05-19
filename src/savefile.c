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
        fwrite(&party[i], sizeof(Yokai), 1, file);
        // 각 요괴의 기술 currentPP 저장
        for (int j = 0; j < party[i].moveCount; j++) {
            fwrite(&party[i].moves[j].currentPP, sizeof(int), 1, file);
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
        fread(&party[i], sizeof(Yokai), 1, file);
        // 각 요괴의 기술 currentPP 불러오기
        for (int j = 0; j < party[i].moveCount; j++) {
            fread(&party[i].moves[j].currentPP, sizeof(int), 1, file);
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
    fclose(file);
    gameState.isLoadedGame = 1; // 이어하기 플래그 설정
    return 1;
} 