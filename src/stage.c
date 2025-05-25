#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "stage.h"
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
    stage->stageNumber = stageNumber;
    stage->isBossStage = (stageNumber % 10 == 0);
    
    // 레벨 범위 계산
    calculateLevelRange(stageNumber, &stage->minLevel, &stage->maxLevel);
    
    // 보스 스테이지일 경우 적 수를 1로, 일반 스테이지는 3-5마리
    stage->enemyCount = stage->isBossStage ? 1 : (rand() % 3 + 3);
    
    // 적 요괴 생성
    generateStageEnemies(stage);
}

void generateStageEnemies(StageInfo* stage) {
    for (int i = 0; i < stage->enemyCount; i++) {
        int level = stage->minLevel + (rand() % (stage->maxLevel - stage->minLevel + 1));
        
        if (stage->isBossStage) {
            stage->enemies[i] = createRandomBossYokaiWithLevel(level);
        } else {
            stage->enemies[i] = createRandomYokaiWithLevel(level);
        }
    }
}

void printStageInfo(const StageInfo* stage) {
    printf("\n=== 스테이지 %d ===\n", stage->stageNumber);
    printf("스테이지 유형: %s\n", stage->isBossStage ? "보스 스테이지" : "일반 스테이지");
    printf("적 요괴 수: %d\n", stage->enemyCount);
    printf("레벨 범위: %d-%d\n", stage->minLevel, stage->maxLevel);
    
    printf("\n등장 요괴:\n");
    for (int i = 0; i < stage->enemyCount; i++) {
        printf("%d. ", i + 1);
        printYokaiInfo(&stage->enemies[i]);
    }
}

void nextStage() {
    currentStage.stageNumber++;
    currentStage.hour = (currentStage.hour + 1) % 24;
    
    // 10개 스테이지마다 지역 변경
    size_t regionIndex = (currentStage.stageNumber - 1) / 10;
    if (regionIndex < sizeof(regions) / sizeof(regions[0])) {
        strcpy(currentStage.region, regions[regionIndex]);
    }
    
    // 랜덤 지형 설정
    currentStage.terrain = rand() % TERRAIN_COUNT;
    
    // 5의 배수 스테이지 완료 시 자동 저장
    if ((currentStage.stageNumber - 1) % 5 == 0) {
        saveGame();
    }
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

void resetAllYokaiPP() {
    for (int i = 0; i < partyCount; i++) {
        for (int j = 0; j < party[i].moveCount; j++) {
            party[i].moves[j].currentPP = party[i].moves[j].move.pp;
        }
    }
}

void clearStage() {
    // ... 기존 코드 ...
    
    // 10의 배수 스테이지 클리어 시 기술 PP 초기화
    if (currentStage.stageNumber % 10 == 0) {
        resetAllYokaiPP();
        printTextAndWait("\n모든 동료 요괴의 기술 PP가 초기화되었습니다!");
    }
    
    // ... 기존 코드 ...
}

void showBattleInterface() {
    int minLevel, maxLevel;
    calculateLevelRange(currentStage.stageNumber, &minLevel, &maxLevel);
    int randomLevel = minLevel + rand() % (maxLevel - minLevel + 1);

    Yokai enemy;
    if (gameState.isLoadedGame) {
        enemy = currentEnemy;
        gameState.isLoadedGame = 0; // 한 번만 사용
    } else if (currentStage.stageNumber % 10 == 0) {
        enemy = createRandomBossYokaiWithLevel(randomLevel);
    } else {
        enemy = createRandomYokaiWithLevel(randomLevel);
    }

    char buffer[256];
    sprintf(buffer, "%s Lv.%d (이)가 싸움을 걸어왔다!\n", enemy.name, enemy.level);
    printText(buffer);

    int battleResult = startBattle(&enemy);

    // 보스 스테이지 처리 (10의 배수 스테이지)
    if (currentStage.stageNumber % 10 == 0) {
        if (battleResult == 101 || battleResult == 102) {  // 승리한 경우
            clearStage();
            nextStage();
        }
        // 패배하거나 도망친 경우는 다음 스테이지로 진행하지 않음
    } else {
        // 일반 스테이지는 항상 다음으로 진행
        nextStage();
    }
} 