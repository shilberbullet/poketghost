#include <stdio.h>
#include <windows.h>
#include "heal_system.h"
#include "text.h"
#include "input.h"
#include "party.h"
#include "hp_system.h"
#include "../core/state.h"
#include "reward.h"

// 현재 사용 중인 아이템
const Item* currentItem = NULL;

// 회복 시스템 초기화 함수
void initHealSystem() {
    // 향후 구현 예정
}

// 회복 시스템 정리 함수
void cleanupHealSystem() {
    // 향후 구현 예정
}

// 요괴 HP 회복 함수
int healYokai(Yokai* targetYokai) {
    if (!currentItem) return -1;
    
    switch (currentItem->type) {
        case ITEM_HEAL:
            if (!targetYokai) return -1;
            
            // 탕국과 막걸리는 기절한 요괴만 회복 가능
            if (strcmp(currentItem->name, "탕국") == 0 || strcmp(currentItem->name, "막걸리") == 0) {
                if (targetYokai->currentHP > 0) {
                    printTextAndWait("\n이 아이템은 기절한 요괴만 회복할 수 있습니다!");
                    return -1;
                }
            }
            // 그 외 회복 아이템은 기절한 요괴를 회복할 수 없음
            else if (targetYokai->currentHP <= 0) {
                printTextAndWait("\n기절한 요괴는 회복할 수 없습니다!");
                fastSleep(500);
                return -1;
            }

            // HP가 이미 최대인 경우
            float maxHP = calculateHP(targetYokai);
            if (targetYokai->currentHP >= maxHP) {
                printTextAndWait("\n이미 체력이 최대입니다!");
                return -1;
            }
            
            if (strcmp(currentItem->name, "제삿밥") == 0) {
                float healAmount = maxHP * 0.3f;
                float oldHP = targetYokai->currentHP;
                targetYokai->currentHP += healAmount;
                if (targetYokai->currentHP > maxHP) {
                    targetYokai->currentHP = maxHP;
                }
                float actualHeal = targetYokai->currentHP - oldHP;
                char buffer[256];
                sprintf(buffer, "\n%s의 체력이 %.0f 회복되었습니다!\n", targetYokai->name, actualHeal);
                printText(buffer);
                fastSleep(500);
                return 1;
            }
            else if (strcmp(currentItem->name, "나물") == 0) {
                float healAmount = maxHP * 0.5f;
                float oldHP = targetYokai->currentHP;
                targetYokai->currentHP += healAmount;
                if (targetYokai->currentHP > maxHP) {
                    targetYokai->currentHP = maxHP;
                }
                float actualHeal = targetYokai->currentHP - oldHP;
                char buffer[256];
                sprintf(buffer, "\n%s의 체력이 %.0f 회복되었습니다!\n", targetYokai->name, actualHeal);
                printText(buffer);
                fastSleep(500);
                return 1;
            }
            else if (strcmp(currentItem->name, "탕국") == 0) {
                // 탕국은 기절한 요괴만 회복 가능
                if (targetYokai->currentHP > 0) {
                    printTextAndWait("\n탕국은 기절한 요괴만 회복할 수 있습니다!");
                    return -1;
                }
                float healAmount = maxHP * 0.7f;
                float oldHP = targetYokai->currentHP;
                targetYokai->currentHP += healAmount;
                if (targetYokai->currentHP > maxHP) {
                    targetYokai->currentHP = maxHP;
                }
                float actualHeal = targetYokai->currentHP - oldHP;
                    char buffer[256];
                sprintf(buffer, "\n%s의 체력이 %.0f 회복되었습니다!\n", targetYokai->name, actualHeal);
                    printText(buffer);
                    fastSleep(500);
                    return 1;
            }
            else if (strcmp(currentItem->name, "막걸리") == 0) {
                // 막걸리는 기절한 요괴만 회복 가능
                if (targetYokai->currentHP > 0) {
                    printTextAndWait("\n막걸리는 기절한 요괴만 회복할 수 있습니다!");
                    return -1;
                }
                float healAmount = maxHP * 0.9f;
                float oldHP = targetYokai->currentHP;
                targetYokai->currentHP += healAmount;
                if (targetYokai->currentHP > maxHP) {
                    targetYokai->currentHP = maxHP;
                }
                float actualHeal = targetYokai->currentHP - oldHP;
                    char buffer[256];
                sprintf(buffer, "\n%s의 체력이 %.0f 회복되었습니다!\n", targetYokai->name, actualHeal);
                    printText(buffer);
                    fastSleep(500);
                    return 1;
            }
            break;
        case ITEM_YANGGAENG:
            if (strcmp(currentItem->name, "이상한 양갱") == 0) {
                printTextAndWait("\n이상한 양갱을 사용합니다...\n");
                
                // 모든 동료 요괴의 레벨을 1 증가
                for (int i = 0; i < gPartyCount; i++) {
                    float oldMaxHP = calculateHP(&gParty[i]);
                    int oldLevel = gParty[i].level;
                    gParty[i].level++;
                    float newMaxHP = calculateHP(&gParty[i]);
                    float hpIncrease = newMaxHP - oldMaxHP;
                    
                    // 기절 상태가 아닐 때만 현재 HP 증가
                    if (gParty[i].status != YOKAI_FAINTED) {
                        gParty[i].currentHP += hpIncrease;
                    }
                    
                    // 레벨업 메시지 출력
                    char buffer[256];
                    sprintf(buffer, "\n%s의 레벨이 %d에서 %d로 상승했습니다!\n", 
                        gParty[i].name, oldLevel, gParty[i].level);
                    printTextAndWait(buffer);
                }
                printTextAndWait("\n모든 동료 요괴의 레벨이 상승했습니다!\n");
                fastSleep(500);
                return 1;
            } else {
                if (!targetYokai) return -1;
                if (useYanggaeng(currentItem, targetYokai)) {
                    char buffer[256];
                    sprintf(buffer, "\n%s가 %s를 먹었습니다!\n", targetYokai->name, currentItem->name);
                    printText(buffer);
                    fastSleep(500);
                    return 1;
                }
            }
            break;
    }
    return -1;
}

// 회복할 요괴 선택 함수
Yokai* selectYokaiToHeal() {
    // 이상한 양갱인 경우 바로 NULL 반환
    if (currentItem && strcmp(currentItem->name, "이상한 양갱") == 0) {
        return NULL;
    }

    // 평범한 양갱이나 고급 양갱인 경우
    if (currentItem && (strcmp(currentItem->name, "평범한 양갱") == 0 || strcmp(currentItem->name, "고급 양갱") == 0)) {
        printText("\n양갱을 먹을 요괴를 선택하세요:\n");
    } else {
        printText("\n회복할 요괴를 선택하세요:\n");
    }

    for (int i = 0; i < gPartyCount; i++) {
        char buffer[256];
        float maxHP = calculateHP(&gParty[i]);
        
        // 기절 상태 표시
        const char* statusText = "";
        const char* statusColor = "";
        if (gParty[i].status == YOKAI_FAINTED) {
            statusText = " [기절]";
            statusColor = "\033[31m";  // 빨간색
        }
        
        sprintf(buffer, "%d. %s Lv.%d (HP: %.0f/%.0f)%s%s\033[0m\n", 
            i+1, 
            gParty[i].name, 
            gParty[i].level,
            gParty[i].currentHP,
            maxHP,
            statusColor,
            statusText);
        printTextAndWait(buffer);
    }
    printText("0. 뒤로 가기\n");
    printText("숫자를 입력해주세요: ");
    
    int choice = getIntInput();
    if (choice == 0) {
        return NULL;
    }
    
    if (choice > 0 && choice <= gPartyCount) {
        return &gParty[choice - 1];
    }
    
    printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
    return NULL;
}

// 기술 선택 함수
int selectMoveToHeal(Yokai* targetYokai) {
    printText("\n=== 회복할 기술 선택 ===\n");
    
    // 기술 목록 표시
    for (int i = 0; i < targetYokai->moveCount; i++) {
        char buffer[256];
        sprintf(buffer, "%d. %s (PP: %d/%d)\n", 
            i + 1,
            targetYokai->moves[i].move.name,
            targetYokai->moves[i].currentPP,
            targetYokai->moves[i].move.pp);
        printText(buffer);
    }
    printText("0. 뒤로 가기\n");
    printText("선택 (번호): ");
    
    // 사용자 입력 받기
    int choice = getIntInput();
    if (choice == 0) {
        return -1; // 뒤로 가기
    }
    if (choice < 1 || choice > targetYokai->moveCount) {
        printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
        return selectMoveToHeal(targetYokai);
    }
    return choice - 1;
}

// 단일 기술 PP 회복 처리 함수
void healSingleMovePP(Yokai* targetYokai, int moveIndex, int ppAmount) {
    if (targetYokai == NULL || moveIndex < 0 || moveIndex >= targetYokai->moveCount) {
        printTextAndWait("\n잘못된 기술 선택입니다.");
        return;
    }
    
    MoveInstance* move = &targetYokai->moves[moveIndex];
    int oldPP = move->currentPP;
    
    if (ppAmount == -1) {
        // -1은 최대 PP로 회복
        move->currentPP = move->move.pp;
    } else {
        move->currentPP += ppAmount;
        if (move->currentPP > move->move.pp) {
            move->currentPP = move->move.pp;
        }
    }
    
    char buffer[256];
    sprintf(buffer, "\n%s의 %s PP가 %d에서 %d로 회복되었습니다!", 
        targetYokai->name,
        move->move.name,
        oldPP,
        move->currentPP);
    printTextAndWait(buffer);
    fastSleep(500);
}

// 모든 기술 PP 회복 처리 함수
void healAllMovesPP(Yokai* targetYokai) {
    if (targetYokai == NULL) {
        printTextAndWait("\n회복할 요괴가 선택되지 않았습니다.");
        return;
    }
    
    for (int i = 0; i < targetYokai->moveCount; i++) {
        targetYokai->moves[i].currentPP = targetYokai->moves[i].move.pp;
    }
    
    char buffer[256];
    sprintf(buffer, "\n%s의 모든 기술 PP가 회복되었습니다!", targetYokai->name);
    printTextAndWait(buffer);
    fastSleep(500);
}

// 기술 PP 회복 처리 함수
void healMovePP(Yokai* targetYokai, int ppAmount) {
    if (targetYokai == NULL) {
        printTextAndWait("\n회복할 요괴가 선택되지 않았습니다.");
        return;
    }
    
    int moveIndex = selectMoveToHeal(targetYokai);
    if (moveIndex == -1) {
        // 기술 선택에서 뒤로가기를 선택한 경우: 요괴 선택창으로 복귀
        Yokai* newTarget = selectYokaiToHeal();
        if (newTarget != NULL) {
            healMovePP(newTarget, ppAmount);
        } else {
            // 요괴 선택창에서 뒤로가기를 선택한 경우: 아이템 보상창으로 복귀
            itemRewardSystem();
        }
        return;
    }
    if (moveIndex != -1) {
        healSingleMovePP(targetYokai, moveIndex, ppAmount);
    }
} 