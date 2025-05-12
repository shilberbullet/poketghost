#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

// 아이템 목록 정의
const Item itemList[ITEM_LIST_MAX] = {
    // 일반
    {"낡은부적", "요괴를 낮은 확률로 동료로 만들수 있다", ITEM_COMMON, ITEM_TALISMAN},
    {"제삿밥", "동료 요괴의 약간의 체력을 회복 할수있다", ITEM_COMMON, ITEM_HEAL},
    // 희귀
    {"빳빳한 부적", "요괴를 높은 확률로 동료로 만들수 있다", ITEM_RARE, ITEM_TALISMAN},
    {"나물", "동료 요괴의 많은 체력을 회복한다", ITEM_RARE, ITEM_HEAL},
    {"탕국", "기절한 동료를 깨우고 체력의 반을 회복한다", ITEM_RARE, ITEM_HEAL},
    // 초희귀
    {"신비한 부적", "요괴를 반드시 동료로 만들수 있다", ITEM_SUPERRARE, ITEM_TALISMAN},
    {"막걸리", "기절한 동료를 깨우고 모든 체력을 회복한다", ITEM_SUPERRARE, ITEM_HEAL},
    {"무당방울", "퇴마사가 지니고 있으면 클리어 후 더 많은 돈을 얻는다", ITEM_SUPERRARE, ITEM_EQUIP},
    {"작두", "퇴마사가 지니고 있으면 요괴들의 공격력이 올라간다", ITEM_SUPERRARE, ITEM_EQUIP},
    {"복숭아", "동료 요괴가 지니고 있으면 매턴 약간의 체력을 회복한다", ITEM_SUPERRARE, ITEM_EQUIP}
};
const int itemListCount = 10;

// 인벤토리
InventoryItem inventory[INVENTORY_MAX];
int inventoryCount = 0;

// 등급별 등장 확률 (누적)
// 일반: 70%, 희귀: 25%, 초희귀: 5%
static const int gradeProb[3] = {70, 95, 100};

// 아이템 3개를 확률에 따라 랜덤 추출
void getRandomItems(Item* outItems, int count) {
    int selected[ITEM_LIST_MAX] = {0};
    int found = 0;
    while (found < count) {
        int r = rand() % 100;
        ItemGrade grade = ITEM_COMMON;
        if (r < gradeProb[0]) grade = ITEM_COMMON;
        else if (r < gradeProb[1]) grade = ITEM_RARE;
        else grade = ITEM_SUPERRARE;
        // 해당 등급에서 랜덤 추출
        int candidates[ITEM_LIST_MAX];
        int c = 0;
        for (int i = 0; i < itemListCount; i++) {
            if (itemList[i].grade == grade && !selected[i]) {
                candidates[c++] = i;
            }
        }
        if (c == 0) continue;
        int pick = candidates[rand() % c];
        outItems[found++] = itemList[pick];
        selected[pick] = 1;
    }
}

// 인벤토리에 아이템 추가 (중복시 개수 증가)
void addItemToInventory(const Item* item) {
    for (int i = 0; i < inventoryCount; i++) {
        if (strcmp(inventory[i].item.name, item->name) == 0) {
            inventory[i].count++;
            return;
        }
    }
    if (inventoryCount < INVENTORY_MAX) {
        inventory[inventoryCount].item = *item;
        inventory[inventoryCount].count = 1;
        inventoryCount++;
    }
} 