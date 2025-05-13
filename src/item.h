#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "yokai.h"

#define ITEM_NAME_MAX 32
#define ITEM_DESC_MAX 128
#define ITEM_LIST_MAX 32
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
    ITEM_YOKAI      // 요괴 아이템
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

// 전체 아이템 목록
extern Item itemList[ITEM_LIST_MAX];
extern int itemListCount;

// 인벤토리
extern InventoryItem inventory[INVENTORY_MAX];
extern int inventoryCount;

// 아이템 데이터 파일 로드
void loadItemsFromFile(const char* filename);

// 아이템 3개를 확률에 따라 랜덤 추출
void getRandomItems(Item* outItems, int count);

// 인벤토리에 아이템 추가
void addItemToInventory(const Item* item);

// 부적 사용 함수
bool useTalisman(const Item* item, Yokai* targetYokai);

// 인벤토리 출력 함수
void printInventory();

#endif // ITEM_H 