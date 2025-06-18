#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reward.h"
#include "text.h"
#include "input.h"
#include "item.h"
#include "game.h"
#include "party.h"
#include "battle.h"
#include "../core/state.h"
#include <windows.h>
#include <math.h>

// 필요한 전역 변수는 state 모듈에서 접근

// static 변수들을 전역 변수로 이동
Item candidates[3];  // 보상 후보 아이템 배열
int isInitialized = 0;  // 아이템 후보 초기화 여부
int resetCount = 0;     // 아이템 목록 초기화 횟수
int lastStageNumber = -1; // 마지막 스테이지 번호

// 전투 보상 전 계산 함수
int calculateBattleReward() {
    // 기본 보상: 100전
    int baseReward = 100;
    
    // 스테이지 보너스: 스테이지가 올라갈수록 보상이 점진적으로 증가
    // 예: 1스테이지 = 100, 2스테이지 = 110, 3스테이지 = 121, 4스테이지 = 133, ...
    int stageBonus = (int)(baseReward * pow(1.1, gStage.stageNumber - 1));
    
    // 랜덤 보너스: 0-49전
    int randomBonus = rand() % 50;
    
    // 레벨 보너스: 적 요괴 레벨당 15전씩 보너스
    int levelBonus = currentEnemy.level * 15;
    
    // 10의 배수 스테이지에서는 추가 보너스
    if (gStage.stageNumber % 10 == 0) {
        // 10의 배수 스테이지에서는 기본 보상의 50% 추가
        stageBonus = (int)(stageBonus * 1.5);
    }
    
    // 최종 보상 계산
    return stageBonus + randomBonus + levelBonus;
}

// 초기화 비용 계산 함수
static int calculateResetCost(int stageNumber) {
    int baseCost = 100;  // 기본 비용
    int multiplier = 1;  // 기본 배수
    
    // 10스테이지마다 2배씩 증가
    if (stageNumber > 10) {
        multiplier = 1 << ((stageNumber - 1) / 10);  // 2의 (스테이지/10)제곱
    }
    
    return baseCost * multiplier;
}

// getRandomItems는 item.c의 것을 사용

// 아이템 등급을 한글로 반환하는 함수
const char* getGradeName(ItemGrade grade) {
    switch (grade) {
        case ITEM_COMMON:
            return "일반";
        case ITEM_RARE:
            return "희귀";
        case ITEM_SUPERRARE:
            return "초희귀";
        default:
            return "알 수 없음";
    }
}

// 파이널 스테이지 여부를 반환하는 함수
int isFinalStage() {
    return strcmp(gStage.region, "이계의 심연") == 0;
}

// 아이템 보상 시스템
void itemRewardSystem() {
    // 스테이지가 바뀌면 resetCount와 isInitialized를 초기화
    if (lastStageNumber != gStage.stageNumber) {
        resetCount = 0;
        isInitialized = 0;
        lastStageNumber = gStage.stageNumber;
    }

    // 현재 스테이지 번호에 따른 초기화 비용 계산
    int resetCost = calculateResetCost(gStage.stageNumber);
    
    // 초기화 횟수에 따른 추가 비용 계산
    for (int i = 0; i < resetCount; i++) {
        resetCost *= 2;
    }
    
    // 처음 호출될 때만 랜덤 아이템 생성
    if (!isInitialized) {
        // 중복 제거를 위해 반복
        int valid = 0;
        while (!valid) {
            getRandomItems(candidates, 3);
            valid = 1;
            for (int i = 0; i < 3; i++) {
                // 보스 스테이지이면서 파이널 스테이지가 아닐 때만 회복형 아이템 제외
                if (gStage.isBossStage && !isFinalStage() && candidates[i].type == ITEM_HEAL) {
                    valid = 0;
                    break;
                }
                for (int j = i + 1; j < 3; j++) {
                    if (strcmp(candidates[i].name, candidates[j].name) == 0) {
                        valid = 0;
                        break;
                    }
                }
                // 지도 아이템 중복 방지: 이미 보유 중이면 후보에서 제외
                if (strcmp(candidates[i].name, "지도") == 0) {
                    for (int k = 0; k < inventoryCount; k++) {
                        if (strcmp(inventory[k].item.name, "지도") == 0) {
                            valid = 0;
                            break;
                        }
                    }
                }
                if (!valid) break;
            }
        }
        isInitialized = 1;
    }
    
    printText("\n=== 아이템 보상 ===\n");
    for (int i = 0; i < 3; i++) {
        char buffer[256];
        const char* colorCode;
        switch (candidates[i].grade) {
            case ITEM_COMMON:
                colorCode = "\033[0m";  // 기본색 (흰색)
                break;
            case ITEM_RARE:
                colorCode = "\033[33m";  // 노란색
                break;
            case ITEM_SUPERRARE:
                colorCode = "\033[31m";  // 빨간색
                break;
            default:
                colorCode = "\033[0m";
        }
        sprintf(buffer, "%d. %s%s [%s]\033[0m - %s\n", i+1, 
            colorCode,
            candidates[i].name,
            getGradeName(candidates[i].grade),
            candidates[i].desc);
        printText(buffer);
    }
    char resetBuffer[128];
    sprintf(resetBuffer, "\033[94m4. 아이템 목록 초기화 (%d전)\033[0m\n", resetCost);  // 하드코딩된 200전 대신 resetCost 사용
    printText(resetBuffer);
    printText("\033[95m5. 아이템을 받지 않고 넘어간다\033[0m\n");  // 밝은 보라
    printText("선택 (번호): ");
    int idx = getIntInput() - 1;
    
    if (idx == 4) {  // 아이템을 받지 않고 넘어가기 선택
        printTextAndWait("\n아이템을 받지 않고 넘어갑니다.");
        isInitialized = 0;
        resetCount = 0;
        return;
    }
    
    if (idx == 3) {  // 아이템 목록 초기화 선택
        if (gPlayer.money >= resetCost) {
            gPlayer.money -= resetCost;
            char buffer[128];
            sprintf(buffer, "\n%d전을 사용하여 아이템 목록을 초기화했습니다! (남은 전: %d)\n", resetCost, gPlayer.money);
            printText(buffer);
            
            // 초기화 횟수 증가
            resetCount++;
            
            // 아이템 목록 초기화
            isInitialized = 0;
            itemRewardSystem();
            return;
        } else {
            printTextAndWait("\n전이 부족합니다!");
            itemRewardSystem();
            return;
        }
    }
    
    if (idx < 0 || idx >= 3) {
        printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
        itemRewardSystem();
        return;
    }
    
    addItemToInventory(&candidates[idx]);
    
    // 작두나 무당방울의 경우 개수에 따라 메시지 출력 여부 결정
    if (strcmp(candidates[idx].name, "작두") == 0 || strcmp(candidates[idx].name, "무당방울") == 0) {
        int count = 0;
        for (int i = 0; i < inventoryCount; i++) {
            if (strcmp(inventory[i].item.name, candidates[idx].name) == 0) {
                count = inventory[i].count;
                break;
            }
        }
        // 5개 미만일 때만 메시지 출력
        if (count < 5) {
            char buffer[128];
            sprintf(buffer, "\n%s를 인벤토리에 획득했습니다!", candidates[idx].name);
            printTextAndWait(buffer);
            fastSleep(500);
        }
    }
    // 회복형, 양갱형 아이템이 아닌 경우에만 획득 메시지 출력
    else if (candidates[idx].type != ITEM_HEAL && candidates[idx].type != ITEM_YANGGAENG) {
        char buffer[128];
        sprintf(buffer, "\n%s를 인벤토리에 획득했습니다!", candidates[idx].name);
        printTextAndWait(buffer);
        fastSleep(500);
    }
    
    // 보상 선택이 완료되면 초기화 플래그를 리셋
    isInitialized = 0;
    resetCount = 0;  // 초기화 횟수도 리셋
}

// 새 게임 시작 시 호출할 초기화 함수
void resetItemRewardSystem(void) {
    resetCount = 0;
    isInitialized = 0;
    lastStageNumber = -1;
} 