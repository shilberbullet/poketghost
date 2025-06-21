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

// 편지 전달 이벤트 처리
void handleLetterDeliveryEvent(Event* event) {
    LOG_FUNCTION_EXECUTION("handleLetterDeliveryEvent");
    
    system("cls");
    printText("=== 이벤트 발생! ===\n\n");
    // 여인의 요청 대화
    startDialogue(1007);
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "목표 지역: %s\n", event->target_region);
    printText(buffer);
    printText("\n1. 예\n");
    printText("2. 아니요\n\n");
    printText("선택하세요: ");
    
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
        printText("\n아무 키나 누르면 계속합니다...\n");
        _getch();
    } else {
        // 편지 전달 거절
        printText("\n편지 전달을 거절했습니다.\n");
        printText("\n아무 키나 누르면 계속합니다...\n");
        _getch();
        free(event);
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
            printText("=== 이벤트 완료! ===\n\n");
            printText("편지를 전달했습니다!\n");
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "보상으로 %d전을 받았습니다.\n", event->reward_money);
            printText(buffer);
            
            // 보상 지급
            gPlayer.money += event->reward_money;
            
            printText("\n아무 키나 누르면 계속합니다...\n");
            _getch();
            break;
        default:
            break;
    }
    
    // 이벤트 완료 후 메모리 해제
    free(event);
    currentEvent = NULL;
}

// 현재 활성 이벤트 반환
Event* getCurrentEvent(void) {
    return currentEvent;
}

// 파일에서 이벤트 로드
Event* loadEventFromFile(FILE* file) {
    LOG_FUNCTION_EXECUTION("loadEventFromFile");
    
    if (!file) return NULL;
    
    Event* event = (Event*)malloc(sizeof(Event));
    if (!event) {
        return NULL;
    }
    
    // 이벤트 데이터 읽기
    fread(&event->type, sizeof(EventType), 1, file);
    fread(&event->description, sizeof(char), 256, file);
    fread(&event->target_region, sizeof(char), 32, file);
    fread(&event->reward_money, sizeof(int), 1, file);
    fread(&event->is_completed, sizeof(bool), 1, file);
    
    return event;
}

// 현재 이벤트 설정
void setCurrentEvent(Event* event) {
    LOG_FUNCTION_EXECUTION("setCurrentEvent");
    currentEvent = event;
} 