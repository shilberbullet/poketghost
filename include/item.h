#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "yokai.h"

struct Yokai;

#define ITEM_NAME_MAX 32
#define ITEM_DESC_MAX 128
#define INVENTORY_MAX 20

// 아이템 등급
typedef enum {
    ITEM_COMMON,    // 일반
    ITEM_RARE,      // 희귀
    ITEM_SUPERRARE  // 초희귀
} ItemGrade;

// 아이템 타입
typedef enum {
    ITEM_HEAL,      // 회복형
    ITEM_TALISMAN,  // 부적형
    ITEM_PLAYER,    // 플레이어 아이템
    ITEM_YOKAI,     // 요괴 아이템
    ITEM_YANGGAENG  // 양갱형
} ItemType;

// 아이템 구조체
typedef struct {
    char name[ITEM_NAME_MAX];
    char desc[ITEM_DESC_MAX];
    ItemGrade grade;
    ItemType type;
} Item;

// 인벤토리용 구조체 (아이템+개수)
typedef struct {
    Item item;
    int count;
} InventoryItem;

// 전체 아이템 목록 (동적 배열)
extern Item* itemList;
extern int itemListCount;
extern int itemListCapacity;

// 인벤토리
extern InventoryItem inventory[INVENTORY_MAX];
extern int inventoryCount;

// 아이템 시스템 초기화
void initItemSystem();

// 아이템 시스템 정리
void cleanupItemSystem();

// 아이템 데이터 파일 로드
void loadItemsFromFile(const char* filename);

// 아이템 3개를 확률에 따라 랜덤 추출
void getRandomItems(Item* outItems, int count);

// 인벤토리에 아이템 추가
void addItemToInventory(const Item* item);

// 부적 사용 함수
bool useTalisman(const Item* item, Yokai* targetYokai);

// 양갱 사용 함수
bool useYanggaeng(const Item* item, Yokai* targetYokai);

// 무당방울 보너스 계산 함수
int calculateMudangBonus(int amount, int mudangCount);

// 인벤토리 출력 함수
void printInventory();

// 작두 아이템 개수 반환 함수
int getJakduCount();

#endif // ITEM_H 