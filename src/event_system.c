#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "event_system.h"
#include "text.h"
#include "input.h"
#include "game.h"
#include "region.h"
#include "../core/state.h"
#include "logger.h"
#include "dialogue.h"

// 이벤트 발생 확률 설정
#define EVENT_TRIGGER_CHANCE 20  // 20% 확률

// 전역 변수로 현재 활성 이벤트 저장
static Event* currentEvent = NULL;

// 이벤트 시스템 초기화
void initEventSystem(void) {
    LOG_FUNCTION_EXECUTION("initEventSystem");
    if (currentEvent) {
        free(currentEvent);
        currentEvent = NULL;
    }
}

// 이벤트 발생 확률 체크
bool shouldTriggerEvent(void) {
    LOG_FUNCTION_EXECUTION("shouldTriggerEvent");
    if (currentEvent != NULL) {
        return false; // 이미 활성 이벤트가 있으면 새로운 이벤트 발생 안함
    }
    
    // 11스테이지 이전에는 이벤트 발생 안함
    if (gStage.stageNumber < 11) {
        return false;
    }
    
    // 70스테이지부터는 이벤트 발생 안함 (더 이상 전달할 곳이 없음)
    if (gStage.stageNumber >= 70) {
        return false;
    }
    
    // 파이널 스테이지(81스테이지부터)에서는 이벤트 발생 안함
    if (gStage.stageNumber >= 81) {
        return false;
    }
    
    // 일의 자리가 2, 3, 4, 7, 8인 경우에만 이벤트 발생
    int lastDigit = gStage.stageNumber % 10;
    if (!(lastDigit == 2 || lastDigit == 3 || lastDigit == 4 || lastDigit == 7 || lastDigit == 8)) {
        return false;
    }
    
    int chance = rand() % 100;
    return chance < EVENT_TRIGGER_CHANCE; // 설정된 확률로 이벤트 발생
}

// 랜덤 이벤트 생성
Event* generateRandomEvent(void) {
    LOG_FUNCTION_EXECUTION("generateRandomEvent");
    
    Event* event = (Event*)malloc(sizeof(Event));
    if (!event) {
        return NULL;
    }
    
    // 현재는 편지 전달 이벤트만 구현
    event->type = EVENT_LETTER_DELIVERY;
    event->is_completed = false;
    event->reward_money = 100 + (rand() % 200); // 100~300전 보상
    
    // 방문하지 않은 지역 중에서 목표 지역 설정
    char unvisitedRegions[MAX_REGIONS][REGION_NAME_LENGTH];
    int unvisitedCount = getUnvisitedRegions(unvisitedRegions, MAX_REGIONS);
    
    if (unvisitedCount > 0) {
        // 방문하지 않은 지역 중에서 랜덤 선택
        int targetIndex = rand() % unvisitedCount;
        strcpy(event->target_region, unvisitedRegions[targetIndex]);
    } else {
        // 모든 지역을 방문했다면 현재 지역이 아닌 다른 지역 선택
        const char* currentRegion = getCurrentRegion();
        const char* allRegions[] = {"함경도", "평안도", "황해도", "강원도", "경기도", "충청도", "경상도", "전라도"};
        int targetIndex;
        
        do {
            targetIndex = rand() % 8;
        } while (strcmp(allRegions[targetIndex], currentRegion) == 0);
        
        strcpy(event->target_region, allRegions[targetIndex]);
    }
    
    // 이벤트 설명 생성
    snprintf(event->description, sizeof(event->description),
             "한 여인이 말을 걸어왔다\n여인: 이 편지를 %s에 전해줘", event->target_region);
    
    return event;
}

// 이벤트 처리
void handleEvent(Event* event) {
    LOG_FUNCTION_EXECUTION("handleEvent");
    
    if (!event) return;
    
    switch (event->type) {
        case EVENT_LETTER_DELIVERY:
            handleLetterDeliveryEvent(event);
            break;
        default:
            break;
    }
}

// 편지 아이템을 찾거나 생성하는 함수
Item* getLetterItem(const char* targetRegion) {
    LOG_FUNCTION_EXECUTION("getLetterItem");
    
    // 기존 편지 아이템이 있는지 확인
    for (int i = 0; i < itemListCount; i++) {
        if (strcmp(itemList[i].name, "편지") == 0) {
            return &itemList[i];
        }
    }
    
    // 편지 아이템이 없으면 생성
    if (itemListCount >= itemListCapacity) {
        // 아이템 목록 확장 필요
        int newCapacity = itemListCapacity * 2;
        Item* newList = (Item*)realloc(itemList, newCapacity * sizeof(Item));
        if (newList == NULL) {
            return NULL;
        }
        itemList = newList;
        itemListCapacity = newCapacity;
    }
    
    // 편지 아이템 생성
    Item* letterItem = &itemList[itemListCount];
    strcpy(letterItem->name, "편지");
    char desc[ITEM_DESC_MAX];
    snprintf(desc, sizeof(desc), "%s로 전달할 편지입니다.", targetRegion);
    strcpy(letterItem->desc, desc);
    letterItem->grade = ITEM_COMMON;
    letterItem->type = ITEM_PLAYER;
    
    itemListCount++;
    return letterItem;
}

// 편지 전달 이벤트 처리
void handleLetterDeliveryEvent(Event* event) {
    LOG_FUNCTION_EXECUTION("handleLetterDeliveryEvent");
    
    system("cls");
    printText("=== 스테이지 정보 ===\n\n");
    
    // 스테이지 정보 출력
    char buffer[256];
    sprintf(buffer, "스테이지: %d\n", gStage.stageNumber);
    printText(buffer);
    sprintf(buffer, "지역: %s\n", getCurrentRegion());
    printText(buffer);
    sprintf(buffer, "지형: %s\n", getCurrentTerrain());
    printText(buffer);
    int hour = (gStage.stageNumber - 1) % 24;
    sprintf(buffer, "시간: %02d시\n", hour);
    printText(buffer);
    sprintf(buffer, "보유 전: %d전\n", gPlayer.money);
    printText(buffer);
    
    // 이벤트 대화
    printText("모르는 여인이 말을 걸어 온다\n");
    startDialogue(1007);
    sprintf(buffer, "전달지역: %s\n", event->target_region);
    printText(buffer);
    printText("편지를 전달해주시겠습니까? \n");
    printText("1. 예\n");
    printText("2. 아니요\n");
    printText("숫자를 입력하세요: ");
    
    int choice = getIntInput();
    while (choice != 1 && choice != 2) {
        printText("\n잘못된 선택입니다. 다시 선택하세요: ");
        choice = getIntInput();
    }
    
    if (choice == 1) {
        // 편지 전달 수락
        currentEvent = event;
        startDialogue(1008);
        printText("목표 지역에 도달하면 편지를 전달할 수 있습니다.\n");
        
        // 편지 아이템을 인벤토리에 추가
        Item* letterItem = getLetterItem(event->target_region);
        if (letterItem) {
            addItemToInventoryWithoutMessage(letterItem);
            printText("편지를 인벤토리에 받았습니다.\n");
        }
        
        // 이벤트 처리 후 스테이지 정보 출력 건너뛰기
        gGameState.skipStageInfo = true;
    } else {
        // 편지 전달 거절
        startDialogue(1009);
        printText("\n편지 전달을 거절했습니다.\n");
        free(event);
        // 이벤트 처리 후 스테이지 정보 출력 건너뛰기
        gGameState.skipStageInfo = true;
    }
}

// 이벤트 완료 체크
bool checkEventCompletion(Event* event) {
    LOG_FUNCTION_EXECUTION("checkEventCompletion");
    
    if (!event || event->is_completed) {
        return false;
    }
    
    switch (event->type) {
        case EVENT_LETTER_DELIVERY:
            return isLetterDeliveryCompleted(event);
        default:
            return false;
    }
}

// 편지 전달 완료 체크
bool isLetterDeliveryCompleted(Event* event) {
    LOG_FUNCTION_EXECUTION("isLetterDeliveryCompleted");
    
    if (!event || event->is_completed) {
        return false;
    }
    
    const char* currentRegion = getCurrentRegion();
    return strcmp(currentRegion, event->target_region) == 0;
}

// 이벤트 완료 처리
void completeEvent(Event* event) {
    LOG_FUNCTION_EXECUTION("completeEvent");
    
    if (!event || event->is_completed) {
        return;
    }
    
    event->is_completed = true;
    
    switch (event->type) {
        case EVENT_LETTER_DELIVERY:
            system("cls");
            printText("=== 스테이지 정보 ===\n\n");
            
            // 스테이지 정보 출력
            char buffer[256];
            sprintf(buffer, "스테이지: %d\n", gStage.stageNumber);
            printText(buffer);
            sprintf(buffer, "지역: %s\n", getCurrentRegion());
            printText(buffer);
            sprintf(buffer, "지형: %s\n", getCurrentTerrain());
            printText(buffer);
            int hour = (gStage.stageNumber - 1) % 24;
            sprintf(buffer, "시간: %02d시\n", hour);
            printText(buffer);
            sprintf(buffer, "보유 전: %d전\n", gPlayer.money);
            printText(buffer);
            
            // 이벤트 완료 메시지
            printText("편지를 전달했습니다!\n");
            snprintf(buffer, sizeof(buffer), "보상으로 %d전을 받았습니다.\n", event->reward_money);
            printText(buffer);
            
            // 보상 지급
            gPlayer.money += event->reward_money;
            
            // 편지 아이템을 인벤토리에서 제거
            for (int i = 0; i < inventoryCount; i++) {
                if (strcmp(inventory[i].item.name, "편지") == 0) {
                    // 편지 아이템 제거
                    if (i < inventoryCount - 1) {
                        inventory[i] = inventory[inventoryCount - 1];
                    }
                    inventoryCount--;
                    printText("편지를 인벤토리에서 제거했습니다.\n");
                    break;
                }
            }
            
            // 이벤트 처리 후 스테이지 정보 출력 건너뛰기
            gGameState.skipStageInfo = true;
            break;
        default:
            break;
    }
    
    // 이벤트 완료 후 메모리 해제
    free(event);
    currentEvent = NULL;
}

// 현재 활성 이벤트 반환
Event* getCurrentEvent(void) { // 현재 활성 이벤트 반환 함수 시작
    return currentEvent; // 현재 이벤트 포인터 반환
}

// 파일에서 이벤트 로드
Event* loadEventFromFile(FILE* file) { // 파일에서 이벤트 로드 함수 시작
    LOG_FUNCTION_EXECUTION("loadEventFromFile"); // 함수 실행 로그 기록
    
    if (!file) return NULL; // 파일이 NULL인 경우 NULL 반환
    
    Event* event = (Event*)malloc(sizeof(Event)); // 이벤트 구조체 메모리 할당
    if (!event) { // 메모리 할당 실패한 경우
        return NULL; // NULL 반환
    }
    
    // 이벤트 데이터 읽기
    fread(&event->type, sizeof(EventType), 1, file); // 이벤트 타입 읽기
    fread(&event->description, sizeof(char), 256, file); // 이벤트 설명 읽기
    fread(&event->target_region, sizeof(char), 32, file); // 목표 지역 읽기
    fread(&event->reward_money, sizeof(int), 1, file); // 보상 금액 읽기
    fread(&event->is_completed, sizeof(bool), 1, file); // 완료 상태 읽기
    
    return event; // 로드된 이벤트 반환
}

// 현재 이벤트 설정
void setCurrentEvent(Event* event) { // 현재 이벤트 설정 함수 시작
    LOG_FUNCTION_EXECUTION("setCurrentEvent"); // 함수 실행 로그 기록
    currentEvent = event; // 현재 이벤트 포인터 설정
} 