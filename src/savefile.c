#include <stdio.h>
#include <string.h>
#include "savefile.h"
#include "stage.h"
#include "party.h"
#include "yokai.h"

#define SAVE_FILE "save.dat"

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
    }
    fclose(file);
    return 1;
} 