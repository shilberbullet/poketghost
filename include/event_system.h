#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include <stdbool.h>
#include "item.h"

// 이벤트 타입 정의
typedef enum {
    EVENT_LETTER_DELIVERY,  // 편지 전달 이벤트
    EVENT_COUNT
} EventType;

// 이벤트 구조체
typedef struct {
    EventType type;
    char description[256];
    char target_region[32];
    int reward_money;
    bool is_completed;
    bool target_reached;  // 목표지역 도달 여부 추가
    int created_stage;    // 이벤트 생성 시점의 스테이지 번호 추가
} Event;

// 이벤트 시스템 함수들
void initEventSystem(void);
bool shouldTriggerEvent(void);
int calculateEventReward(int stageNumber);
Event* generateRandomEvent(int stageNumber);
void handleEvent(Event* event);
bool checkEventCompletion(Event* event);
void completeEvent(Event* event);
Event* getCurrentEvent(void);
Event* loadEventFromFile(FILE* file);
void setCurrentEvent(Event* event);

// 편지 전달 이벤트 관련 함수
void handleLetterDeliveryEvent(Event* event);
bool isLetterDeliveryCompleted(Event* event);

#endif // EVENT_SYSTEM_H 