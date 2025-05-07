#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_db.h"

#define SAVE_FILE "game_save.dat"

// 외부 변수 선언
extern SaveData currentGameData;

// 파일 시스템 초기화
int initializeDB() {
    // 저장 파일이 있는지 확인
    FILE* file = fopen(SAVE_FILE, "a");
    if (!file) {
        printf("저장 파일을 생성할 수 없습니다.\n");
        return 0;
    }
    fclose(file);
    printf("DB 초기화 성공!\n");
    return 1;
}

// 게임 저장
void saveGame(int stageNumber, int gameTime, const char* location, const char* terrain, int gameSpeed) {
    FILE* file = fopen("save_data.txt", "w");
    if (file == NULL) {
        printf("저장 파일을 열 수 없습니다.\n");
        return;
    }

    // 스테이지 번호가 0이면 1로 설정
    if (stageNumber < 1) {
        stageNumber = 1;
    }

    // 현재 게임 데이터 업데이트
    currentGameData.stageNumber = stageNumber;
    currentGameData.time = gameTime;
    strncpy(currentGameData.lastLocation, location, sizeof(currentGameData.lastLocation) - 1);
    strncpy(currentGameData.lastTerrain, terrain, sizeof(currentGameData.lastTerrain) - 1);
    currentGameData.gameSpeed = gameSpeed;

    // 기본 게임 정보 저장
    fprintf(file, "%d,%d,%s,%s,%d\n", 
        currentGameData.stageNumber,
        currentGameData.time,
        currentGameData.lastLocation,
        currentGameData.lastTerrain,
        currentGameData.gameSpeed);

    // 동료 요괴 정보 저장
    fprintf(file, "%d\n", currentGameData.companionCount);
    for (int i = 0; i < currentGameData.companionCount; i++) {
        Companion* comp = &currentGameData.companions[i];
        fprintf(file, "%s,%d,%d,%d,%d,%d,%d\n",
            comp->name, comp->level, comp->hp, comp->maxHp,
            comp->attack, comp->defense, comp->evasion);

        // 현재 보유 기술 저장
        fprintf(file, "%d\n", comp->currentSkillCount);
        for (int j = 0; j < comp->currentSkillCount; j++) {
            fprintf(file, "%s,%d,%d,%s\n",
                comp->currentSkills[j].name,
                comp->currentSkills[j].power,
                comp->currentSkills[j].accuracy,
                comp->currentSkills[j].description);
        }

        // 사용 가능한 기술 저장
        fprintf(file, "%d\n", comp->availableSkillCount);
        for (int j = 0; j < comp->availableSkillCount; j++) {
            fprintf(file, "%s,%d,%d,%s\n",
                comp->availableSkills[j].name,
                comp->availableSkills[j].power,
                comp->availableSkills[j].accuracy,
                comp->availableSkills[j].description);
        }
    }

    fclose(file);
    printf("\n[성공] 게임이 저장되었습니다! (스테이지: %d)\n", stageNumber);
}

// 게임 불러오기
SaveData* loadGame() {
    FILE* file = fopen("save_data.txt", "r");
    if (file == NULL) {
        return NULL;
    }

    SaveData* saveData = malloc(sizeof(SaveData));
    memset(saveData, 0, sizeof(SaveData));

    // 기본 게임 정보 읽기
    fscanf(file, "%d,%d,%[^,],%[^,],%d\n", 
        &saveData->stageNumber,
        &saveData->time,
        saveData->lastLocation,
        saveData->lastTerrain,
        &saveData->gameSpeed);

    // 스테이지 번호가 0이면 1로 설정
    if (saveData->stageNumber < 1) {
        saveData->stageNumber = 1;
    }

    // 동료 요괴 정보 읽기
    fscanf(file, "%d\n", &saveData->companionCount);
    for (int i = 0; i < saveData->companionCount; i++) {
        Companion* comp = &saveData->companions[i];
        fscanf(file, "%[^,],%d,%d,%d,%d,%d,%d\n",
            comp->name, &comp->level, &comp->hp, &comp->maxHp,
            &comp->attack, &comp->defense, &comp->evasion);

        // 현재 보유 기술 읽기
        fscanf(file, "%d\n", &comp->currentSkillCount);
        for (int j = 0; j < comp->currentSkillCount; j++) {
            fscanf(file, "%[^,],%d,%d,%[^\n]\n",
                comp->currentSkills[j].name,
                &comp->currentSkills[j].power,
                &comp->currentSkills[j].accuracy,
                comp->currentSkills[j].description);
        }

        // 사용 가능한 기술 읽기
        fscanf(file, "%d\n", &comp->availableSkillCount);
        for (int j = 0; j < comp->availableSkillCount; j++) {
            fscanf(file, "%[^,],%d,%d,%[^\n]\n",
                comp->availableSkills[j].name,
                &comp->availableSkills[j].power,
                &comp->availableSkills[j].accuracy,
                comp->availableSkills[j].description);
        }
    }

    fclose(file);
    return saveData;
}

// 파일 시스템 정리
void closeDB() {
    // 파일 기반 시스템에서는 특별한 정리가 필요 없음
} 