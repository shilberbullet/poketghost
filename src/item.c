#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "item.h"
#include "yokai.h"
#include "text.h"
#include "input.h"
#include "game.h"
#include "heal_system.h"
#include "sikhye_system.h"
#include "reward.h"
#include "party.h"
#include "../core/state.h"

#define INITIAL_ITEM_CAPACITY 32

// 아이템 목록 (동적 배열)
Item* itemList = NULL;
int itemListCount = 0;
int itemListCapacity = 0;

// 인벤토리
InventoryItem inventory[INVENTORY_MAX];
int inventoryCount = 0;

// 아이템 시스템 초기화
void initItemSystem() {
    itemList = (Item*)malloc(INITIAL_ITEM_CAPACITY * sizeof(Item));
    if (itemList == NULL) {
        printf("아이템 시스템 초기화 실패: 메모리 할당 오류\n");
        exit(1);
    }
    itemListCapacity = INITIAL_ITEM_CAPACITY;
    itemListCount = 0;
}

// 아이템 시스템 정리
void cleanupItemSystem() {
    if (itemList != NULL) {
        free(itemList);
        itemList = NULL;
    }
    itemListCount = 0;
    itemListCapacity = 0;
}

// 아이템 목록 크기 확장
static void expandItemList() {
    int newCapacity = itemListCapacity * 2;
    Item* newList = (Item*)realloc(itemList, newCapacity * sizeof(Item));
    if (newList == NULL) {
        printf("아이템 목록 확장 실패: 메모리 할당 오류\n");
        return;
    }
    itemList = newList;
    itemListCapacity = newCapacity;
}

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
    if (strcmp(str, "YANGGAENG") == 0) return ITEM_YANGGAENG;
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

    while (fgets(line, sizeof(line), file)) {
        // 주석이나 빈 줄 무시
        if (line[0] == '#' || line[0] == '\n') continue;

        // 아이템 목록이 가득 찼다면 확장
        if (itemListCount >= itemListCapacity) {
            expandItemList();
        }

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
        int* validIndices = (int*)malloc(itemListCount * sizeof(int));
        if (validIndices == NULL) {
            printf("메모리 할당 실패\n");
            return;
        }
        
        for (int j = 0; j < itemListCount; j++) {
            if (itemList[j].grade == targetGrade) {
                validIndices[validItems++] = j;
            }
        }

        if (validItems > 0) {
            int selectedIndex = validIndices[rand() % validItems];
            outItems[i] = itemList[selectedIndex];
        }
        
        free(validIndices);
    }
}

// 인벤토리에 아이템 추가
void addItemToInventory(const Item* item) {
    // 회복형, 양갱형 아이템은 즉시 사용
    if (item->type == ITEM_HEAL || item->type == ITEM_YANGGAENG) {
        currentItem = item;  // 현재 사용할 아이템 설정
        if (item->type == ITEM_YANGGAENG && item->grade == ITEM_RARE) {
            // 이상한 양갱은 요괴 선택 없이 바로 사용
            int result = healYokai(NULL);  // NULL을 전달하여 모든 요괴 처리
            currentItem = NULL;  // 아이템 사용 후 초기화
            if (!result) {
                printText("\n다시 아이템을 선택하세요.\n");
                itemRewardSystem();
                return;
            }
            // 아이템 사용 성공 시 다음 스테이지로 진행
            return;
        } else {
            Yokai* targetYokai = selectYokaiToHeal();
            if (targetYokai != NULL) {
                if (item->type == ITEM_HEAL) {
                    // 곶감 아이템 처리
                    if (strcmp(item->name, "곶감") == 0) {
                        if (targetYokai->status == YOKAI_FAINTED) {
                            printText("\n기절한 요괴에게는 사용할 수 없습니다.\n");
                            currentItem = NULL;
                            itemRewardSystem();
                            return;
                        }
                        targetYokai->currentHP = calculateHP(targetYokai);
                        char msg[64];
                        snprintf(msg, sizeof(msg), "\n%s의 체력이 완전히 회복되었습니다!\n", targetYokai->name);
                        printTextAndWait(msg);
                        fastSleep(500);
                        // 아이템 사용 성공 시 다음 스테이지로 진행
                        currentItem = NULL;
                        return;
                    }
                    // 식혜류 아이템 처리
                    else if (strcmp(item->name, "미지근한 식혜") == 0 || strcmp(item->name, "시원한 식혜") == 0 || strcmp(item->name, "맛있는 식혜") == 0) {
                        if (targetYokai->status == YOKAI_FAINTED) {
                            printText("\n기절한 요괴에게는 사용할 수 없습니다.\n");
                            currentItem = NULL;
                            itemRewardSystem();
                            return;
                        }
                        useSikhyeItem(item->name, targetYokai);
                        // 아이템 사용 성공 시 다음 스테이지로 진행
                        currentItem = NULL;
                        return;
                    }
                    // 탕국과 막걸리 처리
                    else if (strcmp(item->name, "탕국") == 0 || strcmp(item->name, "막걸리") == 0) {
                        if (targetYokai->status != YOKAI_FAINTED) {
                            printText("\n기절하지 않은 요괴에게는 사용할 수 없습니다.\n");
                            fastSleep(500);
                            currentItem = NULL;
                            itemRewardSystem();
                            return;
                        }
                        int healResult = healYokai(targetYokai);  // HP 회복 처리
                        if (healResult == -1) {
                            currentItem = NULL;
                            printText("\n다시 아이템을 선택하세요.\n");
                            itemRewardSystem();  // 아이템 선택 메뉴로 돌아가기
                            return;
                        }
                        targetYokai->status = YOKAI_NORMAL; // 기절 상태 해제
                        printText("\n요괴의 기절 상태가 해제되었습니다!\n");
                        // 아이템 사용 성공 시 다음 스테이지로 진행
                        currentItem = NULL;
                        return;
                    } else {
                        if (targetYokai->status == YOKAI_FAINTED) {
                            printText("\n기절한 요괴에게는 사용할 수 없습니다.\n");
                            currentItem = NULL;
                            itemRewardSystem();
                            return;
                        }
                        int healResult = healYokai(targetYokai);  // HP 회복 처리
                        if (healResult == -1) {
                            currentItem = NULL;
                            printText("\n다시 아이템을 선택하세요.\n");
                            itemRewardSystem();  // 아이템 선택 메뉴로 돌아가기
                            return;
                        }
                        // 아이템 사용 성공 시 다음 스테이지로 진행
                        currentItem = NULL;
                        return;
                    }
                }
            } else {
                // 뒤로가기 선택 시 아이템 목록으로 복귀
                itemRewardSystem();
                return;
            }
        }
        currentItem = NULL;  // 아이템 사용 후 초기화
        return;
    }

    // 지도 아이템은 최대 1개만 보유 가능
    if (strcmp(item->name, "지도") == 0) {
        for (int i = 0; i < inventoryCount; i++) {
            if (strcmp(inventory[i].item.name, "지도") == 0) {
                printTextAndWait("\n지도 아이템은 최대 1개까지만 보유할 수 있습니다!");
                return;
            }
        }
    }

    // 이미 있는 아이템인지 확인
    for (int i = 0; i < inventoryCount; i++) {
        if (strcmp(inventory[i].item.name, item->name) == 0) {
            // 작두는 최대 5개까지만 보유 가능
            if (strcmp(item->name, "작두") == 0 && inventory[i].count >= 5) {
                printTextAndWait("\n작두는 최대 5개까지만 보유할 수 있습니다!");
                printText("\n다시 아이템을 선택하세요.\n");
                itemRewardSystem();
                return;
            }
            // 무당방울은 최대 5개까지만 보유 가능
            if (strcmp(item->name, "무당방울") == 0 && inventory[i].count >= 5) {
                printTextAndWait("\n무당방울은 최대 5개까지만 보유할 수 있습니다!");
                printText("\n다시 아이템을 선택하세요.\n");
                itemRewardSystem();
                return;
            }
            // 그 외 아이템은 최대 99개까지만 보유 가능
            if (strcmp(item->name, "작두") != 0 && strcmp(item->name, "무당방울") != 0 && inventory[i].count >= 99) {
                printTextAndWait("\n이 아이템은 최대 99개까지만 보유할 수 있습니다!");
                printText("\n다시 아이템을 선택하세요.\n");
                itemRewardSystem();
                return;
            }
            inventory[i].count++;
            return;
        }
    }

    // 새로운 아이템 추가
    if (inventoryCount < INVENTORY_MAX) {
        // 작두는 최대 5개까지만 보유 가능
        if (strcmp(item->name, "작두") == 0) {
            if (1 > 5) {
                printTextAndWait("\n작두는 최대 5개까지만 보유할 수 있습니다!");
                printText("\n다시 아이템을 선택하세요.\n");
                itemRewardSystem();
                return;
            }
        }
        // 무당방울은 최대 5개까지만 보유 가능
        if (strcmp(item->name, "무당방울") == 0) {
            if (1 > 5) {
                printTextAndWait("\n무당방울은 최대 5개까지만 보유할 수 있습니다!");
                printText("\n다시 아이템을 선택하세요.\n");
                itemRewardSystem();
                return;
            }
        }
        // 그 외 아이템은 최대 99개까지만 보유 가능
        if (strcmp(item->name, "작두") != 0 && strcmp(item->name, "무당방울") != 0) {
            if (1 > 99) {
                printTextAndWait("\n이 아이템은 최대 99개까지만 보유할 수 있습니다!");
                printText("\n다시 아이템을 선택하세요.\n");
                itemRewardSystem();
                return;
            }
        }
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
        // 등급에 따른 색상 설정
        const char* colorCode;
        switch (inventory[i].item.grade) {
            case ITEM_COMMON:
                colorCode = "\033[0m";  // 기본색 (흰색)
                break;
            case ITEM_RARE:
                colorCode = "\033[33m";  // 노란색
                break;
            case ITEM_SUPERRARE:
                colorCode = "\033[31m";  // 빨간색
                break;
            default:
                colorCode = "\033[0m";
        }
        
        sprintf(buffer, "%d. %s%s [%s]\033[0m x%d\n    %s\n", 
            i+1, 
            colorCode,
            inventory[i].item.name,
            inventory[i].item.grade == ITEM_COMMON ? "일반" : 
            inventory[i].item.grade == ITEM_RARE ? "희귀" : "초희귀",
            inventory[i].count,
            inventory[i].item.desc);
        printText(buffer);
    }
    printText("\n선택하세요: ");
}

// 부적 사용 함수
bool useTalisman(const Item* item, Yokai* targetYokai) {
    if (item->type != ITEM_TALISMAN) {
        printf("이 아이템은 부적이 아닙니다!\n");
        return false;
    }

    // 현재 HP 비율 계산
    float maxHP = targetYokai->stamina * (1.0f + (targetYokai->level * targetYokai->level) / 100.0f);
    float hpRatio = targetYokai->currentHP / maxHP;
    
    // HP 비율에 따른 포획률 보정 (HP가 낮을수록 포획률 증가)
    float hpBonus = 1.0f - hpRatio;  // HP가 0%일 때 1.0, 100%일 때 0.0
    
    // 부적 등급에 따른 기본 포획률
    float baseCatchRate = 0.0f;
    switch (item->grade) {
        case ITEM_COMMON:
            baseCatchRate = 0.15f;
            break;
        case ITEM_RARE:
            baseCatchRate = 0.3;
            break;
        case ITEM_SUPERRARE:
            baseCatchRate = 1;  // 100%
            break;
        default:
            baseCatchRate = 0.15;
    }
    
    // 최종 포획률 계산 (기본 포획률 + HP 보너스)
    float finalCatchRate = baseCatchRate + (hpBonus * 0.4f);  // HP 보너스는 최대 40%까지
    
    // 포획 시도
    if ((float)rand() / RAND_MAX < finalCatchRate) {
        printf("포획 성공!\n");
        return true;
    } else {
        printf("포획 실패...\n");
        return false;
    }
}

// 무당방울 보너스 계산 함수
int calculateMudangBonus(int amount, int mudangCount) {
    if (mudangCount > 5) mudangCount = 5;
    return (amount * 10 * mudangCount) / 100;
}

// 양갱 사용 함수
bool useYanggaeng(const Item* item, Yokai* targetYokai) {
    if (item->type != ITEM_YANGGAENG) {
        printf("이 아이템은 양갱이 아닙니다!\n");
        return false;
    }

    // 양갱 등급에 따른 레벨 상승량 결정
    int levelUp = 0;
    switch (item->grade) {
        case ITEM_COMMON:
            levelUp = 1;  // 일반 양갱: 1레벨
            break;
        case ITEM_RARE:
            // 희귀 양갱: 모든 동료 요괴 1레벨
            printTextAndWait("\n 양갱을 사용합니다...\n");
            
            for (int i = 0; i < gPartyCount; i++) {
                float oldMaxHP = calculateHP(&gParty[i]);  // 이전 최대 HP 저장
                int oldLevel = gParty[i].level;
                gParty[i].level++;  // 레벨 증가
                float newMaxHP = calculateHP(&gParty[i]);  // 새로운 최대 HP 계산
                float hpIncrease = newMaxHP - oldMaxHP;  // HP 증가량 계산
                
                // 기절 상태가 아닐 때만 현재 HP 증가
                if (gParty[i].status != YOKAI_FAINTED) {
                    gParty[i].currentHP += hpIncrease;  // 현재 HP에 증가량만큼 더하기
                }
                
                // 레벨업 메시지 출력
                char buffer[256];
                sprintf(buffer, "\n%s의 레벨이 %d에서 %d로 상승했습니다!\n", 
                    gParty[i].name, oldLevel, gParty[i].level);
                printTextAndWait(buffer);
            }
            printTextAndWait("\n모든 동료 요괴의 레벨이 상승했습니다!\n");
            fastSleep(500); 
            return true;
        case ITEM_SUPERRARE:
            levelUp = 3;  // 고급 양갱: 3레벨
            break;
        default:
            return false;
    }

    // 일반 양갱과 고급 양갱의 경우
    if (levelUp > 0) {
        float oldMaxHP = calculateHP(targetYokai);  // 이전 최대 HP 저장
        int oldLevel = targetYokai->level;
        targetYokai->level += levelUp;  // 레벨 증가
        float newMaxHP = calculateHP(targetYokai);  // 새로운 최대 HP 계산
        float hpIncrease = newMaxHP - oldMaxHP;  // HP 증가량 계산
        
        // 기절 상태가 아닐 때만 현재 HP 증가
        if (targetYokai->status != YOKAI_FAINTED) {
            targetYokai->currentHP += hpIncrease;  // 현재 HP에 증가량만큼 더하기
        }
        
        // 레벨업 메시지 출력
        char buffer[256];
        sprintf(buffer, "\n%s의 레벨이 %d에서 %d로 상승했습니다!\n", 
            targetYokai->name, oldLevel, targetYokai->level);
        printTextAndWait(buffer);
        return true;
    }

    return false;
}

// 작두 아이템 개수 반환 함수
int getJakduCount() {
    int count = 0;
    for (int i = 0; i < inventoryCount; i++) {
        if (strcmp(inventory[i].item.name, "작두") == 0) {
            count = inventory[i].count;
            break;
        }
    }
    return count;
} 