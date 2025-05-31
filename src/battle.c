#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "battle.h"
#include "yokai.h"  // Yokai 구조체 정의를 위해 추가
#include "text.h"
#include "input.h"
#include "stage.h"
#include "party.h"
#include "savefile.h"
#include "menu.h"
#include "game.h"
#include "item.h"
#include "move.h"
#include "reward.h"  // 새로 추가
#include "escape.h"  // 도망치기 시스템 추가

// 현재 전투 중인 상대 요괴
Yokai currentEnemy;

int startBattle(const Yokai* enemy) {
    // 현재 전투 중인 상대 요괴 정보 저장
    currentEnemy = *enemy;
    
    // 등장 메시지 출력 삭제
    while (1) {
        int done = showBattleMenu(enemy);
        if (done == 101 || done == 102) {
            int reward = calculateBattleReward();
            addMoney(reward);
            itemRewardSystem(); // 아이템 보상 창 호출
            return done;  // 전투 결과 반환
        } else if (done == 103) {
            // 도망 성공: 보상 없음
            return done;
        } else if (done == 2) {
            // 저장 후 종료
            return done;
        }
    }
}

int showBattleMenu(const Yokai* enemy) {
    int choice;
    
    printText("\n무엇을 하시겠습니까?\n");
    printText("1. 싸운다\n");
    printText("2. 부적을 던진다\n");
    printText("3. 동료 요괴를 본다\n");
    printText("4. 도망간다\n");
    printText("5. 인벤토리를 본다\n");
    printText("6. 게임을 저장하고 메뉴로 돌아간다\n\n");
    printText("숫자를 입력하세요: ");
    
    choice = getIntInput();
    if (choice >= 1 && choice <= 6) {
        return handleBattleChoice((BattleChoice)choice, (Yokai*)enemy);
    } else {
        printTextAndWait("\n잘못된 선택입니다. 1-6 사이의 숫자를 입력해주세요.");
        return 0;
    }
}

// 동료 요괴 선택 함수
int selectPartyYokai() {
    printText("\n동료 요괴를 선택하세요:\n");
    printText("0. 뒤로 가기\n");
    for (int i = 0; i < partyCount; i++) {
        char buffer[128];
        sprintf(buffer, "%d. %s (체력: %d, 공격력: %d, 방어력: %d, 스피드: %d)\n", i+1, party[i].name, party[i].hp, party[i].attack, party[i].defense, party[i].speed);
        printText(buffer);
    }
    printText("선택 (번호): ");
    int idx = getIntInput() - 1;
    if (idx == -1) {
        return -1; // 뒤로 가기
    }
    if (idx < 0 || idx >= partyCount) {
        printTextAndWait("\n잘못된 선택입니다. 다시 시도하세요.");
        return selectPartyYokai();
    }
    return idx;
}

// 기술 선택 함수
int selectMove(const Yokai* yokai) {
    printText("\n사용할 기술을 선택하세요:\n");
    for (int i = 0; i < yokai->moveCount; i++) {
        char buffer[128];
        sprintf(buffer, "%d. %s (공격력: %d, 명중률: %d%%, PP: %d/%d)\n", i+1, yokai->moves[i].move.name, yokai->moves[i].move.power, yokai->moves[i].move.accuracy, yokai->moves[i].currentPP, yokai->moves[i].move.pp);
        printText(buffer);
    }
    printText("선택 (번호): ");
    int idx = getIntInput() - 1;
    if (idx < 0 || idx >= yokai->moveCount) {
        printTextAndWait("\n잘못된 선택입니다. 다시 시도하세요.");
        return selectMove(yokai);
    }
    if (yokai->moves[idx].currentPP <= 0) {
        printTextAndWait("\n해당 기술의 PP가 부족합니다. 다른 기술을 선택하세요.");
        return selectMove(yokai);
    }
    return idx;
}

// 부적 아이템 선택 함수
int selectTalismanFromInventory() {
    int talismanIdx[INVENTORY_MAX];
    int talismanCount = 0;
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i].item.type == ITEM_TALISMAN && inventory[i].count > 0) {
            talismanIdx[talismanCount++] = i;
        }
    }
    if (talismanCount == 0) {
        printTextAndWait("\n사용할 수 있는 부적이 없습니다!");
        return -1;
    }
    printText("\n사용할 부적을 선택하세요:\n");
    printText("0. 뒤로 가기\n");
    for (int i = 0; i < talismanCount; i++) {
        char buffer[128];
        sprintf(buffer, "%d. %s (보유: %d개)\n", i+1, inventory[talismanIdx[i]].item.name, inventory[talismanIdx[i]].count);
        printText(buffer);
    }
    printText("숫자를 입력하세요: ");
    int idx = getIntInput();
    if (idx == 0) return -1;
    idx--;
    if (idx < 0 || idx >= talismanCount) {
        printTextAndWait("\n잘못된 선택입니다. 다시 시도하세요.");
        return selectTalismanFromInventory();
    }
    return talismanIdx[idx];
}

int handleBattleChoice(BattleChoice choice, Yokai* enemy) {
    switch (choice) {
        case BATTLE_FIGHT: {
            int yokaiIdx = selectPartyYokai();
            if (yokaiIdx == -1) {
                return 0; // 뒤로 돌아가기
            }
            int moveIdx = selectMove(&party[yokaiIdx]);
            // PP 감소
            party[yokaiIdx].moves[moveIdx].currentPP--;
            char buffer[256];
            sprintf(buffer, "\n%s가 %s 기술을 사용했다! (전투 로직은 추후 구현)\n", party[yokaiIdx].name, party[yokaiIdx].moves[moveIdx].move.name);
            printTextAndWait(buffer);
            return 101; // BATTLE_FIGHT 성공
        }
        case BATTLE_TALISMAN: {
            if (currentStage.stageNumber % 10 == 0) {
                printTextAndWait("\n알 수 없는 힘이 부적을 던질 수 없게 합니다!");
                return 0;
            }
            int idx = selectTalismanFromInventory();
            if (idx == -1) {
                return 0; // 부적 없음: 메뉴 반복
            }
            char buffer[128];
            if (useTalisman(&inventory[idx].item, enemy)) {
                sprintf(buffer, "\n%s를 던졌다! 요괴를 잡았다!", inventory[idx].item.name);
                printTextAndWait(buffer);
                // 요괴를 파티에 추가
                Yokai* baseYokai = findYokaiByName(enemy->name);
                if (baseYokai && addYokaiToParty(baseYokai)) {
                    sprintf(buffer, "\n%s가 동료가 되었습니다!", baseYokai->name);
            printTextAndWait(buffer);
                }
            if (inventory[idx].count == 1) {
                for (int i = idx; i < inventoryCount - 1; i++)
                    inventory[i] = inventory[i + 1];
                inventoryCount--;
            } else {
                inventory[idx].count--;
            }
                return 102; // BATTLE_TALISMAN 성공
            } else {
                sprintf(buffer, "\n%s를 던졌다! 하지만 요괴를 잡지 못했다...", inventory[idx].item.name);
                printTextAndWait(buffer);
                if (inventory[idx].count == 1) {
                    for (int i = idx; i < inventoryCount - 1; i++)
                        inventory[i] = inventory[i + 1];
                    inventoryCount--;
                } else {
                    inventory[idx].count--;
                }
                return 0; // 전투 계속
            }
        }
        case BATTLE_CHECK_PARTY:
            printParty();
            return 0;
        case BATTLE_RUN:
            return tryToEscape();  // 도망치기 시스템 사용
        case BATTLE_CHECK_INVENTORY:
            printInventory();
            return 0;
        case BATTLE_SAVE_AND_EXIT:
            saveGame();
            printTextAndWait("\n게임이 저장되었습니다. 메뉴로 돌아갑니다.");
            gameState.isRunning = 0;
            return 2;
    }
    return 0;
} 