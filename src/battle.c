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
#include "hp_system.h"  // HP 시스템 헤더 추가
#include "exp_system.h"  // 경험치 시스템 헤더 추가
#include "battle_system.h"  // 새로 추가
#include "roguelite.h"  // 로그라이트 시스템 추가

// 현재 전투 중인 상대 요괴
Yokai currentEnemy;

// 전역 또는 static 변수로 턴 카운트 선언
int turnCount = 0;
int lastYokaiIdx = 0;  // 전역 변수로 선언

// 요괴가 기절했을 때의 처리 함수
int handleFaintedYokai(int faintedIdx) {
    char buffer[256];
    sprintf(buffer, "\n%s(이)가 쓰러졌다!\n", party[faintedIdx].name);
    printText(buffer);
    
    // 다른 기절하지 않은 요괴가 있는지 확인
    bool hasActiveYokai = false;
    for (int i = 0; i < partyCount; i++) {
        if (party[i].status != YOKAI_FAINTED) {
            hasActiveYokai = true;
            break;
        }
    }
    
    if (!hasActiveYokai) {
        // 모든 요괴가 기절한 경우 즉시 로그라이트 시스템 실행
        handleRogueliteSystem();
        return -2; // 전투 패배
    }
    
    printTextAndWait("\n다른 요괴를 선택하세요.");
    int newIdx = selectPartyYokai();
    if (newIdx == -1) {
        return -1; // 뒤로 돌아가기
    }
    return newIdx;
    }

int startBattle(const Yokai* enemy) {
    // 현재 전투 중인 상대 요괴 정보 저장
    currentEnemy = *enemy;
    
    // 타입에 따른 색상 설정
    const char* colorCode;
    switch (enemy->type) {
        case TYPE_EVIL_SPIRIT:
            colorCode = "\033[31m";  // 빨간색
            break;
        case TYPE_GHOST:
            colorCode = "\033[35m";  // 보라색
            break;
        case TYPE_MONSTER:
            colorCode = "\033[33m";  // 노란색
            break;
        case TYPE_HUMAN:
            colorCode = "\033[36m";  // 청록색
            break;
        case TYPE_ANIMAL:
            colorCode = "\033[32m";  // 초록색
            break;
        default:
            colorCode = "\033[0m";   // 기본색
    }
    
    // 보스 스테이지인 경우 파란색으로 표시
    char buffer[256];
    if (currentStage.stageNumber % 10 == 0) {
        sprintf(buffer, "\033[34m%s %s[%s]\033[0m Lv.%d (이)가 싸움을 걸어왔다!\n", 
            enemy->name, colorCode, typeToString(enemy->type), enemy->level);
    } else {
        sprintf(buffer, "%s %s[%s]\033[0m Lv.%d (이)가 싸움을 걸어왔다!\n", 
            enemy->name, colorCode, typeToString(enemy->type), enemy->level);
    }
    printText(buffer);
    
    // HP 바 출력
    float maxHP = calculateHP(enemy);
    float currentHP = enemy->currentHP;
    float hpPercentage = (currentHP / maxHP) * 100.0f;
    int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
    
    // HP 바만 출력 (이름은 이미 출력했으므로 생략)
    printText("HP[");
    if (hpPercentage <= 20.0f) {
        printText("\033[31m"); // 빨간색
    } else if (hpPercentage <= 50.0f) {
        printText("\033[33m"); // 노란색
    } else {
        printText("\033[1;32m"); // 초록색
    }
    for (int i = 0; i < HP_BAR_LENGTH; i++) {
        if (i < filledLength) {
            printText("█");
        } else {
            printText("░");
        }
    }
    printText("\033[0m"); // 색상 초기화
    sprintf(buffer, "] %.0f/%.0f\n", currentHP, maxHP);
    printText(buffer);
    
    turnCount = 0; // 전투 시작 시 턴 카운트 초기화
    while (1) {
        int done = showBattleMenu(&currentEnemy);
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
        return handleBattleChoice((BattleChoice)choice, &currentEnemy);
    } else {
        printTextAndWait("\n잘못된 선택입니다. 다시 시도하세요.");
        return 0;
    }
}

// 동료 요괴 선택 함수
int selectPartyYokai() {
    printText("\n동료 요괴를 선택하세요:\n");
    printText("0. 뒤로 가기\n");
    for (int i = 0; i < partyCount; i++) {
        char buffer[256];
        float maxHP = calculateHP(&party[i]);
        
        // 타입에 따른 색상 설정
        const char* colorCode;
        switch (party[i].type) {
            case TYPE_EVIL_SPIRIT:
                colorCode = "\033[31m";  // 빨간색
                break;
            case TYPE_GHOST:
                colorCode = "\033[35m";  // 보라색
                break;
            case TYPE_MONSTER:
                colorCode = "\033[33m";  // 노란색
                break;
            case TYPE_HUMAN:
                colorCode = "\033[36m";  // 청록색
                break;
            case TYPE_ANIMAL:
                colorCode = "\033[32m";  // 초록색
                break;
            default:
                colorCode = "\033[0m";   // 기본색
        }
        
        // 기절 상태 표시
        const char* statusText = party[i].status == YOKAI_FAINTED ? " [기절]" : "";
        
        sprintf(buffer, "%d. %s %s[%s]\033[0m Lv.%d (HP: %.0f/%.0f)%s\n", 
            i+1, 
            party[i].name,
            colorCode,
            typeToString(party[i].type),
            party[i].level,
            party[i].currentHP,
            maxHP,
            statusText);
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
    if (party[idx].status == YOKAI_FAINTED) {
        printTextAndWait("\n기절한 요괴는 선택할 수 없습니다!");
        return selectPartyYokai();
    }
    return idx;
}

// 기술 선택 함수
int selectMove(const Yokai* yokai) {
    printText("\n사용할 기술을 선택하세요:\n");
    for (int i = 0; i < yokai->moveCount; i++) {
        char buffer[128];
        const char* colorCode;
        switch (yokai->moves[i].move.type) {
            case TYPE_EVIL_SPIRIT:
                colorCode = "\033[31m";  // 빨간색
                break;
            case TYPE_GHOST:
                colorCode = "\033[35m";  // 보라색
                break;
            case TYPE_MONSTER:
                colorCode = "\033[33m";  // 노란색
                break;
            case TYPE_HUMAN:
                colorCode = "\033[36m";  // 청록색
                break;
            case TYPE_ANIMAL:
                colorCode = "\033[32m";  // 초록색
                break;
            default:
                colorCode = "\033[0m";   // 기본색
        }
        sprintf(buffer, "%d. %s%s%s\033[0m (공격력: %d, 명중률: %d%%, PP: %d/%d)\n", 
            i+1, 
            colorCode,
            yokai->moves[i].move.name,
            colorCode,
            yokai->moves[i].move.power, 
            yokai->moves[i].move.accuracy, 
            yokai->moves[i].currentPP, 
            yokai->moves[i].move.pp);
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
            int yokaiIdx;
            if (turnCount == 0) {
                yokaiIdx = selectPartyYokai();
                if (yokaiIdx == -1) {
                    return 0; // 뒤로 돌아가기
                }
            } else {
                // 첫 턴 이후에는 이전에 선택한 요괴를 자동 사용
                yokaiIdx = lastYokaiIdx;
                if (party[yokaiIdx].status == YOKAI_FAINTED) {
                    // 모든 요괴가 기절했는지 먼저 확인
                    bool allFainted = true;
                    for (int i = 0; i < partyCount; i++) {
                        if (party[i].status != YOKAI_FAINTED) {
                            allFainted = false;
                            break;
                        }
                    }
                    
                    if (allFainted) {
                        handleRogueliteSystem();
                        return 104; // 전투 패배
                    }
                    
                    // 기절하지 않은 요괴가 있으면 즉시 교체 메뉴 표시
                    printTextAndWait("\n다른 요괴를 선택하세요.");
                    yokaiIdx = selectPartyYokai();
            if (yokaiIdx == -1) {
                return 0; // 뒤로 돌아가기
                    }
                }
            }
            int moveIdx = selectMove(&party[yokaiIdx]);
            party[yokaiIdx].moves[moveIdx].currentPP--;
            int result = executeTurnBattle(&party[yokaiIdx], enemy, moveIdx);
            
            // 전투 결과 처리 전에 요괴가 기절했는지 확인
            if (party[yokaiIdx].currentHP <= 0) {
                party[yokaiIdx].status = YOKAI_FAINTED;
                party[yokaiIdx].currentHP = 0;
                
                // 모든 요괴가 기절했는지 확인
                bool allFainted = true;
                for (int i = 0; i < partyCount; i++) {
                    if (party[i].status != YOKAI_FAINTED) {
                        allFainted = false;
                        break;
                    }
                }
                
                if (allFainted) {
                    handleRogueliteSystem();
                    return 104; // 전투 패배
                }
                
                // 기절하지 않은 요괴가 있으면 즉시 교체 메뉴 표시
                printTextAndWait("\n다른 요괴를 선택하세요.");
                yokaiIdx = selectPartyYokai();
                if (yokaiIdx == -1) {
                    return 0; // 뒤로 돌아가기
                }
            }
            
            handleBattleResult(&party[yokaiIdx], enemy, result);
            turnCount++;
            lastYokaiIdx = yokaiIdx;
            if (result == 1) {
                int exp = calculateBattleExp(enemy);
                gainExp(&party[yokaiIdx], exp);
                return 101;
            } else if (result == -1) {
                return 104;
            }
            return 0;
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
                // 요괴를 파티에 추가 (현재 전투 중인 요괴의 정보 사용)
                if (addYokaiToParty(enemy)) {
                    sprintf(buffer, "\n%s가 동료가 되었습니다!", enemy->name);
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
                
                // 부적 실패 시 이전에 선택한 요괴 사용
                int yokaiIdx = lastYokaiIdx;
                if (party[yokaiIdx].status == YOKAI_FAINTED) {
                    printTextAndWait("\n기절한 요괴는 더 이상 싸울 수 없습니다!");
                    yokaiIdx = selectPartyYokai();
                    if (yokaiIdx == -1) {
                        return 0; // 뒤로 돌아가기
                    }
                }
                
                // 상대 요괴의 랜덤 기술 선택
                int enemyMoveIdx = rand() % enemy->moveCount;
                
                // 상대 요괴의 공격만 실행 (플레이어는 공격하지 않음)
                float damage = calculateDamage(enemy, &party[yokaiIdx], &enemy->moves[enemyMoveIdx].move);
                party[yokaiIdx].currentHP -= damage;
                
                // 데미지 메시지 출력
                sprintf(buffer, "\n%s의 %s 공격!\n", enemy->name, enemy->moves[enemyMoveIdx].move.name);
                printText(buffer);
                sprintf(buffer, "%s는 %.0f의 데미지를 입었다!\n", party[yokaiIdx].name, damage);
                printText(buffer);
                
                // HP 바 업데이트
                float maxHP = calculateHP(&party[yokaiIdx]);
                float hpPercentage = (party[yokaiIdx].currentHP / maxHP) * 100.0f;
                int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
                
                printText("HP[");
                if (hpPercentage <= 20.0f) {
                    printText("\033[31m"); // 빨간색
                } else if (hpPercentage <= 50.0f) {
                    printText("\033[33m"); // 노란색
                } else {
                    printText("\033[1;32m"); // 초록색
                }
                for (int i = 0; i < HP_BAR_LENGTH; i++) {
                    if (i < filledLength) {
                        printText("█");
                    } else {
                        printText("░");
                    }
                }
                printText("\033[0m");
                sprintf(buffer, "] %.0f/%.0f\n", party[yokaiIdx].currentHP, maxHP);
                printText(buffer);
                
                // 전투 결과 확인
                if (party[yokaiIdx].currentHP <= 0) {
                    printTextAndWait("\n전투에서 패배했습니다...");
                    return 104; // 전투 패배
                }
                
                turnCount++;  // 부적 실패 시에도 턴 카운트 증가
                // 기술 선택 없이 바로 턴 종료 (메뉴로 돌아감)
                return 0;
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