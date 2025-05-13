#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "battle.h"
#include "text.h"
#include "input.h"
#include "stage.h"
#include "party.h"
#include "savefile.h"
#include "menu.h"
#include "game.h"
#include "item.h"

void startBattle() {
    Yokai enemy;
    if (currentStage.stageNumber % 10 == 0) {
        enemy = createRandomBossYokai();
    } else {
        enemy = createRandomYokai();
    }
    char buffer[256];
    sprintf(buffer, "\n%s(이)가 싸움을 걸어왔다!\n", enemy.name);
    printText(buffer);
    
    while (1) {
        int done = showBattleMenu(&enemy);
        if (done) break;
    }
}

int showBattleMenu(const Yokai* enemy) {
    int choice;
    
    printText("\n무엇을 하시겠습니까?\n");
    printText("1. 싸운다\n");
    printText("2. 부적을 던진다\n");
    printText("3. 동료 요괴를 본다\n");
    printText("4. 도망간다\n");
    printText("5. 게임을 저장하고 메뉴로 돌아간다\n\n");
    printText("숫자를 입력하세요: ");
    
    choice = getIntInput();
    if (choice >= 1 && choice <= 5) {
        return handleBattleChoice((BattleChoice)choice, enemy);
    } else {
        printTextAndWait("\n잘못된 선택입니다. 1-5 사이의 숫자를 입력해주세요.");
        return 0;
    }
}

// 동료 요괴 선택 함수
int selectPartyYokai() {
    printText("\n동료 요괴를 선택하세요:\n");
    for (int i = 0; i < partyCount; i++) {
        char buffer[128];
        sprintf(buffer, "%d. %s (체력: %d, 공격력: %d, 방어력: %d)\n", i+1, party[i].name, party[i].hp, party[i].attack, party[i].defense);
        printText(buffer);
    }
    printText("선택 (번호): ");
    int idx = getIntInput() - 1;
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
        sprintf(buffer, "%d. %s (공격력: %d, 명중률: %d%%)\n", i+1, yokai->moves[i].name, yokai->moves[i].power, yokai->moves[i].accuracy);
        printText(buffer);
    }
    printText("선택 (번호): ");
    int idx = getIntInput() - 1;
    if (idx < 0 || idx >= yokai->moveCount) {
        printTextAndWait("\n잘못된 선택입니다. 다시 시도하세요.");
        return selectMove(yokai);
    }
    return idx;
}

// 아이템 선택 함수
void itemRewardSystem() {
    Item candidates[3];
    getRandomItems(candidates, 3);
    printText("\n전투 보상! 아이템을 하나 선택하세요:\n");
    for (int i = 0; i < 3; i++) {
        char buffer[256];
        sprintf(buffer, "%d. %s [%s] - %s\n", i+1, candidates[i].name,
            candidates[i].grade == ITEM_COMMON ? "일반" : candidates[i].grade == ITEM_RARE ? "희귀" : "초희귀",
            candidates[i].desc);
        printText(buffer);
    }
    printText("선택 (번호): ");
    int idx = getIntInput() - 1;
    if (idx < 0 || idx >= 3) {
        printTextAndWait("\n잘못된 선택입니다. 다시 시도하세요.");
        itemRewardSystem();
        return;
    }
    addItemToInventory(&candidates[idx]);
    char buffer[128];
    sprintf(buffer, "\n%s를 인벤토리에 획득했습니다!", candidates[idx].name);
    printTextAndWait(buffer);
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
    for (int i = 0; i < talismanCount; i++) {
        char buffer[128];
        sprintf(buffer, "%d. %s (보유: %d개)\n", i+1, inventory[talismanIdx[i]].item.name, inventory[talismanIdx[i]].count);
        printText(buffer);
    }
    printText("0. 뒤로 돌아간다\n");
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

int handleBattleChoice(BattleChoice choice, const Yokai* enemy) {
    (void)enemy;  // 사용하지 않는 매개변수 명시
    switch (choice) {
        case BATTLE_FIGHT: {
            int yokaiIdx = selectPartyYokai();
            int moveIdx = selectMove(&party[yokaiIdx]);
            char buffer[256];
            sprintf(buffer, "\n%s가 %s 기술을 사용했다! (전투 로직은 추후 구현)\n", party[yokaiIdx].name, party[yokaiIdx].moves[moveIdx].name);
            printTextAndWait(buffer);
            itemRewardSystem();
            return 1;
        }
        case BATTLE_TALISMAN: {
            int idx = selectTalismanFromInventory();
            if (idx == -1) {
                return 0; // 부적 없음: 메뉴 반복
            }
            char buffer[128];
            // 실제 부적 효과 적용
            if (useTalisman(&inventory[idx].item, (Yokai*)enemy)) {
                sprintf(buffer, "\n%s를 던졌다! 요괴를 잡았다!", inventory[idx].item.name);
            printTextAndWait(buffer);
                // 인벤토리에서 부적 차감
            if (inventory[idx].count == 1) {
                for (int i = idx; i < inventoryCount - 1; i++)
                    inventory[i] = inventory[i + 1];
                inventoryCount--;
            } else {
                inventory[idx].count--;
            }
            itemRewardSystem();
                return 1; // 전투 종료(잡았으니)
            } else {
                sprintf(buffer, "\n%s를 던졌다! 하지만 요괴를 잡지 못했다...", inventory[idx].item.name);
                printTextAndWait(buffer);
                // 인벤토리에서 부적 차감
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
            if (rand() % 2 == 0) {
                printTextAndWait("\n도망치는데 성공했습니다!");
                return 1;
            } else {
                printTextAndWait("\n도망치는데 실패했습니다!");
                return 0;
            }
        case BATTLE_SAVE_AND_EXIT:
            saveGame();
            printTextAndWait("\n게임이 저장되었습니다. 메뉴로 돌아갑니다.");
            gameState.isRunning = 0;
            return 1;
    }
    return 0;
} 