#ifndef ITEM_H           // 중복 포함 방지를 위한 전처리기 지시문 시작
#define ITEM_H           // 중복 포함 방지를 위한 매크로 정의

#include <stdio.h>      // 표준 입출력 함수 (FILE, printf 등) 사용을 위한 헤더
#include <stdlib.h>     // 표준 라이브러리 함수 (malloc, free 등) 사용을 위한 헤더
#include <string.h>     // 문자열 처리 함수 (strcpy, strcmp 등) 사용을 위한 헤더
#include <stdbool.h>    // 불린 타입 사용을 위한 헤더 (bool, true, false)
#include "types.h"     // 기본 타입 정의 (공용 타입, 상수 등)

#define ITEM_NAME_MAX 32      // 아이템 이름의 최대 길이 (문자열 배열 크기)
#define ITEM_DESC_MAX 128     // 아이템 설명의 최대 길이 (문자열 배열 크기)
#define INVENTORY_MAX 20      // 인벤토리의 최대 아이템 수 (플레이어가 가질 수 있는 아이템 개수)

// 아이템 등급 (희귀도 구분)
typedef enum {
    ITEM_COMMON,    // 일반 등급 (가장 흔함)
    ITEM_RARE,      // 희귀 등급 (중간 희귀)
    ITEM_SUPERRARE  // 초희귀 등급 (가장 희귀)
} ItemGrade; // 아이템의 희귀도를 나타내는 열거형 (int형)

// 아이템 타입 (사용 목적 구분)
typedef enum {
    ITEM_HEAL,      // 회복형 아이템 (HP, PP 회복 등)
    ITEM_TALISMAN,  // 부적형 아이템 (특수 효과)
    ITEM_PLAYER,    // 플레이어 아이템 (플레이어 전용)
    ITEM_YOKAI,     // 요괴 아이템 (요괴 전용)
    ITEM_YANGGAENG, // 양갱형 아이템 (특수 회복/효과)
    ITEM_FORGOTTEN_MOVE // 잊은 기술 배우기 아이템
} ItemType; // 아이템의 사용 목적을 나타내는 열거형 (int형)

// 아이템 구조체 (기본 정보)
typedef struct {
    char name[ITEM_NAME_MAX]; // 아이템 이름 (최대 ITEM_NAME_MAX 길이)
    char desc[ITEM_DESC_MAX]; // 아이템 설명 (최대 ITEM_DESC_MAX 길이)
    ItemGrade grade; // 아이템 등급 (희귀도)
    ItemType type; // 아이템 타입 (사용 목적)
} Item; // 아이템의 기본 정보를 담는 구조체

// 인벤토리용 구조체 (아이템+개수)
typedef struct {
    Item item; // 아이템 정보 (이름, 설명, 등급, 타입)
    int count; // 보유 개수 (플레이어가 가진 수량)
} InventoryItem; // 인벤토리에 저장되는 아이템 구조체

// 전체 아이템 목록 (동적 배열)
extern Item* itemList; // 모든 아이템 목록 (동적 할당)
extern int itemListCount; // 아이템 목록 개수 (현재 등록된 아이템 수)
extern int itemListCapacity; // 아이템 목록 용량 (동적 배열의 크기)

// 인벤토리
extern InventoryItem inventory[INVENTORY_MAX]; // 플레이어 인벤토리 (최대 INVENTORY_MAX개)
extern int inventoryCount; // 인벤토리 아이템 개수 (현재 보유 중인 아이템 종류 수)

// 아이템 시스템 초기화
/**
 * @brief 아이템 시스템을 초기화하는 함수
 * @details 아이템 목록과 인벤토리를 초기화합니다.
 */
void initItemSystem(); // 아이템 목록, 인벤토리 등 내부 상태를 초기화하는 함수 선언

// 아이템 시스템 정리
/**
 * @brief 아이템 시스템을 정리하는 함수
 * @details 동적 할당된 메모리를 해제합니다.
 */
void cleanupItemSystem(); // 동적 할당된 아이템 목록 메모리를 해제하는 함수 선언

// 아이템 데이터 파일 로드
/**
 * @brief 파일에서 아이템 데이터를 로드하는 함수
 * @param filename 로드할 파일명 (텍스트 파일 경로)
 */
void loadItemsFromFile(const char* filename); // 파일에서 아이템 데이터를 읽어오는 함수 선언

// 아이템 3개를 확률에 따라 랜덤 추출
/**
 * @brief 확률에 따라 랜덤 아이템을 추출하는 함수
 * @param outItems 추출된 아이템을 저장할 배열
 * @param count 추출할 아이템 개수
 */
void getRandomItems(Item* outItems, int count); // 확률에 따라 랜덤 아이템을 추출하는 함수 선언

// 인벤토리에 아이템 추가
/**
 * @brief 인벤토리에 아이템을 추가하는 함수
 * @param item 추가할 아이템 (포인터)
 */
void addItemToInventory(const Item* item); // 인벤토리에 아이템을 추가하는 함수 선언

// 메시지 없이 인벤토리에 아이템 추가
/**
 * @brief 메시지 없이 인벤토리에 아이템을 추가하는 함수
 * @param item 추가할 아이템 (포인터)
 */
void addItemToInventoryWithoutMessage(const Item* item); // 메시지 출력 없이 인벤토리에 아이템을 추가하는 함수 선언

// 부적 사용 함수
/**
 * @brief 부적 아이템을 사용하는 함수
 * @param item 사용할 부적 아이템 (포인터)
 * @param targetYokai 대상 요괴 (void*로 전달)
 * @return 사용 성공 여부 (true/false)
 */
bool useTalisman(const Item* item, void* targetYokai); // 부적 아이템을 사용하여 효과를 적용하는 함수 선언

// 양갱 사용 함수
/**
 * @brief 양갱 아이템을 사용하는 함수
 * @param item 사용할 양갱 아이템 (포인터)
 * @param targetYokai 대상 요괴 (void*로 전달)
 * @return 사용 성공 여부 (true/false)
 */
bool useYanggaeng(const Item* item, void* targetYokai); // 양갱 아이템을 사용하여 효과를 적용하는 함수 선언

// 잊은 기술 배우기 함수
/**
 * @brief 잊은 기술을 다시 배우는 아이템을 사용하는 함수
 * @param item 사용할 아이템 (포인터)
 * @param targetYokai 대상 요괴 (void*로 전달)
 * @return 사용 성공 여부 (true/false)
 */
bool useForgottenMoveItem(const Item* item, void* targetYokai); // 잊은 기술을 다시 배우는 아이템 사용 함수 선언

// 무당방울 보너스 계산 함수
/**
 * @brief 무당방울 개수에 따른 보너스를 계산하는 함수
 * @param amount 기본 금액 (int)
 * @param mudangCount 무당방울 개수 (int)
 * @return 계산된 보너스 금액 (int)
 */
int calculateMudangBonus(int amount, int mudangCount); // 무당방울 개수에 따라 보너스 금액을 계산하는 함수 선언

// 인벤토리 출력 함수
/**
 * @brief 인벤토리 내용을 화면에 출력하는 함수
 */
void printInventory(); // 현재 인벤토리의 모든 아이템 정보를 출력하는 함수 선언

// 작두 아이템 개수 반환 함수
/**
 * @brief 인벤토리에서 작두 아이템의 개수를 반환하는 함수
 * @return 작두 아이템 개수 (int)
 */
int getJakduCount(); // 인벤토리 내 작두 아이템의 개수를 반환하는 함수 선언

/**
 * @brief 요괴 전용 아이템을 사용하는 함수
 * @param item 사용할 아이템 (포인터)
 * @param targetYokai 대상 요괴 (void*로 전달)
 * @return 사용 성공 여부 (int)
 */
int useYokaiItem(const Item* item, void* targetYokai); // 요괴 전용 아이템을 사용하여 효과를 적용하는 함수 선언

#endif // ITEM_H           // 중복 포함 방지를 위한 전처리기 지시문 종료 