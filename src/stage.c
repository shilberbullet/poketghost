// 표준 입출력, 메모리, 시간, 문자열, 게임 관련 헤더
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
#include "game.h"

// 지역 이름 배열 (10개씩 그룹화)
const char* regions[] = {
    // 1-10 스테이지 (초보자 지역)
    "초보자의 숲", "초보자의 언덕", "초보자의 계곡", "초보자의 호수", "초보자의 들판",
    "초보자의 마을", "초보자의 길", "초보자의 동굴", "초보자의 강", "초보자의 산",
    
    // 11-20 스테이지 (숙련자 지역)
    "숙련자의 숲", "숙련자의 언덕", "숙련자의 계곡", "숙련자의 호수", "숙련자의 들판",
    "숙련자의 마을", "숙련자의 길", "숙련자의 동굴", "숙련자의 강", "숙련자의 산",
    
    // 21-30 스테이지 (전문가 지역)
    "전문가의 숲", "전문가의 언덕", "전문가의 계곡", "전문가의 호수", "전문가의 들판",
    "전문가의 마을", "전문가의 길", "전문가의 동굴", "전문가의 강", "전문가의 산"
};

// 지형 이름 배열
const char* terrainNames[] = {
    "산", "강", "바다", "논", "마을"
};

// 전역 스테이지 정보
StageInfo currentStage = {0};

// 스테이지 초기화 함수
void initStage(StageInfo* stage, int stageNumber) {
    if (stageNumber % 10 == 0) {
        initBossStage(stage, stageNumber);  // 10의 배수 스테이지는 보스 스테이지
    } else {
        initNormalStage(stage, stageNumber);  // 그 외는 일반 스테이지
    }
    
    // 새 게임일 경우 시간을 0으로 초기화
    if (gameState.isNewGame) {
        stage->hour = 0;
        gameState.isNewGame = 0;  // 플래그 초기화
    }
    
    // 랜덤 지형 설정
    stage->terrain = rand() % TERRAIN_COUNT;
}

// 다음 스테이지로 진행하는 함수
void nextStage() {
    currentStage.stageNumber++;  // 스테이지 번호 증가
    turnCount = 0;              // 턴 카운트 초기화
    currentStage.hour = (currentStage.hour + 1) % 24;  // 시간 증가 (24시간 주기)
    currentStage.terrain = rand() % TERRAIN_COUNT;     // 랜덤 지형 설정
    
    // 새로운 스테이지 초기화 (hour, terrain은 유지)
    initStage(&currentStage, currentStage.stageNumber);
    
    // 5의 배수 스테이지 완료 시 자동 저장 (수동 저장이 아닌 경우에만)
    if ((currentStage.stageNumber - 1) % 5 == 0 && !gameState.isLoadedGame && !gameState.isManualSave) {
        saveGame();
        char buffer[128];
        sprintf(buffer, "\n%d스테이지 완료! 게임이 자동 저장되었습니다.\n", currentStage.stageNumber - 1);
        printTextAndWait(buffer);
    }
}

// 스테이지 정보를 표시하는 함수
void showStageInfo() {
    char buffer[256];
    
    system("cls");  // 화면 지우기
    printText("=== 스테이지 정보 ===\n\n");
    
    sprintf(buffer, "스테이지: %d\n", currentStage.stageNumber);  // 스테이지 번호
    printText(buffer);
    
    sprintf(buffer, "지역: %s\n", currentStage.region);  // 지역 이름
    printText(buffer);
    
    sprintf(buffer, "지형: %s\n", terrainNames[currentStage.terrain]);  // 지형 이름
    printText(buffer);
    
    int hour = (currentStage.stageNumber - 1) % 24;  // 현재 시간
    sprintf(buffer, "시간: %02d시\n", hour);
    printText(buffer);
    
    sprintf(buffer, "보유 전: %d전\n\n", player.money);  // 보유 금액
    printText(buffer);
}

// 전투 인터페이스를 표시하고 전투를 시작하는 함수
void showBattleInterface() {
    int minLevel, maxLevel;
    calculateLevelRange(currentStage.stageNumber, &minLevel, &maxLevel);  // 레벨 범위 계산
    int randomLevel = minLevel + rand() % (maxLevel - minLevel + 1);      // 랜덤 레벨 선택

    Yokai enemy;
    if (gameState.isLoadedGame) {
        enemy = currentEnemy;  // 이어하기 시 저장된 적 요괴 사용
        gameState.isLoadedGame = 0;  // 플래그 초기화
    } else if (currentStage.isBossStage) {
        enemy = currentStage.enemies[0];  // 보스 스테이지의 경우 첫 번째 적 요괴 사용
    } else {
        enemy = currentStage.enemies[rand() % currentStage.enemyCount];  // 랜덤 적 요괴 선택
    }

    int battleResult = startBattle(&enemy);  // 전투 시작

    if (currentStage.isBossStage) {
        if (battleResult == 101 || battleResult == 102) {  // 승리한 경우
            handleBossStageClear();  // 보스 스테이지 클리어 처리
            nextStage();             // 다음 스테이지로 진행
        }
    } else {
        handleNormalStageClear();  // 일반 스테이지 클리어 처리
        nextStage();               // 다음 스테이지로 진행
    }
} 