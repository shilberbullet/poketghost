// 표준 입출력 함수를 위한 헤더
#include <stdio.h> // 표준 입출력 함수
// 표준 라이브러리 함수를 위한 헤더
#include <stdlib.h> // 표준 라이브러리 함수
// 문자열 처리 함수를 위한 헤더
#include <string.h> // 문자열 처리 함수
// 윈도우 API를 위한 헤더
#include <windows.h> // 윈도우 API
// 요괴 관련 함수와 구조체 정의
#include "yokai.h" // 요괴 관련 헤더
// 아이템 관련 함수와 구조체 정의
#include "item.h" // 아이템 관련 헤더
// 텍스트 출력 관련 함수
#include "text.h" // 텍스트 출력 헤더
// 입력 처리 관련 함수
#include "input.h" // 입력 처리 헤더
// 게임 관련 함수
#include "game.h" // 게임 헤더
// 회복 시스템 관련 함수
#include "heal_system.h" // 회복 시스템 헤더
// 식혜 시스템 관련 함수
#include "sikhye_system.h" // 식혜 시스템 헤더
// 보상 관련 함수
#include "reward.h" // 보상 헤더
// 파티 관리 관련 함수
#include "party.h" // 파티 관리 헤더
// 기술 학습 관련 함수
#include "move_learning.h" // 기술 학습 헤더
// 게임 상태 관련 함수
#include "../core/state.h" // 게임 상태 헤더
// 로거 관련 함수
#include "logger.h" // 로거 헤더
// 전투 관련 함수
#include "battle.h" // 전투 헤더
// 통계 관련 함수
#include "statistics.h" // 통계 헤더

// 상수 정의
#define INITIAL_ITEM_CAPACITY 32 // 초기 아이템 목록 용량
#define MAX_ITEMS 100 // 최대 아이템 종류

// 아이템 목록 (동적 배열)
Item* itemList = NULL; // 아이템 목록 포인터
int itemListCount = 0; // 아이템 목록 개수
int itemListCapacity = 0; // 아이템 목록 용량

// 인벤토리
InventoryItem inventory[INVENTORY_MAX]; // 인벤토리 배열
int inventoryCount = 0; // 인벤토리 아이템 개수

// 아이템 시스템 초기화
void initItemSystem() { // 아이템 시스템 초기화 함수 시작
    LOG_FUNCTION_EXECUTION("initItemSystem"); // 함수 실행 로그 기록
    itemList = (Item*)malloc(INITIAL_ITEM_CAPACITY * sizeof(Item)); // 아이템 목록 메모리 할당
    if (itemList == NULL) { // 메모리 할당 실패시
        printf("아이템 시스템 초기화 실패: 메모리 할당 오류\n"); // 오류 메시지 출력
        exit(1); // 프로그램 종료
    }
    itemListCapacity = INITIAL_ITEM_CAPACITY; // 초기 용량 설정
    itemListCount = 0; // 아이템 개수 초기화
}

// 아이템 시스템 정리
void cleanupItemSystem() { // 아이템 시스템 정리 함수 시작
    LOG_FUNCTION_EXECUTION("cleanupItemSystem"); // 함수 실행 로그 기록
    if (itemList != NULL) { // 아이템 목록이 존재하는 경우
        free(itemList); // 메모리 해제
        itemList = NULL; // 포인터 초기화
    }
    itemListCount = 0; // 아이템 개수 초기화
    itemListCapacity = 0; // 용량 초기화
}

// 아이템 목록 크기 확장
static void expandItemList() { // 아이템 목록 확장 함수 시작
    LOG_FUNCTION_EXECUTION("expandItemList"); // 함수 실행 로그 기록
    int newCapacity = itemListCapacity * 2; // 새로운 용량 계산 (2배)
    Item* newList = (Item*)realloc(itemList, newCapacity * sizeof(Item)); // 메모리 재할당
    if (newList == NULL) { // 재할당 실패시
        printf("아이템 목록 확장 실패: 메모리 할당 오류\n"); // 오류 메시지 출력
        return; // 함수 종료
    }
    itemList = newList; // 새로운 목록 포인터 설정
    itemListCapacity = newCapacity; // 새로운 용량 설정
}

// 문자열을 ItemGrade로 변환
ItemGrade stringToGrade(const char* str) { // 문자열을 등급으로 변환하는 함수 시작
    LOG_FUNCTION_EXECUTION("stringToGrade"); // 함수 실행 로그 기록
    if (strcmp(str, "COMMON") == 0) return ITEM_COMMON; // 일반 등급
    if (strcmp(str, "RARE") == 0) return ITEM_RARE; // 희귀 등급
    if (strcmp(str, "SUPERRARE") == 0) return ITEM_SUPERRARE; // 초희귀 등급
    return ITEM_COMMON; // 기본값
}

// 문자열을 ItemType으로 변환
ItemType stringToType(const char* str) { // 문자열을 타입으로 변환하는 함수 시작
    LOG_FUNCTION_EXECUTION("stringToType"); // 함수 실행 로그 기록
    if (strcmp(str, "HEAL") == 0) return ITEM_HEAL; // 회복 타입
    if (strcmp(str, "TALISMAN") == 0) return ITEM_TALISMAN; // 부적 타입
    if (strcmp(str, "PLAYER") == 0) return ITEM_PLAYER; // 플레이어 타입
    if (strcmp(str, "YOKAI") == 0) return ITEM_YOKAI; // 요괴 타입
    if (strcmp(str, "YANGGAENG") == 0) return ITEM_YANGGAENG; // 양갱 타입
    if (strcmp(str, "FORGOTTEN_MOVE") == 0) return ITEM_FORGOTTEN_MOVE; // 잊은 기술 타입
    return ITEM_HEAL; // 기본값
}

// 아이템 데이터 파일 로드
void loadItemsFromFile(const char* filename) { // 아이템 파일 로드 함수 시작
    LOG_FUNCTION_EXECUTION("loadItemsFromFile"); // 함수 실행 로그 기록
    FILE* file = fopen(filename, "r"); // 파일 열기
    if (!file) { // 파일 열기 실패시
        printf("아이템 데이터 파일을 열 수 없습니다: %s\n", filename); // 오류 메시지 출력
        return; // 함수 종료
    }

    char line[256]; // 라인 버퍼
    itemListCount = 0; // 아이템 개수 초기화

    while (fgets(line, sizeof(line), file)) { // 파일의 각 라인을 읽기
        // 주석이나 빈 줄 무시
        if (line[0] == '#' || line[0] == '\n') continue; // 주석이나 빈 라인 건너뛰기

        // 아이템 목록이 가득 찼다면 확장
        if (itemListCount >= itemListCapacity) { // 목록이 가득 찬 경우
            expandItemList(); // 목록 확장
        }

        char name[ITEM_NAME_MAX]; // 이름 버퍼
        char gradeStr[32]; // 등급 문자열 버퍼
        char typeStr[32]; // 타입 문자열 버퍼
        char desc[ITEM_DESC_MAX]; // 설명 버퍼

        // CSV 형식 파싱
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^\n]", 
            name, gradeStr, typeStr, desc) == 4) { // 4개 필드가 모두 파싱된 경우
            
            Item* item = &itemList[itemListCount]; // 현재 아이템 포인터
            strncpy(item->name, name, ITEM_NAME_MAX - 1); // 이름 복사
            strncpy(item->desc, desc, ITEM_DESC_MAX - 1); // 설명 복사
            item->grade = stringToGrade(gradeStr); // 등급 변환
            item->type = stringToType(typeStr); // 타입 변환
            
            itemListCount++; // 아이템 개수 증가
        }
    }

    fclose(file); // 파일 닫기
}

// 아이템 3개를 확률에 따라 랜덤 추출
void getRandomItems(Item* outItems, int count) { // 랜덤 아이템 추출 함수 시작
    LOG_FUNCTION_EXECUTION("getRandomItems"); // 함수 실행 로그 기록
    for (int i = 0; i < count; i++) { // 요청된 개수만큼 반복
        int randomValue = rand() % 100; // 0-99 랜덤 값 생성
        ItemGrade targetGrade; // 목표 등급
        
        // 등급별 확률 설정
        if (randomValue < 70) targetGrade = ITEM_COMMON;      // 70% 확률로 일반 등급
        else if (randomValue < 95) targetGrade = ITEM_RARE;   // 25% 확률로 희귀 등급
        else targetGrade = ITEM_SUPERRARE;             // 5% 확률로 초희귀 등급

        // 해당 등급의 아이템 중 랜덤 선택 (편지 제외)
        int validItems = 0; // 유효한 아이템 개수
        int* validIndices = (int*)malloc(itemListCount * sizeof(int)); // 유효한 인덱스 배열 할당
        if (validIndices == NULL) { // 메모리 할당 실패시
            printf("메모리 할당 실패\n"); // 오류 메시지 출력
            return; // 함수 종료
        }
        
        for (int j = 0; j < itemListCount; j++) { // 모든 아이템 검사
            // 편지 아이템은 제외
            if (strcmp(itemList[j].name, "편지") == 0) {
                continue; // 편지 아이템 건너뛰기
            }
            
            if (itemList[j].grade == targetGrade) { // 목표 등급과 일치하는 경우
                validIndices[validItems++] = j; // 유효한 인덱스 배열에 추가
            }
        }

        if (validItems > 0) { // 유효한 아이템이 있는 경우
            int selectedIndex = validIndices[rand() % validItems]; // 랜덤 인덱스 선택
            outItems[i] = itemList[selectedIndex]; // 선택된 아이템 복사
        }
        
        free(validIndices); // 메모리 해제
    }
}

// 인벤토리에 아이템 추가
void addItemToInventory(const Item* item) { // 인벤토리에 아이템 추가 함수 시작
    LOG_FUNCTION_EXECUTION("addItemToInventory"); // 함수 실행 로그 기록
    // 회복형, 양갱형, 잊은 기술 배우기 아이템은 즉시 사용
    if (item->type == ITEM_HEAL || item->type == ITEM_YANGGAENG || item->type == ITEM_FORGOTTEN_MOVE) { // 즉시 사용 아이템인 경우
        currentItem = item;  // 현재 사용할 아이템 설정
        if (item->type == ITEM_YANGGAENG) { // 양갱 타입인 경우
            if (item->grade == ITEM_RARE) { // 이상한 양갱인 경우
            // 이상한 양갱은 요괴 선택 없이 바로 사용
            int result = healYokai(NULL);  // NULL을 전달하여 모든 요괴 처리
            currentItem = NULL;  // 아이템 사용 후 초기화
            if (!result) { // 사용 실패시
                printText("\n다시 아이템을 선택하세요.\n"); // 안내 메시지 출력
                itemRewardSystem(); // 아이템 보상 시스템 재실행
                return; // 함수 종료
            }
            // 아이템 사용 성공 시 다음 스테이지로 진행
            return; // 함수 종료
            } else { // 평범한 양갱이나 고급 양갱인 경우
                // 평범한 양갱과 고급 양갱은 요괴 선택 필요
                Yokai* targetYokai = selectYokaiToHeal(); // 회복할 요괴 선택
                if (targetYokai != NULL) { // 요괴가 선택된 경우
                    int result = healYokai(targetYokai); // 요괴 회복
                    currentItem = NULL;  // 아이템 사용 후 초기화
                    if (!result) { // 사용 실패시
                        printText("\n다시 아이템을 선택하세요.\n"); // 안내 메시지 출력
                        itemRewardSystem(); // 아이템 보상 시스템 재실행
                        return; // 함수 종료
                    }
                    // 아이템 사용 성공 시 다음 스테이지로 진행
                    return; // 함수 종료
                } else { // 뒤로가기 선택한 경우
                    // 뒤로가기 선택 시 아이템 목록으로 복귀
                    currentItem = NULL; // 현재 아이템 초기화
                    itemRewardSystem(); // 아이템 보상 시스템 재실행
                    return; // 함수 종료
                }
            }
        } else if (item->type == ITEM_FORGOTTEN_MOVE) {
            // 잊은 기술 배우기 아이템 처리
            Yokai* targetYokai = selectYokaiToHeal(); // 기술을 배울 요괴 선택
            if (targetYokai != NULL) {
                int result = useForgottenMoveItem(item, targetYokai); // 기술 배우기 시도
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
            Yokai* targetYokai = selectYokaiToHeal(); // 회복할 요괴 선택
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
                        float maxHP = calculateHP(targetYokai); // 최대 HP 계산
                        if (targetYokai->currentHP >= maxHP) {
                            printText("\n이미 체력이 최대입니다!\n");
                            currentItem = NULL;
                            itemRewardSystem();
                            return;
                        }
                        targetYokai->currentHP = maxHP; // 체력 완전 회복
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
                        useSikhyeItem(item->name, targetYokai); // 식혜 아이템 사용
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

        while (1) {
            Yokai* targetYokai = selectYokaiToHeal(); // 보관할 요괴 선택
            if (targetYokai == NULL) {
                printText("\n요괴 선택이 취소되었습니다. 아이템 보상창으로 돌아갑니다.\n");
                itemRewardSystem();
                return;
            }

            int found = 0;
            int existing_item_index = -1;
            for (int i = 0; i < targetYokai->yokaiInventoryCount; i++) {
                if (strcmp(targetYokai->yokaiInventory[i].item.name, item->name) == 0) {
                    found = 1;
                    existing_item_index = i;
                    break;
                }
            }

            if (found) {
                if (strcmp(item->name, "복숭아") == 0) {
                    if (targetYokai->yokaiInventory[existing_item_index].count >= 5) {
                        printTextAndWait("\n이미 복숭아를 5개 가지고 있습니다!");
                        continue;
                    }
                } else if (strcmp(item->name, "고대의 서적") == 0) {
                    if (targetYokai->yokaiInventory[existing_item_index].count >= 5) {
                        char msg[128];
                        snprintf(msg, sizeof(msg), "\n%s는 해당 요괴가 최대 5개까지만 보유할 수 있습니다!\n", item->name);
                        printTextAndWait(msg);
                        continue;
                    }
                } else {
                    if (targetYokai->yokaiInventory[existing_item_index].count >= 99) {
                        printTextAndWait("\n이 아이템은 해당 요괴가 최대 99개까지만 보유할 수 있습니다!\n");
                        continue;
                    }
                }
                targetYokai->yokaiInventory[existing_item_index].count++;
            } else {
                if (targetYokai->yokaiInventoryCount >= INVENTORY_MAX) {
                    printTextAndWait("\n해당 요괴의 인벤토리가 가득 찼습니다!\n");
                    continue;
                }
                targetYokai->yokaiInventory[targetYokai->yokaiInventoryCount].item = *item;
                targetYokai->yokaiInventory[targetYokai->yokaiInventoryCount].count = 1;
                targetYokai->yokaiInventoryCount++;
            }

            char msg[128];
            snprintf(msg, sizeof(msg), "\n%s에게 %s를 보관했습니다.", targetYokai->name, item->name);
            printTextAndWait(msg);
            fastSleep(500);
            break; 
        }
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
            // 평범한 양갱 등 양갱류는 메시지 출력하지 않음
            if (item->type != ITEM_YANGGAENG) {
                char msg[128];
                snprintf(msg, sizeof(msg), "\n%s를 인벤토리에 획득했습니다!", item->name);
                printTextAndWait(msg);
                fastSleep(500);
            }
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
        // 평범한 양갱 등 양갱류는 메시지 출력하지 않음
        if (item->type != ITEM_YANGGAENG) {
            char msg[128];
            snprintf(msg, sizeof(msg), "\n%s를 인벤토리에 획득했습니다!", item->name);
            printTextAndWait(msg);
            fastSleep(500);
        }
        inventoryCount++;
        return;
    }
}

// 메시지 없이 인벤토리에 아이템 추가
void addItemToInventoryWithoutMessage(const Item* item) {
    LOG_FUNCTION_EXECUTION("addItemToInventoryWithoutMessage"); // 함수 실행 로그 기록
    // 회복형, 양갱형, 잊은 기술 배우기 아이템은 즉시 사용
    if (item->type == ITEM_HEAL || item->type == ITEM_YANGGAENG || item->type == ITEM_FORGOTTEN_MOVE) {
        // 이 함수에서는 즉시 사용 로직을 처리하지 않음
        return;
    }
    
    if (item->type == ITEM_YOKAI) {
        // 이 함수에서는 요괴 아이템을 처리하지 않음
        return;
    }

    // 지도 아이템은 최대 1개만 보유 가능
    if (strcmp(item->name, "지도") == 0) {
        for (int i = 0; i < inventoryCount; i++) {
            if (strcmp(inventory[i].item.name, "지도") == 0) {
                return;
            }
        }
    }

    // 이미 있는 아이템인지 확인
    for (int i = 0; i < inventoryCount; i++) {
        if (strcmp(inventory[i].item.name, item->name) == 0) {
            // 작두는 최대 5개까지만 보유 가능
            if (strcmp(item->name, "작두") == 0 && inventory[i].count >= 5) {
                return;
            }
            // 무당방울은 최대 5개까지만 보유 가능
            if (strcmp(item->name, "무당방울") == 0 && inventory[i].count >= 5) {
                return;
            }
            // 그 외 아이템은 최대 99개까지만 보유 가능
            if (strcmp(item->name, "작두") != 0 && strcmp(item->name, "무당방울") != 0 && inventory[i].count >= 99) {
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
                return;
            }
        }
        // 무당방울은 최대 5개까지만 보유 가능
        if (strcmp(item->name, "무당방울") == 0) {
            if (1 > 5) {
                return;
            }
        }
        // 그 외 아이템은 최대 99개까지만 보유 가능
        if (strcmp(item->name, "작두") != 0 && strcmp(item->name, "무당방울") != 0) {
            if (1 > 99) {
                return;
            }
        }
        inventory[inventoryCount].item = *item;
        inventory[inventoryCount].count = 1;
        inventoryCount++;
        return;
    }
}

// 인벤토리 출력 함수
void printInventory() {
    LOG_FUNCTION_EXECUTION("printInventory"); // 함수 실행 로그 기록
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
            inventory[i].item.desc); // 아이템 정보 출력
        printText(buffer);
    }
    printText("\n선택하세요: ");
}

// 부적 사용 함수
bool useTalisman(const Item* item, void* targetYokai) {
    LOG_FUNCTION_EXECUTION("useTalisman"); // 함수 실행 로그 기록
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
    float finalCatchRate = baseCatchRate + hpBonus;
    
    // 포획 시도
    if ((float)rand() / RAND_MAX < finalCatchRate) {
        printf("포획 성공!\n");
        gPlayer.stats.yokai_caught++; // 잡은 요괴 수 증가
        return true;
    } else {
        printf("포획 실패...\n");
        return false;
    }
}

// 무당방울 보너스 계산 함수
int calculateMudangBonus(int amount, int mudangCount) {
    LOG_FUNCTION_EXECUTION("calculateMudangBonus"); // 함수 실행 로그 기록
    if (mudangCount > 5) mudangCount = 5;
    return (amount * 10 * mudangCount) / 100;
}

// 양갱 사용 함수
bool useYanggaeng(const Item* item, void* targetYokai) {
    LOG_FUNCTION_EXECUTION("useYanggaeng"); // 함수 실행 로그 기록
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
    LOG_FUNCTION_EXECUTION("getJakduCount"); // 함수 실행 로그 기록
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
    LOG_FUNCTION_EXECUTION("useForgottenMoveItem"); // 함수 실행 로그 기록
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
            yokai->forgottenMoves[i].pp); // 잊은 기술 정보 출력
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
            yokai->forgottenMoves[i] = yokai->forgottenMoves[i + 1]; // 선택된 기술을 목록에서 제거하기 위해 뒤의 기술들을 앞으로 이동
        }
        yokai->forgottenMoveCount--; // 잊은 기술 개수 감소
        
        // 기존 기술을 잊은 기술 목록에 추가
        yokai->forgottenMoves[yokai->forgottenMoveCount++] = yokai->moves[forgetChoice - 1].move; // 잊을 기술을 잊은 기술 목록에 추가
        
        // 기존 기술 제거
        for (int i = forgetChoice - 1; i < yokai->moveCount - 1; i++) {
            yokai->moves[i] = yokai->moves[i + 1]; // 잊을 기술을 제거하기 위해 뒤의 기술들을 앞으로 이동
        }
        yokai->moveCount--; // 기술 개수 감소
        
        // 새로운 기술 배우기
        yokai->moves[yokai->moveCount].move = selectedMove; // 선택된 기술을 기술 슬롯에 추가
        yokai->moves[yokai->moveCount].currentPP = selectedMove.pp; // 현재 PP를 최대 PP로 설정
        yokai->moveCount++; // 기술 개수 증가
        
        char buffer[256];
        sprintf(buffer, "\n%s를 잊고 %s를 배웠습니다!\n", oldMoveName, selectedMove.name); // 기술 교체 메시지 생성
        printText(buffer); // 기술 교체 메시지 출력
    } else {
        // 빈 슬롯이 있는 경우
        // 선택된 기술을 잊은 기술 목록에서 제거
        for (int i = choice - 1; i < yokai->forgottenMoveCount - 1; i++) {
            yokai->forgottenMoves[i] = yokai->forgottenMoves[i + 1]; // 선택된 기술을 목록에서 제거하기 위해 뒤의 기술들을 앞으로 이동
        }
        yokai->forgottenMoveCount--; // 잊은 기술 개수 감소
        
        // 새로운 기술 배우기
        yokai->moves[yokai->moveCount].move = selectedMove; // 선택된 기술을 기술 슬롯에 추가
        yokai->moves[yokai->moveCount].currentPP = selectedMove.pp; // 현재 PP를 최대 PP로 설정
        yokai->moveCount++; // 기술 개수 증가
        
        char buffer[256];
        sprintf(buffer, "\n%s를 배웠습니다!\n", selectedMove.name); // 기술 학습 메시지 생성
        printText(buffer); // 기술 학습 메시지 출력
    }

    return true; // 기술 배우기 성공
}

// 요괴 아이템 사용 함수
int useYokaiItem(const Item* item, void* targetYokai) {
    LOG_FUNCTION_EXECUTION("useYokaiItem"); // 함수 실행 로그 기록
    Yokai* yokai = (Yokai*)targetYokai; // 타겟 요괴 포인터 캐스팅
    if (!yokai) return 0; // 요괴가 없으면 실패
    
    if (strcmp(item->name, "복숭아") == 0) { // 복숭아 아이템인 경우
        for (int i = 0; i < yokai->yokaiInventoryCount; i++) { // 요괴 인벤토리 검사
            if (strcmp(yokai->yokaiInventory[i].item.name, "복숭아") == 0) { // 이미 복숭아를 가지고 있는지 확인
                if (yokai->yokaiInventory[i].count >= 5) { // 최대 개수(5개) 확인
                    printTextAndWait("\n이미 복숭아를 5개 가지고 있습니다!"); // 최대 개수 초과 메시지
                    return -1; // 실패 반환
                }
                break; // 복숭아를 찾았으므로 루프 종료
            }
        }
        addItemToYokaiInventory(yokai, item); // 요괴 인벤토리에 복숭아 추가
        return 1; // 성공 반환
    }
    else if (strcmp(item->name, "고대의 서적") == 0) { // 고대의 서적 아이템인 경우
        for (int i = 0; i < yokai->yokaiInventoryCount; i++) { // 요괴 인벤토리 검사
            if (strcmp(yokai->yokaiInventory[i].item.name, "고대의 서적") == 0) { // 이미 고대의 서적을 가지고 있는지 확인
                if (yokai->yokaiInventory[i].count >= 5) { // 최대 개수(5개) 확인
                    printTextAndWait("\n이미 고대의 서적을 5개 가지고 있습니다!"); // 최대 개수 초과 메시지
                    return -1; // 실패 반환
                }
                break; // 고대의 서적을 찾았으므로 루프 종료
            }
        }
        addItemToYokaiInventory(yokai, item); // 요괴 인벤토리에 고대의 서적 추가
        return 1; // 성공 반환
    }
    else if (strcmp(item->name, "돋보기") == 0) { // 돋보기 아이템인 경우
        if (yokai->magnifierCount >= MAX_MAGNIFIER_COUNT) { // 최대 돋보기 개수 확인
            char buffer[256];
            sprintf(buffer, "\n%s는 이미 최대 개수(%d개)의 돋보기를 가지고 있습니다!", yokai->name, MAX_MAGNIFIER_COUNT); // 최대 개수 초과 메시지 생성
            printTextAndWait(buffer); // 최대 개수 초과 메시지 출력
            return -1; // 실패 반환
        }
        yokai->magnifierCount++; // 돋보기 개수 증가
        if (!addItemToYokaiInventory(yokai, item)) { // 요괴 인벤토리에 돋보기 추가 시도
            yokai->magnifierCount--; // 추가 실패 시 돋보기 개수 되돌리기
            return 0; // 실패 반환
        }
        char buffer[256];
        sprintf(buffer, "\n%s의 돋보기 개수가 %d개가 되었습니다! (명중률 +%d%%)", yokai->name, yokai->magnifierCount, yokai->magnifierCount * 3); // 돋보기 획득 메시지 생성
        printTextAndWait(buffer); // 돋보기 획득 메시지 출력
        fastSleep(500); // 0.5초 대기
        return 1; // 성공 반환
    }
    else if (item->type == ITEM_YANGGAENG) { // 양갱 아이템인 경우
        if (useYanggaeng(item, yokai)) { // 양갱 사용 함수 호출
            char buffer[256];
            sprintf(buffer, "\n%s가 %s를 먹었습니다!\n", yokai->name, item->name); // 양갱 사용 메시지 생성
            printTextAndWait(buffer); // 양갱 사용 메시지 출력
            fastSleep(500); // 0.5초 대기
            return 1; // 성공 반환
        } else {
            printTextAndWait("\n양갱 사용에 실패했습니다!\n"); // 양갱 사용 실패 메시지
            return 0; // 실패 반환
        }
    }
    return 0; // 알 수 없는 아이템이므로 실패 반환
}

// 찹살경단 자동 발동 함수
// HP가 20% 이하로 내려가면, 기절 상태가 아니면 30% 회복 후 1개 소모
bool tryActivateChapsalgyungdan(Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("tryActivateChapsalgyungdan"); // 함수 실행 로그 기록
    // 기절 상태면 발동 불가
    if (yokai->status == YOKAI_FAINTED) return false; // 기절 상태면 찹살경단 발동 불가
    
    // 찹살경단 보유 여부 확인
    int idx = -1; // 찹살경단 인덱스 초기화
    for (int i = 0; i < yokai->yokaiInventoryCount; i++) { // 요괴 인벤토리 검사
        if (strcmp(yokai->yokaiInventory[i].item.name, "찹살경단") == 0 && yokai->yokaiInventory[i].count > 0) { // 찹살경단이 있고 개수가 0보다 큰지 확인
            idx = i; // 찹살경단 인덱스 저장
            break; // 찹살경단을 찾았으므로 루프 종료
        }
    }
    if (idx == -1) return false; // 찹살경단이 없으면 발동 불가
    
    float maxHP = calculateHP(yokai); // 최대 HP 계산
    float hpPercentage = (yokai->currentHP / maxHP) * 100.0f; // 현재 HP 비율 계산
    if (hpPercentage >= 30.0f) return false; // HP가 30% 이상이면 발동 불가
    
    // 회복량 계산
    float healAmount = maxHP * 0.3f; // 최대 HP의 30%만큼 회복
    float beforeHP = yokai->currentHP; // 회복 전 HP 저장
    yokai->currentHP += healAmount; // 현재 HP에 회복량 추가
    if (yokai->currentHP > maxHP) yokai->currentHP = maxHP; // 최대 HP를 초과하지 않도록 제한
    
    // 아이템 1개 소모
    yokai->yokaiInventory[idx].count--; // 찹살경단 개수 감소
    if (yokai->yokaiInventory[idx].count <= 0) { // 찹살경단이 0개 이하가 되면
        removeItemFromYokaiInventory(yokai, idx); // 요괴 인벤토리에서 찹살경단 제거
    }
    
    // 메시지 출력
    char buffer[256];
    sprintf(buffer, "\n찹살경단의 힘으로 %s가 %.0f의 체력을 회복했습니다! (%.0f → %.0f)", yokai->name, yokai->currentHP - beforeHP, beforeHP, yokai->currentHP); // 회복 메시지 생성
    printTextAndWait(buffer); // 회복 메시지 출력
    fastSleep(500); // 0.5초 대기
    return true; // 찹살경단 발동 성공
} 