#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include <stdbool.h>

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
} Event;

// 이벤트 시스템 함수들
void initEventSystem(void);
bool shouldTriggerEvent(void);
Event* generateRandomEvent(void);
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