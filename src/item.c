#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "text.h"
#include "yokai.h"

// 아이템 목록
Item itemList[ITEM_LIST_MAX];
int itemListCount = 0;

// 인벤토리
InventoryItem inventory[INVENTORY_MAX];
int inventoryCount = 0;

// 문자열을 ItemGrade로 변환
ItemGrade stringToGrade(const char* str) {
    if (strcmp(str, "COMMON") == 0) return ITEM_COMMON;
    if (strcmp(str, "RARE") == 0) return ITEM_RARE;
    if (strcmp(str, "SUPERRARE") == 0) return ITEM_SUPERRARE;
    return ITEM_COMMON;
}

// 문자열을 ItemType으로 변환
ItemType stringToType(const char* str) {
    if (strcmp(str, "HEAL") == 0) return ITEM_HEAL;
    if (strcmp(str, "TALISMAN") == 0) return ITEM_TALISMAN;
    if (strcmp(str, "PLAYER") == 0) return ITEM_PLAYER;
    if (strcmp(str, "YOKAI") == 0) return ITEM_YOKAI;
    return ITEM_HEAL;
}

// 아이템 데이터 파일 로드
void loadItemsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("아이템 데이터 파일을 열 수 없습니다: %s\n", filename);
        return;
    }

    char line[256];
    itemListCount = 0;

    while (fgets(line, sizeof(line), file) && itemListCount < ITEM_LIST_MAX) {
        // 주석이나 빈 줄 무시
        if (line[0] == '#' || line[0] == '\n') continue;

        char name[ITEM_NAME_MAX];
        char gradeStr[32];
        char typeStr[32];
        char desc[ITEM_DESC_MAX];

        // CSV 형식 파싱
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^\n]", 
            name, gradeStr, typeStr, desc) == 4) {
            
            Item* item = &itemList[itemListCount];
            strncpy(item->name, name, ITEM_NAME_MAX - 1);
            strncpy(item->desc, desc, ITEM_DESC_MAX - 1);
            item->grade = stringToGrade(gradeStr);
            item->type = stringToType(typeStr);
            
            itemListCount++;
        }
    }

    fclose(file);
}

// 아이템 3개를 확률에 따라 랜덤 추출
void getRandomItems(Item* outItems, int count) {
    for (int i = 0; i < count; i++) {
        int randomValue = rand() % 100;
        ItemGrade targetGrade;
        
        // 등급별 확률 설정
        if (randomValue < 70) targetGrade = ITEM_COMMON;      // 70%
        else if (randomValue < 95) targetGrade = ITEM_RARE;   // 25%
        else targetGrade = ITEM_SUPERRARE;             // 5%

        // 해당 등급의 아이템 중 랜덤 선택
        int validItems = 0;
        int validIndices[ITEM_LIST_MAX];
        
        for (int j = 0; j < itemListCount; j++) {
            if (itemList[j].grade == targetGrade) {
                validIndices[validItems++] = j;
            }
        }

        if (validItems > 0) {
            int selectedIndex = validIndices[rand() % validItems];
            outItems[i] = itemList[selectedIndex];
        }
    }
}

// 인벤토리에 아이템 추가
void addItemToInventory(const Item* item) {
    // 이미 있는 아이템인지 확인
    for (int i = 0; i < inventoryCount; i++) {
        if (strcmp(inventory[i].item.name, item->name) == 0) {
            inventory[i].count++;
            return;
        }
    }

    // 새로운 아이템 추가
    if (inventoryCount < INVENTORY_MAX) {
        inventory[inventoryCount].item = *item;
        inventory[inventoryCount].count = 1;
        inventoryCount++;
    }
}

// 인벤토리 출력 함수
void printInventory() {
    if (inventoryCount == 0) {
        printTextAndWait("\n인벤토리가 비어있습니다.");
        return;
    }

    printText("\n=== 인벤토리 ===\n");
    for (int i = 0; i < inventoryCount; i++) {
        char buffer[256];
        sprintf(buffer, "%d. %s [%s] x%d\n    %s\n", 
            i+1, 
            inventory[i].item.name,
            inventory[i].item.grade == ITEM_COMMON ? "일반" : 
            inventory[i].item.grade == ITEM_RARE ? "희귀" : "초희귀",
            inventory[i].count,
            inventory[i].item.desc);
        printText(buffer);
    }
    printTextAndWait("\n");
}

// 부적 사용 함수
bool useTalisman(const Item* item, Yokai* targetYokai) {
    if (item->type != ITEM_TALISMAN) {
        printf("이 아이템은 부적이 아닙니다!\n");
        return false;
    }
    // 부적 등급에 따라 포획 확률 결정
    float catchRate = 0.0f;
    switch (item->grade) {
        case ITEM_COMMON:
            catchRate = 0.3f;
            break;
        case ITEM_RARE:
            catchRate = 0.5f;
            break;
        case ITEM_SUPERRARE:
            catchRate = 0.8f;
            break;
        default:
            catchRate = 0.3f;
    }
    if ((float)rand() / RAND_MAX < catchRate) {
        printf("포획 성공!\n");
        return true;
    } else {
        printf("포획 실패...\n");
        return false;
    }
} 