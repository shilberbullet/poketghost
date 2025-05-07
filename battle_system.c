#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "game_db.h"
#include "boss_monsters.h"

// 외부 함수 선언
extern void printCharByChar(const char* str);
extern GameSpeed gameSpeeds[];
extern int currentSpeedIndex;
extern SaveData* loadGame();  // 저장된 게임 데이터 불러오기 함수
extern void initializeTeamCode();
extern int isTeamCodeCompleted();

// 전투 시스템 초기화
void initializeCompanionBattle() {
    // 필요한 초기화 작업
}

// 전투 시스템 함수
void battleSystem(SaveData* saveData) {
    system("cls");
    printCharByChar("\n=== 전투 시스템 ===");
    
    // 보스 스테이지 확인
    if (isBossStage(saveData->stageNumber)) {
        BossMonster* boss = getRandomBossMonster();
        printCharByChar("\n보스 요괴가 나타났다!");
        printCharByChar("\n=== 보스 요괴 정보 ===");
        char buffer[256];
        sprintf(buffer, "\n이름: %s", boss->name);
        printCharByChar(buffer);
        sprintf(buffer, "\n타입: %s", boss->type);
        printCharByChar(buffer);
        sprintf(buffer, "\nHP: %d", boss->hp);
        printCharByChar(buffer);
        sprintf(buffer, "\n공격력: %d", boss->attack);
        printCharByChar(buffer);
        sprintf(buffer, "\n방어력: %d", boss->defense);
        printCharByChar(buffer);
        sprintf(buffer, "\n특수 기술: %s", boss->specialMove);
        printCharByChar(buffer);
        sprintf(buffer, "\n%s", boss->description);
        printCharByChar(buffer);
    } else {
        printCharByChar("\n일반 요괴가 나타났다!");
    }
    
    // 동료 요괴 목록 표시
    if (!saveData || saveData->companionCount == 0) {
        printCharByChar("\n아직 동료 요괴가 없습니다!");
        Sleep(2000);
        return;
    }
    
    printCharByChar("\n\n=== 동료 요괴 목록 ===");
    for (int i = 0; i < saveData->companionCount; i++) {
        char buffer[200];
        Companion* comp = &saveData->companions[i];
        sprintf(buffer, "\n%d. %s (Lv.%d)", i + 1, comp->name, comp->level);
        printCharByChar(buffer);
        sprintf(buffer, "\n   HP: %d/%d  공격력: %d  방어력: %d  회피율: %d%%", 
                comp->hp, comp->maxHp, comp->attack, comp->defense, comp->evasion);
        printCharByChar(buffer);
    }
    
    printCharByChar("\n\n사용할 동료 요괴를 선택하세요: ");
    
    char input[10];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    int choice = atoi(input);
    if (choice < 1 || choice > saveData->companionCount) {
        printCharByChar("\n잘못된 선택입니다!");
        Sleep(2000);
        return;
    }
    
    // 선택한 동료 요괴의 기술 목록 표시
    Companion* selectedCompanion = &saveData->companions[choice - 1];
    printCharByChar("\n=== 사용 가능한 기술 ===");
    for (int i = 0; i < selectedCompanion->currentSkillCount; i++) {
        char buffer[200];
        CompanionSkill* skill = &selectedCompanion->currentSkills[i];
        sprintf(buffer, "\n%d. %s", i + 1, skill->name);
        printCharByChar(buffer);
        sprintf(buffer, "\n   위력: %d  명중률: %d%%", skill->power, skill->accuracy);
        printCharByChar(buffer);
        sprintf(buffer, "\n   설명: %s", skill->description);
        printCharByChar(buffer);
    }
    
    printCharByChar("\n\n사용할 기술을 선택하세요: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    int skillChoice = atoi(input);
    if (skillChoice < 1 || skillChoice > selectedCompanion->currentSkillCount) {
        printCharByChar("\n잘못된 기술을 선택했습니다!");
        Sleep(2000);
        return;
    }
    
    printCharByChar("\n전투를 시작합니다!");
    Sleep(1000);
    
    // 전투 결과 처리
    printCharByChar("\n전투에서 승리했습니다!");
    Sleep(2000);
    
    // 전투 완료 후 저장
    saveGame(saveData->stageNumber, saveData->time, 
            saveData->lastLocation, saveData->lastTerrain, 
            saveData->gameSpeed);
}

// 동료 요괴 전투 시작
void startCompanionBattle() {
    SaveData* saveData = loadGame();
    if (!saveData) {
        printCharByChar("\n게임 데이터를 불러올 수 없습니다!");
        Sleep(2000);
        return;
    }
    
    battleSystem(saveData);
    free(saveData);
}

// 동료 요괴 전투 종료
void endCompanionBattle() {
    // TODO: 전투 종료 처리
} 