#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "stage_types.h"
#include "normal_stage.h"
#include "boss_stage.h"
#include "text.h"
#include "input.h"
#include "battle.h"
#include "savefile.h"
#include "party.h"
#include "yokai.h"

// 지역 이름 배열 (10개씩 그룹화)
const char* regions[] = {
    // 1-10 스테이지
    "초보자의 숲", "초보자의 언덕", "초보자의 계곡", "초보자의 호수", "초보자의 들판",
    "초보자의 마을", "초보자의 길", "초보자의 동굴", "초보자의 강", "초보자의 산",
    
    // 11-20 스테이지
    "숙련자의 숲", "숙련자의 언덕", "숙련자의 계곡", "숙련자의 호수", "숙련자의 들판",
    "숙련자의 마을", "숙련자의 길", "숙련자의 동굴", "숙련자의 강", "숙련자의 산",
    
    // 21-30 스테이지
    "전문가의 숲", "전문가의 언덕", "전문가의 계곡", "전문가의 호수", "전문가의 들판",
    "전문가의 마을", "전문가의 길", "전문가의 동굴", "전문가의 강", "전문가의 산"
};

// 지형 이름 배열
const char* terrainNames[] = {
    "산", "강", "바다", "논", "마을"
};

// 전역 스테이지 정보
StageInfo currentStage = {0};

void initStage(StageInfo* stage, int stageNumber) {
    if (stageNumber % 10 == 0) {
        initBossStage(stage, stageNumber);
    } else {
        initNormalStage(stage, stageNumber);
    }
    
    // 랜덤 지형 설정
    stage->terrain = rand() % TERRAIN_COUNT;
}

void nextStage() {
    currentStage.stageNumber++;
    currentStage.hour = (currentStage.hour + 1) % 24;
    currentStage.terrain = rand() % TERRAIN_COUNT;
    
    // 5의 배수 스테이지 완료 시 자동 저장
    if ((currentStage.stageNumber - 1) % 5 == 0) {
        saveGame();
        char buffer[128];
        sprintf(buffer, "\n%d스테이지 완료! 게임이 자동 저장되었습니다.\n", currentStage.stageNumber - 1);
        printTextAndWait(buffer);
    }
    
    // 새로운 스테이지 초기화 (hour, terrain은 유지)
    initStage(&currentStage, currentStage.stageNumber);
}

void showStageInfo() {
    char buffer[256];
    
    system("cls");
    printText("=== 스테이지 정보 ===\n\n");
    
    sprintf(buffer, "스테이지: %d\n", currentStage.stageNumber);
    printText(buffer);
    
    sprintf(buffer, "지역: %s\n", currentStage.region);
    printText(buffer);
    
    sprintf(buffer, "지형: %s\n", terrainNames[currentStage.terrain]);
    printText(buffer);
    
    sprintf(buffer, "시간: %02d시\n", currentStage.hour);
    printText(buffer);
    
    sprintf(buffer, "보유 전: %d전\n\n", player.money);
    printText(buffer);
}

void showBattleInterface() {
    int minLevel, maxLevel;
    calculateLevelRange(currentStage.stageNumber, &minLevel, &maxLevel);
    int randomLevel = minLevel + rand() % (maxLevel - minLevel + 1);

    Yokai enemy;
    if (gameState.isLoadedGame) {
        enemy = currentEnemy;
        gameState.isLoadedGame = 0; // 한 번만 사용
    } else if (currentStage.isBossStage) {
        enemy = currentStage.enemies[0];
    } else {
        enemy = currentStage.enemies[rand() % currentStage.enemyCount];
    }

    char buffer[256];
    sprintf(buffer, "%s Lv.%d (이)가 싸움을 걸어왔다!\n", enemy.name, enemy.level);
    printText(buffer);

    int battleResult = startBattle(&enemy);

    if (currentStage.isBossStage) {
        if (battleResult == 101 || battleResult == 102) {  // 승리한 경우
            handleBossStageClear();
            nextStage();
        }
    } else {
        handleNormalStageClear();
        nextStage();
    }
} 