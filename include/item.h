/**
 * @file item.h
 * @brief 포켓요괴 게임의 아이템 시스템 헤더 파일
 * @details 아이템 구조체, 인벤토리, 아이템 사용 등의 함수를 선언합니다.
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef ITEM_H
#define ITEM_H

#include <stdio.h> // 표준 입출력 함수
#include <stdlib.h> // 표준 라이브러리 함수
#include <string.h> // 문자열 처리 함수
#include <stdbool.h> // 불린 타입 사용을 위한 헤더
#include "types.h" // 기본 타입 정의

#define ITEM_NAME_MAX 32 // 아이템 이름의 최대 길이
#define ITEM_DESC_MAX 128 // 아이템 설명의 최대 길이
#define INVENTORY_MAX 20 // 인벤토리의 최대 아이템 수

// 아이템 등급
typedef enum {
    ITEM_COMMON,    // 일반 등급
    ITEM_RARE,      // 희귀 등급
    ITEM_SUPERRARE  // 초희귀 등급
} ItemGrade; // 아이템의 희귀도를 나타내는 열거형

// 아이템 타입
typedef enum {
    ITEM_HEAL,      // 회복형 아이템
    ITEM_TALISMAN,  // 부적형 아이템
    ITEM_PLAYER,    // 플레이어 아이템
    ITEM_YOKAI,     // 요괴 아이템
    ITEM_YANGGAENG, // 양갱형 아이템
    ITEM_FORGOTTEN_MOVE // 잊은 기술 배우기 아이템
} ItemType; // 아이템의 사용 목적을 나타내는 열거형

// 아이템 구조체
typedef struct {
    char name[ITEM_NAME_MAX]; // 아이템 이름
    char desc[ITEM_DESC_MAX]; // 아이템 설명
    ItemGrade grade; // 아이템 등급
    ItemType type; // 아이템 타입
} Item; // 아이템의 기본 정보를 담는 구조체

// 인벤토리용 구조체 (아이템+개수)
typedef struct {
    Item item; // 아이템 정보
    int count; // 보유 개수
} InventoryItem; // 인벤토리에 저장되는 아이템 구조체

// 전체 아이템 목록 (동적 배열)
extern Item* itemList; // 모든 아이템 목록
extern int itemListCount; // 아이템 목록 개수
extern int itemListCapacity; // 아이템 목록 용량

// 인벤토리
extern InventoryItem inventory[INVENTORY_MAX]; // 플레이어 인벤토리
extern int inventoryCount; // 인벤토리 아이템 개수

// 아이템 시스템 초기화
/**
 * @brief 아이템 시스템을 초기화하는 함수
 * @details 아이템 목록과 인벤토리를 초기화합니다.
 */
void initItemSystem();

// 아이템 시스템 정리
/**
 * @brief 아이템 시스템을 정리하는 함수
 * @details 동적 할당된 메모리를 해제합니다.
 */
void cleanupItemSystem();

// 아이템 데이터 파일 로드
/**
 * @brief 파일에서 아이템 데이터를 로드하는 함수
 * @param filename 로드할 파일명
 */
void loadItemsFromFile(const char* filename);

// 아이템 3개를 확률에 따라 랜덤 추출
/**
 * @brief 확률에 따라 랜덤 아이템을 추출하는 함수
 * @param outItems 추출된 아이템을 저장할 배열
 * @param count 추출할 아이템 개수
 */
void getRandomItems(Item* outItems, int count);

// 인벤토리에 아이템 추가
/**
 * @brief 인벤토리에 아이템을 추가하는 함수
 * @param item 추가할 아이템
 */
void addItemToInventory(const Item* item);

// 메시지 없이 인벤토리에 아이템 추가
/**
 * @brief 메시지 없이 인벤토리에 아이템을 추가하는 함수
 * @param item 추가할 아이템
 */
void addItemToInventoryWithoutMessage(const Item* item);

// 부적 사용 함수
/**
 * @brief 부적 아이템을 사용하는 함수
 * @param item 사용할 부적 아이템
 * @param targetYokai 대상 요괴
 * @return 사용 성공 여부
 */
bool useTalisman(const Item* item, void* targetYokai);

// 양갱 사용 함수
/**
 * @brief 양갱 아이템을 사용하는 함수
 * @param item 사용할 양갱 아이템
 * @param targetYokai 대상 요괴
 * @return 사용 성공 여부
 */
bool useYanggaeng(const Item* item, void* targetYokai);

// 잊은 기술 배우기 함수
/**
 * @brief 잊은 기술을 다시 배우는 아이템을 사용하는 함수
 * @param item 사용할 아이템
 * @param targetYokai 대상 요괴
 * @return 사용 성공 여부
 */
bool useForgottenMoveItem(const Item* item, void* targetYokai);

// 무당방울 보너스 계산 함수
/**
 * @brief 무당방울 개수에 따른 보너스를 계산하는 함수
 * @param amount 기본 금액
 * @param mudangCount 무당방울 개수
 * @return 계산된 보너스 금액
 */
int calculateMudangBonus(int amount, int mudangCount);

// 인벤토리 출력 함수
/**
 * @brief 인벤토리 내용을 화면에 출력하는 함수
 */
void printInventory();

// 작두 아이템 개수 반환 함수
/**
 * @brief 인벤토리에서 작두 아이템의 개수를 반환하는 함수
 * @return 작두 아이템 개수
 */
int getJakduCount();

/**
 * @brief 요괴 전용 아이템을 사용하는 함수
 * @param item 사용할 아이템
 * @param targetYokai 대상 요괴
 * @return 사용 성공 여부
 */
int useYokaiItem(const Item* item, void* targetYokai);

#endif // ITEM_H 