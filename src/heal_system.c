#include <stdio.h>
#include "heal_system.h"
#include "text.h"
#include "input.h"
#include "party.h"
#include "hp_system.h"

// 현재 사용 중인 아이템
const Item* currentItem = NULL;

void initHealSystem() {
    // 향후 구현 예정
}

void cleanupHealSystem() {
    // 향후 구현 예정
}

void healYokai(Yokai* targetYokai) {
    if (targetYokai == NULL) {
        printTextAndWait("\n회복할 요괴가 선택되지 않았습니다.");
        return;
    }
    
    // 최대 HP 계산
    float maxHP = calculateHP(targetYokai);
    float oldHP = targetYokai->currentHP;
    float healAmount;
    char buffer[256];
    
    // 아이템 이름에 따른 회복량 설정
    if (strcmp(currentItem->name, "제삿밥") == 0) {
        healAmount = maxHP * 0.3f;  // 30% 회복
    } else if (strcmp(currentItem->name, "나물") == 0) {
        healAmount = maxHP * 0.5f;  // 50% 회복
    } else if (strcmp(currentItem->name, "탕국") == 0) {
        if (targetYokai->status == YOKAI_FAINTED) {
            targetYokai->status = YOKAI_NORMAL;  // 기절 상태 해제
            healAmount = maxHP * 0.5f;  // 50% 회복
            printTextAndWait("\n기절 상태가 해제되었습니다!");
        } else {
            healAmount = maxHP * 0.5f;  // 50% 회복
        }
    } else if (strcmp(currentItem->name, "막걸리") == 0) {
        if (targetYokai->status == YOKAI_FAINTED) {
            targetYokai->status = YOKAI_NORMAL;  // 기절 상태 해제
            healAmount = maxHP;  // 100% 회복
            printTextAndWait("\n기절 상태가 해제되었습니다!");
        } else {
            healAmount = maxHP;  // 100% 회복
        }
    } else {
        printTextAndWait("\n알 수 없는 회복 아이템입니다.");
        return;
    }
    
    // HP 회복 적용
    targetYokai->currentHP += healAmount;
    if (targetYokai->currentHP > maxHP) {
        targetYokai->currentHP = maxHP;
    }
    
    // 회복 메시지 출력
    sprintf(buffer, "\n%s의 HP가 %.0f에서 %.0f로 회복되었습니다!", 
        targetYokai->name, oldHP, targetYokai->currentHP);
    printTextAndWait(buffer);
}

Yokai* selectYokaiToHeal() {
    printText("\n=== 회복할 요괴 선택 ===\n");
    
    // 파티의 요괴 목록 표시
    for (int i = 0; i < partyCount; i++) {
        char buffer[256];
        float maxHP = calculateHP(&party[i]);
        sprintf(buffer, "%d. %s (HP: %.0f/%.0f)\n", 
            i + 1,
            party[i].name,
            party[i].currentHP,
            maxHP);
        printText(buffer);
    }
    
    // 사용자 입력 받기
    int choice = getNumberInput(1, partyCount);
    if (choice == -1) {
        return NULL;
    }
    
    return &party[choice - 1];
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
    
    // 사용자 입력 받기
    int choice = getNumberInput(1, targetYokai->moveCount);
    if (choice == -1) {
        return -1;
    }
    
    return choice - 1;
}

// 단일 기술 PP 회복 처리
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
}

// 모든 기술 PP 회복 처리
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
}

// 기술 PP 회복 처리
void healMovePP(Yokai* targetYokai, int ppAmount) {
    if (targetYokai == NULL) {
        printTextAndWait("\n회복할 요괴가 선택되지 않았습니다.");
        return;
    }
    
    int moveIndex = selectMoveToHeal(targetYokai);
    if (moveIndex != -1) {
        healSingleMovePP(targetYokai, moveIndex, ppAmount);
    }
} 