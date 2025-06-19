#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "yokai.h"
#include "item.h"
#include "text.h"
#include "input.h"
#include "game.h"
#include "heal_system.h"
#include "sikhye_system.h"
#include "reward.h"
#include "party.h"
#include "move_learning.h"
#include "../core/state.h"
#include "logger.h"

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
    if (strcmp(str, "FORGOTTEN_MOVE") == 0) return ITEM_FORGOTTEN_MOVE;
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
        if (randomValue < 60) targetGrade = ITEM_COMMON;      // 70%
        else if (randomValue < 5) targetGrade = ITEM_RARE;   // 25%
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
    // 회복형, 양갱형, 잊은 기술 배우기 아이템은 즉시 사용
    if (item->type == ITEM_HEAL || item->type == ITEM_YANGGAENG || item->type == ITEM_FORGOTTEN_MOVE) {
        currentItem = item;  // 현재 사용할 아이템 설정
        if (item->type == ITEM_YANGGAENG) {
            if (item->grade == ITEM_RARE) {
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
                // 평범한 양갱과 고급 양갱은 요괴 선택 필요
                Yokai* targetYokai = selectYokaiToHeal();
                if (targetYokai != NULL) {
                    int result = healYokai(targetYokai);
                    currentItem = NULL;  // 아이템 사용 후 초기화
                    if (!result) {
                        printText("\n다시 아이템을 선택하세요.\n");
                        itemRewardSystem();
                        return;
                    }
                    // 아이템 사용 성공 시 다음 스테이지로 진행
                    return;
                } else {
                    // 뒤로가기 선택 시 아이템 목록으로 복귀
                    currentItem = NULL;
                    itemRewardSystem();
                    return;
                }
            }
        } else if (item->type == ITEM_FORGOTTEN_MOVE) {
            // 잊은 기술 배우기 아이템 처리
            Yokai* targetYokai = selectYokaiToHeal();
            if (targetYokai != NULL) {
                int result = useForgottenMoveItem(item, targetYokai);
                currentItem = NULL;  // 아이템 사용 후 초기화
                if (!result) {
                    printText("\n다시 아이템을 선택하세요.\n");
                    itemRewardSystem();
                    return;
                }
                // 아이템 사용 성공 시 다음 스테이지로 진행
                return;
            } else {
                // 뒤로가기 선택 시 아이템 목록으로 복귀
                currentItem = NULL;
                itemRewardSystem();
                return;
            }
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
                        float maxHP = calculateHP(targetYokai);
                        if (targetYokai->currentHP >= maxHP) {
                            printText("\n이미 체력이 최대입니다!\n");
                            currentItem = NULL;
                            itemRewardSystem();
                            return;
                        }
                        targetYokai->currentHP = maxHP;
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

    // 요괴형 아이템은 요괴 인벤토리로
    if (item->type == ITEM_YOKAI) {
        printText("\n요괴형 아이템입니다. 아이템을 보관할 요괴를 선택하세요.\n");
        Yokai* targetYokai = selectYokaiToHeal();
        if (targetYokai == NULL) {
            printText("\n요괴 선택이 취소되었습니다. 아이템 보상창으로 돌아갑니다.\n");
            itemRewardSystem();
            return;
        }
        // 중복 아이템 확인
        int found = 0;
        for (int i = 0; i < targetYokai->yokaiInventoryCount; i++) {
            if (strcmp(targetYokai->yokaiInventory[i].item.name, item->name) == 0) {
                // 복숭아와 고대의 서적은 5개까지만 보유 가능
                if (strcmp(item->name, "복숭아") == 0 || strcmp(item->name, "고대의 서적") == 0) {
                    if (targetYokai->yokaiInventory[i].count >= 5) {
                        char msg[128];
                        snprintf(msg, sizeof(msg), "\n%s는 해당 요괴가 최대 5개까지만 보유할 수 있습니다!\n", item->name);
                        printTextAndWait(msg);
                        itemRewardSystem();
                        return;
                    }
                } else {
                    // 그 외 아이템은 최대 99개까지만 보유
                    if (targetYokai->yokaiInventory[i].count >= 99) {
                        printTextAndWait("\n이 아이템은 해당 요괴가 최대 99개까지만 보유할 수 있습니다!\n");
                        itemRewardSystem();
                        return;
                    }
                }
                targetYokai->yokaiInventory[i].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (targetYokai->yokaiInventoryCount >= INVENTORY_MAX) {
                printTextAndWait("\n해당 요괴의 인벤토리가 가득 찼습니다!\n");
                itemRewardSystem();
                return;
            }
            targetYokai->yokaiInventory[targetYokai->yokaiInventoryCount].item = *item;
            targetYokai->yokaiInventory[targetYokai->yokaiInventoryCount].count = 1;
            targetYokai->yokaiInventoryCount++;
        }
        fastSleep(500);
        // 보상 선택이 완료되면 함수 종료
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
bool useTalisman(const Item* item, void* targetYokai) {
    Yokai* yokai = (Yokai*)targetYokai;
    if (item->type != ITEM_TALISMAN) {
        printf("이 아이템은 부적이 아닙니다!\n");
        return false;
    }

    // 현재 HP 비율 계산
    float maxHP = yokai->stamina * (1.0f + (yokai->level * yokai->level) / 100.0f);
    float hpRatio = yokai->currentHP / maxHP;
    
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
bool useYanggaeng(const Item* item, void* targetYokai) {
    Yokai* yokai = (Yokai*)targetYokai;
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
        float oldMaxHP = calculateHP(yokai);  // 이전 최대 HP 저장
        int oldLevel = yokai->level;
        yokai->level += levelUp;  // 레벨 증가
        float newMaxHP = calculateHP(yokai);  // 새로운 최대 HP 계산
        float hpIncrease = newMaxHP - oldMaxHP;  // HP 증가량 계산
        
        // 기절 상태가 아닐 때만 현재 HP 증가
        if (yokai->status != YOKAI_FAINTED) {
            yokai->currentHP += hpIncrease;  // 현재 HP에 증가량만큼 더하기
        }
        
        // 레벨업 메시지 출력
        char buffer[256];
        sprintf(buffer, "\n%s의 레벨이 %d에서 %d로 상승했습니다!\n", 
            yokai->name, oldLevel, yokai->level);
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

// 잊은 기술 배우기 함수
bool useForgottenMoveItem(const Item* item, void* targetYokai) {
    Yokai* yokai = (Yokai*)targetYokai;
    if (item->type != ITEM_FORGOTTEN_MOVE) {
        printf("이 아이템은 잊은 기술 배우기 아이템이 아닙니다!\n");
        return false;
    }

    // 기절한 요괴에게는 사용할 수 없음
    if (yokai->status == YOKAI_FAINTED) {
        printText("\n기절한 요괴에게는 사용할 수 없습니다.\n");
        return false;
    }

    // 잊은 기술이 있는지 확인
    if (yokai->forgottenMoveCount == 0) {
        printText("\n이 요괴는 잊은 기술이 없습니다.\n");
        return false;
    }

    // 잊은 기술 목록 출력
    printText("\n=== 잊은 기술 목록 ===\n");
    for (int i = 0; i < yokai->forgottenMoveCount; i++) {
        char* typeColor = "";
        switch (yokai->forgottenMoves[i].type) {
            case TYPE_EVIL_SPIRIT: typeColor = "\033[31m"; break; // 빨간색
            case TYPE_GHOST: typeColor = "\033[35m"; break;      // 보라색
            case TYPE_MONSTER: typeColor = "\033[33m"; break;    // 노란색
            case TYPE_HUMAN: typeColor = "\033[36m"; break;      // 청록색
            case TYPE_ANIMAL: typeColor = "\033[32m"; break;     // 초록색
            default: typeColor = "\033[0m"; break;               // 기본색
        }
        char buffer[256];
        sprintf(buffer, "%d. %s%s\033[0m (공격력: %d, 명중률: %d%%, PP: %d)\n", 
            i + 1, 
            typeColor,
            yokai->forgottenMoves[i].name,
            yokai->forgottenMoves[i].power,
            yokai->forgottenMoves[i].accuracy,
            yokai->forgottenMoves[i].pp);
        printText(buffer);
    }
    printText("0. 뒤로 가기\n");
    printText("\n배울 기술을 선택하세요: ");

    int choice = getIntInput(0, yokai->forgottenMoveCount);
    if (choice == 0) {
        return false; // 뒤로 가기
    }

    // 선택된 기술
    Move selectedMove = yokai->forgottenMoves[choice - 1];

    // 기술 슬롯이 가득 찬 경우 기존 기술 중 하나를 잊어야 함
    if (yokai->moveCount >= MAX_MOVES) {
        printText("\n기술 슬롯이 가득 찼습니다. 어떤 기술을 잊으시겠습니까?\n");
        printAvailableMoves(yokai);
        
        int forgetChoice = getIntInput(0, yokai->moveCount);
        if (forgetChoice == 0) {
            return false; // 뒤로 가기
        }
        
        // 기존 기술을 잊고 새로운 기술 배우기
        char oldMoveName[YOKAI_NAME_MAX];
        strcpy(oldMoveName, yokai->moves[forgetChoice - 1].move.name);
        
        // 기존 기술을 잊은 기술 목록에서 제거하고 새로운 기술을 잊은 기술 목록에 추가
        for (int i = choice - 1; i < yokai->forgottenMoveCount - 1; i++) {
            yokai->forgottenMoves[i] = yokai->forgottenMoves[i + 1];
        }
        yokai->forgottenMoveCount--;
        
        // 기존 기술을 잊은 기술 목록에 추가
        yokai->forgottenMoves[yokai->forgottenMoveCount++] = yokai->moves[forgetChoice - 1].move;
        
        // 기존 기술 제거
        for (int i = forgetChoice - 1; i < yokai->moveCount - 1; i++) {
            yokai->moves[i] = yokai->moves[i + 1];
        }
        yokai->moveCount--;
        
        // 새로운 기술 배우기
        yokai->moves[yokai->moveCount].move = selectedMove;
        yokai->moves[yokai->moveCount].currentPP = selectedMove.pp;
        yokai->moveCount++;
        
        char buffer[256];
        sprintf(buffer, "\n%s를 잊고 %s를 배웠습니다!\n", oldMoveName, selectedMove.name);
        printText(buffer);
    } else {
        // 빈 슬롯이 있는 경우
        // 선택된 기술을 잊은 기술 목록에서 제거
        for (int i = choice - 1; i < yokai->forgottenMoveCount - 1; i++) {
            yokai->forgottenMoves[i] = yokai->forgottenMoves[i + 1];
        }
        yokai->forgottenMoveCount--;
        
        // 새로운 기술 배우기
        yokai->moves[yokai->moveCount].move = selectedMove;
        yokai->moves[yokai->moveCount].currentPP = selectedMove.pp;
        yokai->moveCount++;
        
        char buffer[256];
        sprintf(buffer, "\n%s를 배웠습니다!\n", selectedMove.name);
        printText(buffer);
    }

    return true;
}

bool useYokaiItem(const Item* item, void* targetYokai) {
    Yokai* yokai = (Yokai*)targetYokai;
    if (!yokai) return false;
    
    if (strcmp(item->name, "복숭아") == 0) {
        // 복숭아 아이템 처리
        for (int i = 0; i < yokai->yokaiInventoryCount; i++) {
            if (strcmp(yokai->yokaiInventory[i].item.name, "복숭아") == 0) {
                printTextAndWait("\n이미 복숭아를 가지고 있습니다!");
                return false;
            }
        }
        addItemToYokaiInventory(yokai, item);
        return true;
    }
    else if (strcmp(item->name, "고대의 서적") == 0) {
        // 고대의 서적 아이템 처리
        for (int i = 0; i < yokai->yokaiInventoryCount; i++) {
            if (strcmp(yokai->yokaiInventory[i].item.name, "고대의 서적") == 0) {
                printTextAndWait("\n이미 고대의 서적을 가지고 있습니다!");
                return false;
            }
        }
        addItemToYokaiInventory(yokai, item);
        return true;
    }
    else if (strcmp(item->name, "돋보기") == 0) {
        // 돋보기 개수 체크
        if (yokai->magnifierCount >= MAX_MAGNIFIER_COUNT) {
            char buffer[256];
            sprintf(buffer, "\n%s는 이미 최대 개수(%d개)의 돋보기를 가지고 있습니다!", yokai->name, MAX_MAGNIFIER_COUNT);
            printTextAndWait(buffer);
            return false;
        }
        // 먼저 돋보기 개수를 증가시킴
        yokai->magnifierCount++;
        
        // 그 다음 돋보기를 요괴의 인벤토리에 추가
        if (!addItemToYokaiInventory(yokai, item)) {
            // 인벤토리 추가 실패 시 돋보기 개수 원복
            yokai->magnifierCount--;
            return false;
        }
        char buffer[256];
        sprintf(buffer, "\n%s의 돋보기 개수가 %d개가 되었습니다! (명중률 +%d%%)", 
            yokai->name, yokai->magnifierCount, yokai->magnifierCount * 3);
        printTextAndWait(buffer);
        fastSleep(500);
        return true;
    }
    return false;
} 