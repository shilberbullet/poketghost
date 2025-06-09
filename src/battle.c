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
#include "../core/state.h"
#include "roguelite.h"  // 로그라이트 시스템 추가
#include "settings.h"
#include "region.h"

#define MAX_PARTY 6  // 최대 파티 요괴 수

// 현재 전투 중인 상대 요괴
Yokai currentEnemy;

// 전역 또는 static 변수로 턴 카운트 선언
int turnCount = 0;
int lastYokaiIdx = 0;  // 전역 변수로 선언

// 전투 참여 요괴 인덱스 추적용 배열 및 카운트
int participatedIdx[MAX_PARTY];
int participatedCount = 0;

// 참여 요괴 인덱스 추가 함수(중복 방지)
static void addParticipatedIdx(int idx) {
    for (int i = 0; i < participatedCount; i++) {
        if (participatedIdx[i] == idx) return;
    }
    participatedIdx[participatedCount++] = idx;
}

// 상대 요괴 이름 색상 반환 함수
static const char* getEnemyNameColor() {
    return (gStage.stageNumber % 10 == 0) ? "\033[35m" : "\033[34m";
}

// 외부에서 사용할 수 있도록 export 함수 추가
const char* getEnemyNameColorExport() { return getEnemyNameColor(); }

// 요괴가 기절했을 때의 처리 함수
int handleFaintedYokai(int faintedIdx) {
    char buffer[256];
    sprintf(buffer, "\n%s(이)가 쓰러졌다!\n", gParty[faintedIdx].name);
    printText(buffer);
    
    // 다른 기절하지 않은 요괴가 있는지 확인
    bool hasActiveYokai = false;
    for (int i = 0; i < gPartyCount; i++) {
        if (gParty[i].status != YOKAI_FAINTED) {
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
    return newIdx;  // 뒤로가기 옵션 제거
}

int startBattle(const Yokai* enemy) {
    if (gameSettings.debugMode) {
        char debugbuf[128];
        sprintf(debugbuf, "[DEBUG] startBattle 진입 - isLoadedGame: %d, turnCount: %d\n", gGameState.isLoadedGame, turnCount);
        printText(debugbuf);
    }
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
    
    // 이름 색상 적용
    const char* nameColor = getEnemyNameColor();
    
    // 요괴 등장 메시지 출력
    char buffer[256];
    sprintf(buffer, "%s%s %s[%s]\033[0m Lv.%d (이)가 싸움을 걸어왔다!\n", 
        nameColor, enemy->name, colorCode, typeToString(enemy->type), enemy->level);
    printText(buffer);
    
    // HP 바 출력
    float maxHP = calculateHP(enemy);
    float currentHP = enemy->currentHP;
    float hpPercentage = (currentHP / maxHP) * 100.0f;
    int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
    
    // HP 바 전체를 하나의 문자열로 구성
    char hpBuffer[512];
    sprintf(hpBuffer, "HP[");
    
    // HP 비율에 따른 색상 설정
    if (hpPercentage <= 20.0f) {
        strcat(hpBuffer, "\033[31m"); // 빨간색
    } else if (hpPercentage <= 50.0f) {
        strcat(hpBuffer, "\033[33m"); // 노란색
    } else {
        strcat(hpBuffer, "\033[1;32m"); // 초록색
    }
    
    // HP 바 시각화
    for (int i = 0; i < HP_BAR_LENGTH; i++) {
        if (i < filledLength) {
            strcat(hpBuffer, "█");
        } else {
            strcat(hpBuffer, "░");
        }
    }
    
    // 색상 초기화 및 HP 바 종료
    strcat(hpBuffer, "\033[0m]");
    sprintf(hpBuffer + strlen(hpBuffer), " %.0f/%.0f\n", currentHP, maxHP);
    
    // HP 바 전체를 한 번에 출력
    printText(hpBuffer);
    
    // 이어하기가 아닌 경우에만 초기화
    if (turnCount == 0) {  // turnCount가 0이면 새 전투 시작
        turnCount = 1;  // 첫 턴을 1로 시작
        lastYokaiIdx = 0;  // 마지막 요괴 인덱스 초기화
        participatedCount = 0;
    } else {
    }
    while (1) {
        int done = showBattleMenu(&currentEnemy);
        if (done == 101 || done == 102) {
            int reward = calculateBattleReward();
            addMoney(reward);
            // 경험치 지급: 참여했고 기절하지 않은 모든 요괴에게 지급
            int exp = calculateBattleExp(&currentEnemy);
            for (int i = 0; i < participatedCount; i++) {
                int idx = participatedIdx[i];
                if (gParty[idx].status != YOKAI_FAINTED) {
                    gainExp(&gParty[idx], exp);
                }
            }
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
    
    if (gameSettings.debugMode) {
        char buffer[128];
        sprintf(buffer, "\n[DEBUG] 현재 턴: %d, 마지막 요괴 인덱스: %d\n", turnCount, lastYokaiIdx);
        printText(buffer);
    }
    
    printText("\n무엇을 하시겠습니까?\n");
    printText("1. 싸운다\n");
    printText("2. 부적을 던진다\n");
    printText("3. 동료 요괴를 본다\n");
    if (turnCount > 1) {  // turnCount > 0에서 turnCount > 1로 변경
        printText("4. 동료 요괴 교체\n");
        printText("5. 도망간다\n");
        printText("6. 인벤토리를 본다\n");
        printText("7. 게임을 저장하고 메뉴로 돌아간다\n\n");
        printText("숫자를 입력해주세요: ");
        
        choice = getIntInput();
        if (choice >= 1 && choice <= 7) {
            return handleBattleChoice((BattleChoice)choice, &currentEnemy);
        }
    } else {
        printText("4. 도망간다\n");
        printText("5. 인벤토리를 본다\n");
        printText("6. 게임을 저장하고 메뉴로 돌아간다\n\n");
        printText("숫자를 입력해주세요: ");
        
        choice = getIntInput();
        if (choice >= 1 && choice <= 6) {
            // 첫 턴에는 교체 옵션이 없으므로 메뉴 번호 조정
            if (choice >= 4) choice++; // 4,5,6 -> 5,6,7
            return handleBattleChoice((BattleChoice)choice, &currentEnemy);
        }
    }
    
    printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
        return 0;
}

// 동료 요괴 선택 함수
int selectPartyYokai() {
    printText("\n동료 요괴를 선택하세요:\n");
    for (int i = 0; i < gPartyCount; i++) {
        char buffer[256];
        float maxHP = calculateHP(&gParty[i]);
        
        // 타입에 따른 색상 설정
        const char* colorCode;
        switch (gParty[i].type) {
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
        const char* statusText = "";
        const char* statusColor = "";
        if (gParty[i].status == YOKAI_FAINTED) {
            statusText = " [기절]";
            statusColor = "\033[31m";  // 빨간색
        }
        
        sprintf(buffer, "%d. %s %s[%s]\033[0m Lv.%d (HP: %.0f/%.0f)%s%s\033[0m\n", 
            i+1, 
            gParty[i].name,
            colorCode,
            typeToString(gParty[i].type),
            gParty[i].level,
            gParty[i].currentHP,
            maxHP,
            statusColor,
            statusText);
        printText(buffer);
    }
    printText("선택 (번호): ");
    int idx = getIntInput() - 1;
    if (idx < 0 || idx >= gPartyCount) {
        printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
        return selectPartyYokai();
    }
    if (gParty[idx].status == YOKAI_FAINTED) {
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
        
        // 상성 힌트 생성
        char typeHint[64] = "";
        if (gameSettings.showTypeHint) {
            float effectiveness = getTypeEffectiveness(yokai->moves[i].move.type, currentEnemy.type);
            if (effectiveness > 1.0f) {
                sprintf(typeHint, " \033[32m(강한 공격)\033[0m");
            } else if (effectiveness < 1.0f) {
                sprintf(typeHint, " \033[31m(약한 공격)\033[0m");
            }
        }
        
        sprintf(buffer, "%d. %s%s%s\033[0m (공격력: %d, 명중률: %d%%, PP: %d/%d)%s\n", 
            i+1, 
            colorCode,
            yokai->moves[i].move.name,
            colorCode,
            yokai->moves[i].move.power, 
            yokai->moves[i].move.accuracy, 
            yokai->moves[i].currentPP, 
            yokai->moves[i].move.pp,
            typeHint);
        printText(buffer);
    }
    printText("선택 (번호): ");
    int idx = getIntInput() - 1;
    if (idx < 0 || idx >= yokai->moveCount) {
        printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
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
    printText("숫자를 입력해주세요: ");
    int idx = getIntInput();
    if (idx == 0) return -1;
    idx--;
    if (idx < 0 || idx >= talismanCount) {
        printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
        return selectTalismanFromInventory();
    }
    return talismanIdx[idx];
}

// 동료 요괴 교체 함수
int switchYokai() {
    printText("\n교체할 동료 요괴를 선택하세요:\n");
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
        printText(buffer);
    }
    printText("0. 뒤로 가기\n");
    printText("숫자를 입력해주세요: ");
    
    int choice = getIntInput();
    if (choice == 0) {
        return -1;
    }
    
    if (choice > 0 && choice <= gPartyCount) {
        int idx = choice - 1;
        if (idx == lastYokaiIdx) {
            printTextAndWait("\n이미 전투 중인 요괴입니다!");
            return -1;
        }
        if (gParty[idx].status == YOKAI_FAINTED) {
            printTextAndWait("\n기절한 요괴는 교체할 수 없습니다!");
            return -1;
        }
        addParticipatedIdx(idx);  // 교체된 요괴의 인덱스 추가
        return idx;
    }
    
    printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
    return -1;
}

int handleBattleChoice(BattleChoice choice, Yokai* enemy) {
    if (gameSettings.debugMode) {
        char buffer[128];
        sprintf(buffer, "\n[DEBUG] handleBattleChoice 진입 - 현재 턴: %d, 마지막 요괴 인덱스: %d\n", turnCount, lastYokaiIdx);
        printText(buffer);
    }
    
    switch (choice) {
        case BATTLE_FIGHT: {
            int yokaiIdx;
            if (turnCount == 1) {  // 첫 턴(턴 1)일 때만 요괴 선택
                yokaiIdx = selectPartyYokai();
                if (yokaiIdx == -1) {
                    return 0; // 뒤로 돌아가기
                }
            } else {
                yokaiIdx = lastYokaiIdx;
                if (gParty[yokaiIdx].status == YOKAI_FAINTED) {
                    // 모든 요괴가 기절했는지 먼저 확인
                    bool allFainted = true;
                    for (int i = 0; i < gPartyCount; i++) {
                        if (gParty[i].status != YOKAI_FAINTED) {
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
            // 참여 요괴 인덱스 기록
            addParticipatedIdx(yokaiIdx);
            int moveIdx = selectMove(&gParty[yokaiIdx]);
            if (moveIdx == -1) {
                return 0; // 뒤로 돌아가기
            }
            gParty[yokaiIdx].moves[moveIdx].currentPP--;
            int result = executeTurnBattle(&gParty[yokaiIdx], enemy, moveIdx);
            
            // 전투 결과 처리 전에 요괴가 기절했는지 확인
            if (gParty[yokaiIdx].currentHP <= 0) {
                gParty[yokaiIdx].status = YOKAI_FAINTED;
                gParty[yokaiIdx].currentHP = 0;
                
                // 모든 요괴가 기절했는지 확인
                bool allFainted = true;
                for (int i = 0; i < gPartyCount; i++) {
                    if (gParty[i].status != YOKAI_FAINTED) {
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
            
            handleBattleResult(&gParty[yokaiIdx], enemy, result);
            turnCount++;
            lastYokaiIdx = yokaiIdx;
            if (result == 1) {
                // 경험치 지급은 startBattle에서 일괄 처리
                return 101;
            } else if (result == -1) {
                return 104;
            }
            return 0;
        }
        case BATTLE_TALISMAN: {
            if (gStage.stageNumber % 10 == 0) {
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
                
                // 첫 턴이면 요괴를 선택한 후 상대 요괴의 공격을 받음
                int yokaiIdx;
                if (turnCount == 1) {
                    yokaiIdx = selectPartyYokai();
                    if (yokaiIdx == -1) {
                        return 0; // 뒤로 돌아가기
                    }
                    lastYokaiIdx = yokaiIdx;
                } else {
                    // 첫 턴이 아니면 이전에 선택한 요괴 사용
                    yokaiIdx = lastYokaiIdx;
                }
                
                // 상대 요괴의 랜덤 기술 선택
                int enemyMoveIdx = rand() % enemy->moveCount;
                
                // 상대 요괴의 공격만 실행 (플레이어는 공격하지 않음)
                float damage = calculateDamage(enemy, &gParty[yokaiIdx], &enemy->moves[enemyMoveIdx].move);
                float actualDamage = damage;
                if (actualDamage > gParty[yokaiIdx].currentHP) {
                    actualDamage = gParty[yokaiIdx].currentHP;
                }
                gParty[yokaiIdx].currentHP -= actualDamage;
                
                // HP 바 업데이트
                float maxHP = calculateHP(&gParty[yokaiIdx]);
                float hpPercentage = (gParty[yokaiIdx].currentHP / maxHP) * 100.0f;
                int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
                
                // HP 바 전체를 하나의 문자열로 구성
                char hpBuffer[512];
                sprintf(hpBuffer, "\n%s의 HP: %.0f/%.0f\n[", gParty[yokaiIdx].name, gParty[yokaiIdx].currentHP, maxHP);
                
                // HP 비율에 따른 색상 설정
                if (hpPercentage <= 20.0f) {
                    strcat(hpBuffer, "\033[31m"); // 빨간색
                } else if (hpPercentage <= 50.0f) {
                    strcat(hpBuffer, "\033[33m"); // 노란색
                } else {
                    strcat(hpBuffer, "\033[1;32m"); // 초록색
                }
                
                // HP 바 시각화
                for (int i = 0; i < HP_BAR_LENGTH; i++) {
                    if (i < filledLength) {
                        strcat(hpBuffer, "█");
                    } else {
                        strcat(hpBuffer, "░");
                    }
                }
                
                // 색상 초기화 및 HP 바 종료
                strcat(hpBuffer, "\033[0m]");
                sprintf(hpBuffer + strlen(hpBuffer), " (%s)\n", getHPStatus(&gParty[yokaiIdx]));
                
                // HP 바 전체를 한 번에 출력
                printTextAndWait(hpBuffer);
                
                // 전투 결과 확인
                if (gParty[yokaiIdx].currentHP <= 0) {
                    gParty[yokaiIdx].status = YOKAI_FAINTED;
                    gParty[yokaiIdx].currentHP = 0;
                    
                    // 모든 요괴가 기절했는지 확인
                    bool allFainted = true;
                    for (int i = 0; i < gPartyCount; i++) {
                        if (gParty[i].status != YOKAI_FAINTED) {
                            allFainted = false;
                            break;
                        }
                    }
                    
                    if (allFainted) {
                        handleRogueliteSystem();
                        return 104; // 전투 패배
                    }
                    
                    // 첫 턴이 아닐 때만 요괴 교체 메뉴 표시
                    if (turnCount > 0) {
                        printTextAndWait("\n다른 요괴를 선택하세요.");
                        yokaiIdx = selectPartyYokai();
                        if (yokaiIdx == -1) {
                            return 0; // 뒤로 돌아가기
                        }
                        lastYokaiIdx = yokaiIdx;
                    }
                    turnCount++;  // 턴 증가
                    return 0;  // 다음 턴으로
                }
                turnCount++;  // 턴 증가
                return 0;  // 다음 턴으로
            }
        }
        case BATTLE_CHECK_PARTY:
            printParty();
            return 0;
        case BATTLE_SWITCH: {
            int newYokaiIdx = switchYokai();
            if (newYokaiIdx == -1) {
                return 0;
            }
            // 참여 요괴 인덱스 기록
            addParticipatedIdx(newYokaiIdx);
            // 교체 메시지 출력
            char buffer[256];
            sprintf(buffer, "\n%s를 불러왔다!", gParty[newYokaiIdx].name);
            printText(buffer);
            
            // 상대 요괴의 공격
            int enemyMoveIdx = rand() % enemy->moveCount;
            float damage = calculateDamage(enemy, &gParty[newYokaiIdx], &enemy->moves[enemyMoveIdx].move);
            float actualDamage = damage;
            if (actualDamage > gParty[newYokaiIdx].currentHP) {
                actualDamage = gParty[newYokaiIdx].currentHP;
            }
            gParty[newYokaiIdx].currentHP -= actualDamage;
            
            // HP 바 업데이트
            float maxHP = calculateHP(&gParty[newYokaiIdx]);
            float hpPercentage = (gParty[newYokaiIdx].currentHP / maxHP) * 100.0f;
            int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
            
            // HP 바 전체를 하나의 문자열로 구성
            char hpBuffer[512];
            sprintf(hpBuffer, "\n%s의 HP: %.0f/%.0f\n[", gParty[newYokaiIdx].name, gParty[newYokaiIdx].currentHP, maxHP);
            
            // HP 비율에 따른 색상 설정
            if (hpPercentage <= 20.0f) {
                strcat(hpBuffer, "\033[31m"); // 빨간색
            } else if (hpPercentage <= 50.0f) {
                strcat(hpBuffer, "\033[33m"); // 노란색
            } else {
                strcat(hpBuffer, "\033[1;32m"); // 초록색
            }
            
            // HP 바 시각화
            for (int i = 0; i < HP_BAR_LENGTH; i++) {
                if (i < filledLength) {
                    strcat(hpBuffer, "█");
                } else {
                    strcat(hpBuffer, "░");
                }
            }
            
            // 색상 초기화 및 HP 바 종료
            strcat(hpBuffer, "\033[0m]");
            sprintf(hpBuffer + strlen(hpBuffer), " (%s)\n", getHPStatus(&gParty[newYokaiIdx]));
            
            // HP 바 전체를 한 번에 출력
            printTextAndWait(hpBuffer);
            
            // 전투 결과 확인
            if (gParty[newYokaiIdx].currentHP <= 0) {
                gParty[newYokaiIdx].status = YOKAI_FAINTED;
                gParty[newYokaiIdx].currentHP = 0;
                
                // 모든 요괴가 기절했는지 확인
                bool allFainted = true;
                for (int i = 0; i < gPartyCount; i++) {
                    if (gParty[i].status != YOKAI_FAINTED) {
                        allFainted = false;
                        break;
                    }
                }
                
                if (allFainted) {
                    handleRogueliteSystem();
                    return 104; // 전투 패배
                }
                
                // 기절하지 않은 요괴가 있으면 교체 메뉴 표시
                printTextAndWait("\n다른 요괴를 선택하세요.");
                newYokaiIdx = selectPartyYokai();
                if (newYokaiIdx == -1) {
                    return 0; // 뒤로 돌아가기
                }
                lastYokaiIdx = newYokaiIdx;
                turnCount++;
                return 0;
            }
            
            lastYokaiIdx = newYokaiIdx;
            turnCount++;
            return 0;
        }
        case BATTLE_RUN: {
            int escapeResult = tryToEscape();  // 도망치기 시스템 사용
            if (escapeResult == ESCAPE_FAIL) {
                // 도망치기 실패 시 첫 턴이면 요괴 선택
                int yokaiIdx;
                if (turnCount == 0) {
                    printTextAndWait("\n공격을 받을 요괴를 선택하세요.");
                    yokaiIdx = selectPartyYokai();
                    if (yokaiIdx == -1) {
                        return 0; // 뒤로 돌아가기
                    }
                } else {
                    // 첫 턴이 아니면 이전에 선택한 요괴 사용
                    yokaiIdx = lastYokaiIdx;
                    if (gParty[yokaiIdx].status == YOKAI_FAINTED) {
                        printTextAndWait("\n기절한 요괴는 더 이상 싸울 수 없습니다!");
                        yokaiIdx = selectPartyYokai();
                        if (yokaiIdx == -1) {
                            return 0; // 뒤로 돌아가기
                        }
                    }
                }
                
                // 상대 요괴의 랜덤 기술 선택
                int enemyMoveIdx = rand() % enemy->moveCount;
                
                // 상대 요괴의 공격만 실행 (플레이어는 공격하지 않음)
                float damage = calculateDamage(enemy, &gParty[yokaiIdx], &enemy->moves[enemyMoveIdx].move);
                float actualDamage = damage;
                if (actualDamage > gParty[yokaiIdx].currentHP) {
                    actualDamage = gParty[yokaiIdx].currentHP;
                }
                gParty[yokaiIdx].currentHP -= actualDamage;
                
                // HP 바 업데이트
                float maxHP = calculateHP(&gParty[yokaiIdx]);
                float hpPercentage = (gParty[yokaiIdx].currentHP / maxHP) * 100.0f;
                int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
                
                // HP 바 전체를 하나의 문자열로 구성
                char hpBuffer[512];
                sprintf(hpBuffer, "\n%s의 HP: %.0f/%.0f\n[", gParty[yokaiIdx].name, gParty[yokaiIdx].currentHP, maxHP);
                
                // HP 비율에 따른 색상 설정
                if (hpPercentage <= 20.0f) {
                    strcat(hpBuffer, "\033[31m"); // 빨간색
                } else if (hpPercentage <= 50.0f) {
                    strcat(hpBuffer, "\033[33m"); // 노란색
                } else {
                    strcat(hpBuffer, "\033[1;32m"); // 초록색
                }
                
                // HP 바 시각화
                for (int i = 0; i < HP_BAR_LENGTH; i++) {
                    if (i < filledLength) {
                        strcat(hpBuffer, "█");
                    } else {
                        strcat(hpBuffer, "░");
                    }
                }
                
                // 색상 초기화 및 HP 바 종료
                strcat(hpBuffer, "\033[0m]");
                sprintf(hpBuffer + strlen(hpBuffer), " (%s)\n", getHPStatus(&gParty[yokaiIdx]));
                
                // HP 바 전체를 한 번에 출력
                printTextAndWait(hpBuffer);
                
                // 전투 결과 확인
                if (gParty[yokaiIdx].currentHP <= 0) {
                    gParty[yokaiIdx].status = YOKAI_FAINTED;
                    gParty[yokaiIdx].currentHP = 0;

                    // 모든 요괴가 기절했는지 확인
                    bool allFainted = true;
                    for (int i = 0; i < gPartyCount; i++) {
                        if (gParty[i].status != YOKAI_FAINTED) {
                            allFainted = false;
                            break;
                        }
                    }
                    if (allFainted) {
                        handleRogueliteSystem();
                        return 104; // 전투 패배
                    }

                    printTextAndWait("\n전투에서 패배했습니다...");
                    return 104; // 전투 패배
                }
                
                turnCount++;  // 도망치기 실패 시에도 턴 카운트 증가
                return 0;
            }
            return escapeResult;
            }
        case BATTLE_CHECK_INVENTORY:
            printInventory();
            return 0;
        case BATTLE_SAVE_AND_EXIT:
            gGameState.isManualSave = true;  // 수동 저장 플래그 설정
            saveGame();
            printTextAndWait("\n게임이 저장되었습니다. 메뉴로 돌아갑니다.");
            gGameState.isRunning = 0;
            return 2;
    }
    return 0;
}

void printDamageMessage(Yokai* attacker, Yokai* defender, int damage) {
    char buffer[256];
    int actualDamage = damage;
    
    // 실제 데미지가 현재 HP보다 크면 현재 HP만큼만 데미지를 입힘
    if (actualDamage > defender->currentHP) {
        actualDamage = defender->currentHP;
    }
    
    sprintf(buffer, "%s에게 %d의 데미지를 입혔다!", defender->name, actualDamage);
    printTextAndWait(buffer);
} 