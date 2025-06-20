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
#include "logger.h"
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
    LOG_FUNCTION_EXECUTION("calculateBattleReward");
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
    LOG_FUNCTION_EXECUTION("calculateResetCost");
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
    LOG_FUNCTION_EXECUTION("getGradeName");
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
    LOG_FUNCTION_EXECUTION("isFinalStage");
    return strcmp(gStage.region, "백두산 정상") == 0;
}

// 찹살경단 전용 요괴 선택 함수
static void selectYokaiForChapsalgyungdan(int idx) {
    while (1) {
        printText("\n찹살경단을 지니게 할 요괴를 선택하세요:\n");
        for (int i = 0; i < gPartyCount; i++) {
            char buffer[256];
            sprintf(buffer, "%d. %s Lv.%d (HP: %.0f/%.0f)\033[0m\n",
                i + 1,
                gParty[i].name,
                gParty[i].level,
                gParty[i].currentHP,
                calculateHP(&gParty[i]));
            printText(buffer);
        }
        printText("0. 뒤로 가기\n");
        printText("숫자를 입력해주세요: ");
        int yokaiChoice = getIntInput();
        if (yokaiChoice == 0) {
            itemRewardSystem();
            return;
        }
        if (yokaiChoice < 1 || yokaiChoice > gPartyCount) {
            printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
            continue;
        }
        Yokai* targetYokai = &gParty[yokaiChoice - 1];
        int hasChapsal = 0;
        for (int i = 0; i < targetYokai->yokaiInventoryCount; i++) {
            if (strcmp(targetYokai->yokaiInventory[i].item.name, "찹살경단") == 0) {
                hasChapsal = 1;
                break;
            }
        }
        if (hasChapsal) {
            printTextAndWait("\n이미 찹살경단을 가지고 있습니다!");
            fastSleep(500);
            continue; // 요괴 선택 메뉴 반복
        } else {
            if (addItemToYokaiInventory(targetYokai, &candidates[idx])) {
                printTextAndWait("\n찹살경단을 요괴 인벤토리에 추가했습니다!");
            } else {
                printTextAndWait("\n찹살경단은 1개만 보유할 수 있습니다!");
            }
            fastSleep(500);
            break;
        }
    }
}

// 아이템 보상 시스템
void itemRewardSystem() {
    LOG_FUNCTION_EXECUTION("itemRewardSystem");
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
    
    // 찹살경단 분기를 최우선으로 처리
    if (strcmp(candidates[idx].name, "찹살경단") == 0) {
        selectYokaiForChapsalgyungdan(idx);
        return;
    }
    // 요괴형 아이템은 요괴 인벤토리로
    if (strcmp(candidates[idx].name, "돋보기") == 0 || 
        strcmp(candidates[idx].name, "복숭아") == 0 || 
        strcmp(candidates[idx].name, "고대의 서적") == 0 ||
        candidates[idx].type == ITEM_YANGGAENG) {  // 양갱형 아이템 추가
        
        // 요괴 선택 메뉴
        if (strcmp(candidates[idx].name, "돋보기") == 0) {
            printText("\n돋보기를 장착할 요괴를 선택하세요:\n");
        } else if (strcmp(candidates[idx].name, "복숭아") == 0) {
            printText("\n복숭아를 지니게 할 요괴를 선택하세요:\n");
        } else if (strcmp(candidates[idx].name, "고대의 서적") == 0) {
            printText("\n고대의 서적을 지니게 할 요괴를 선택하세요:\n");
        } else if (candidates[idx].type == ITEM_YANGGAENG) {
            if (strcmp(candidates[idx].name, "이상한 양갱") == 0) {
                // 이상한 양갱은 바로 사용
                addItemToInventory(&candidates[idx]);
                return;
            }
            printText("\n양갱을 먹을 요괴를 선택하세요:\n");
        } else {
            printText("\n회복할 요괴를 선택하세요:\n");
        }
        for (int i = 0; i < gPartyCount; i++) {
            char buffer[256];
            sprintf(buffer, "%d. %s Lv.%d (HP: %.0f/%.0f)\033[0m\n", 
                i + 1, 
                gParty[i].name, 
                gParty[i].level,
                gParty[i].currentHP,
                calculateHP(&gParty[i]));
            printText(buffer);
        }
        printText("0. 뒤로 가기\n");
        printText("숫자를 입력해주세요: ");
        
        int yokaiChoice = getIntInput();
        if (yokaiChoice == 0) {
            itemRewardSystem();
            return;
        }
        
        if (yokaiChoice < 1 || yokaiChoice > gPartyCount) {
            printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
            itemRewardSystem();
            return;
        }
        
        // 선택된 요괴에게 아이템 지급
        Yokai* targetYokai = &gParty[yokaiChoice - 1];
        int result = useYokaiItem(&candidates[idx], targetYokai);
        if (result == 1) {
            // 고급양갱은 메시지 출력하지 않음
            if (strcmp(candidates[idx].name, "고급양갱") != 0) {
                fastSleep(500);
            }
        } else if (result == -1) {
            // 최대 개수 초과 시 요괴 선택 메뉴로 복귀
            itemRewardSystem();
            return;
        } else if (candidates[idx].type == ITEM_YANGGAENG) {
            if (addItemToYokaiInventory(targetYokai, &candidates[idx])) {
                // 고급양갱은 메시지 출력하지 않음
                if (strcmp(candidates[idx].name, "고급양갱") != 0) {
                }
            } else {
                printTextAndWait("\n이미 최대치까지 보유하고 있습니다!");
            }
            fastSleep(500);
        }
    } else {
        // 일반 아이템은 플레이어 인벤토리에 추가
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
            // 5개 미만일 때만 메시지 출력 (중복 방지 위해 메시지 출력 코드 삭제)
            if (count < 5) {
                fastSleep(500);
            }
        }
        // 회복형, 양갱형 아이템이 아닌 경우에만 획득 메시지 출력 (중복 방지 위해 메시지 출력 코드 삭제)
        else if (candidates[idx].type != ITEM_HEAL && candidates[idx].type != ITEM_YANGGAENG) {
            fastSleep(500);
        }
    }
    
    // 보상 선택이 완료되면 초기화 플래그를 리셋
    isInitialized = 0;
    resetCount = 0;  // 초기화 횟수도 리셋
}

// 새 게임 시작 시 호출할 초기화 함수
void resetItemRewardSystem(void) {
    LOG_FUNCTION_EXECUTION("resetItemRewardSystem");
    resetCount = 0;
    isInitialized = 0;
    lastStageNumber = -1;
} 