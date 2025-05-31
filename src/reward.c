#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reward.h"
#include "text.h"
#include "input.h"
#include "item.h"
#include "game.h"

// 전투 보상 전 계산 함수
int calculateBattleReward() {
    int baseReward = 100;  // 기본 보상
    int stageBonus = (currentStage.stageNumber / 10) * 50;  // 10스테이지마다 50전씩 증가
    int randomBonus = rand() % 50;  // 0-49전 랜덤 보너스
    
    // 10의 배수 스테이지에서는 2배 보상
    if (currentStage.stageNumber % 10 == 0) {
        return (baseReward + stageBonus + randomBonus) * 2;
    }
    
    return baseReward + stageBonus + randomBonus;
}

// 초기화 비용 계산 함수
static int calculateResetCost(int stageNumber) {
    int baseCost = 100;  // 기본 비용
    
    // 1-10 스테이지: 100전
    // 11-20 스테이지: 200전
    if (stageNumber <= 10) {
        return baseCost;
    } else {
        return baseCost * 2;
    }
}

// getRandomItems는 item.c의 것을 사용

// 아이템 보상 시스템
void itemRewardSystem() {
    static Item candidates[3];  // static으로 선언하여 함수 호출 간에도 값이 유지되도록 함
    static int isInitialized = 0;  // 아이템이 초기화되었는지 확인하는 플래그
    static int resetCount = 0;  // 현재 스테이지에서 초기화한 횟수
    
    // 현재 스테이지 번호에 따른 초기화 비용 계산
    int resetCost = calculateResetCost(currentStage.stageNumber);
    
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
                for (int j = i + 1; j < 3; j++) {
                    if (strcmp(candidates[i].name, candidates[j].name) == 0) {
                        valid = 0;
                        break;
                    }
                }
                if (!valid) break;
            }
        }
        isInitialized = 1;
    }
    
    printText("\n전투 보상! 아이템을 하나 선택하세요:\n");
    for (int i = 0; i < 3; i++) {
        char buffer[256];
        sprintf(buffer, "%d. %s [%s] - %s\n", i+1, candidates[i].name,
            candidates[i].grade == ITEM_COMMON ? "일반" : candidates[i].grade == ITEM_RARE ? "희귀" : "초희귀",
            candidates[i].desc);
        printText(buffer);
    }
    char resetBuffer[128];
    sprintf(resetBuffer, "4. 아이템 목록 초기화 (%d전)\n", resetCost);
    printText(resetBuffer);
    printText("선택 (번호): ");
    int idx = getIntInput() - 1;
    
    if (idx == 3) {  // 아이템 목록 초기화 선택
        if (player.money >= resetCost) {
            player.money -= resetCost;
            char buffer[128];
            sprintf(buffer, "\n%d전을 사용하여 아이템 목록을 초기화했습니다! (남은 전: %d)\n", resetCost, player.money);
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
        printTextAndWait("\n잘못된 선택입니다. 다시 시도하세요.");
        itemRewardSystem();
        return;
    }
    
    addItemToInventory(&candidates[idx]);
    
    // 회복형 아이템이 아닌 경우에만 획득 메시지 출력
    if (candidates[idx].type != ITEM_HEAL) {
        char buffer[128];
        sprintf(buffer, "\n%s를 인벤토리에 획득했습니다!", candidates[idx].name);
        printTextAndWait(buffer);
    }
    
    // 보상 선택이 완료되면 초기화 플래그를 리셋
    isInitialized = 0;
    resetCount = 0;  // 초기화 횟수도 리셋
} 