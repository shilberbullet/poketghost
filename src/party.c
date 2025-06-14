#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "party.h"
#include "yokai.h"
#include "move.h"
#include "text.h"
#include "input.h"
#include "game.h"
#include "exp_system.h"
#include "settings.h"
#include "../core/state.h"
#include <windows.h>
#ifndef YOKAI_DESC_MAX
#define YOKAI_DESC_MAX 256
#endif

// 랜덤 기술 할당 함수 선언
void assignRandomMoves(Yokai* y);

// 파티 요괴 배열과 개수는 state 모듈에서 관리

// 파티 초기화 함수
void initParty() {
    gPartyCount = 0;
    // 도깨비 요괴를 파일에서 불러와 추가
    Yokai* dokkaebi = findYokaiByName("도깨비");
    if (dokkaebi) {
        memset(&gParty[0], 0, sizeof(Yokai));  // 구조체 전체를 0으로 초기화
        gParty[0] = *dokkaebi;                 // 기본 정보 복사
        gParty[0].level = 5;                   // 초기 레벨 5로 설정
        gParty[0].currentHP = calculateHP(&gParty[0]);  // HP 초기화
        
        // 도감 설명 명시적 복사
        strncpy(gParty[0].desc, dokkaebi->desc, YOKAI_DESC_MAX - 1);
        gParty[0].desc[YOKAI_DESC_MAX - 1] = '\0';
        
        // learnableMoves 복사
        gParty[0].learnableMoveCount = dokkaebi->learnableMoveCount;
        for (int i = 0; i < dokkaebi->learnableMoveCount; i++) {
            gParty[0].learnableMoves[i] = dokkaebi->learnableMoves[i];
        }
        assignRandomMoves(&gParty[0]);   // 랜덤 기술 할당
        gPartyCount = 1;
    }
}

// 요괴 성불 처리 함수
void releaseYokai(int index) {
    if (index < 0 || index >= gPartyCount) return;
    
    // 요괴를 성불 상태로 설정
    gParty[index].status = YOKAI_RELEASED;
}

// 새로운 요괴를 잡았을 때 파티가 가득 찼을 경우의 처리
int handleFullParty(const Yokai* newYokai) {
    if (gPartyCount < MAX_PARTY_SIZE) {
        return addYokaiToParty(newYokai);
    }

    printText("\n동료 요괴가 가득 찼습니다!\n");
    printText("1. 잡은 요괴를 성불시킨다\n");
    printText("2. 동료 요괴를 성불시킨다\n");
    printText("숫자를 입력해주세요: ");

    int choice = getIntInput();
    if (choice == 1) {
        printTextAndWait("\n잡은 요괴를 성불시켰습니다.");
        fastSleep(500);
        return 0;
    } else if (choice == 2) {
        while (1) {
            printText("\n성불 시킬 동료 요괴를 선택하세요:\n");
            for (int i = 0; i < gPartyCount; i++) {
                char buffer[256];
                const char* statusText = "";
                const char* statusColor = "";
                switch (gParty[i].status) {
                    case YOKAI_NORMAL: 
                        statusText = "정상"; 
                        statusColor = "\033[0m";  // 기본색
                        break;
                    case YOKAI_FAINTED: 
                        statusText = "기절"; 
                        statusColor = "\033[31m";  // 빨간색
                        break;
                    default: 
                        statusText = "알 수 없음"; 
                        statusColor = "\033[0m";  // 기본색
                        break;
                }
                sprintf(buffer, "%d. %s (Lv.%d, 상태: %s%s\033[0m, 체력: %d, 공격력: %d, 방어력: %d)\n", 
                    i+1, gParty[i].name, gParty[i].level, statusColor, statusText, gParty[i].stamina, gParty[i].attack, gParty[i].defense);
                printText(buffer);
            }
            printText("0. 뒤로 돌아간다\n");
            printText("숫자를 입력해주세요: ");

            int yokaiChoice = getIntInput();
            if (yokaiChoice == 0) {
                return handleFullParty(newYokai); // 다시 선택 메뉴로
            }
            yokaiChoice--; // 0-based index로 변환

            if (yokaiChoice >= 0 && yokaiChoice < gPartyCount) {
                char buffer[128];
                sprintf(buffer, "\n%s를 성불시켰습니다.", gParty[yokaiChoice].name);
                printTextAndWait(buffer);
                
                // 선택된 요괴를 성불시키고 새로운 요괴 추가
                releaseYokai(yokaiChoice);
                return addYokaiToParty(newYokai);
            } else {
                printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
                continue;  // 다시 선택하도록 continue 추가
            }
        }
    } else {
        printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
        return handleFullParty(newYokai); // 다시 선택 메뉴로
    }
    return 0;
}

// 파티에 새로운 요괴 추가 함수
int addYokaiToParty(const Yokai* yokai) {
    // 성불된 요괴의 자리를 찾아서 재사용
    int releasedSlot = -1;
    for (int i = 0; i < gPartyCount; i++) {
        if (gParty[i].status == YOKAI_RELEASED) {
            releasedSlot = i;
            break;
        }
    }

    if (releasedSlot != -1) {
        // 성불된 요괴의 자리에 새로운 요괴 추가
        gParty[releasedSlot] = *yokai;
        gParty[releasedSlot].status = YOKAI_NORMAL;  // 상태를 정상으로 설정
        
        // 도감 설명 명시적 복사
        strncpy(gParty[releasedSlot].desc, yokai->desc, YOKAI_DESC_MAX - 1);
        gParty[releasedSlot].desc[YOKAI_DESC_MAX - 1] = '\0';
        
        // learnableMoves 복사
        gParty[releasedSlot].learnableMoveCount = yokai->learnableMoveCount;
        for (int i = 0; i < yokai->learnableMoveCount; i++) {
            gParty[releasedSlot].learnableMoves[i] = yokai->learnableMoves[i];
        }

        // 현재 가지고 있는 기술과 PP 복사
        gParty[releasedSlot].moveCount = yokai->moveCount;
        for (int i = 0; i < yokai->moveCount; i++) {
            gParty[releasedSlot].moves[i].move = yokai->moves[i].move;
            gParty[releasedSlot].moves[i].currentPP = yokai->moves[i].currentPP;
            // 배운 기술 목록에도 추가
            gParty[releasedSlot].learnedMoves[gParty[releasedSlot].learnedMoveCount++] = yokai->moves[i].move;
        }
        return 1;
    }

    if (gPartyCount >= MAX_PARTY_SIZE) {
        return handleFullParty(yokai);
    }
    
    // 요괴 정보 직접 복사
    gParty[gPartyCount] = *yokai;  // 기본 정보 복사
    gParty[gPartyCount].status = YOKAI_NORMAL;  // 상태를 정상으로 설정
    
    // 도감 설명 명시적 복사
    strncpy(gParty[gPartyCount].desc, yokai->desc, YOKAI_DESC_MAX - 1);
    gParty[gPartyCount].desc[YOKAI_DESC_MAX - 1] = '\0';
    
    // learnableMoves 복사
    gParty[gPartyCount].learnableMoveCount = yokai->learnableMoveCount;
    for (int i = 0; i < yokai->learnableMoveCount; i++) {
        gParty[gPartyCount].learnableMoves[i] = yokai->learnableMoves[i];
    }

    // 현재 가지고 있는 기술과 PP 복사
    gParty[gPartyCount].moveCount = yokai->moveCount;
    for (int i = 0; i < yokai->moveCount; i++) {
        gParty[gPartyCount].moves[i].move = yokai->moves[i].move;
        gParty[gPartyCount].moves[i].currentPP = yokai->moves[i].currentPP;
        // 배운 기술 목록에도 추가
        gParty[gPartyCount].learnedMoves[gParty[gPartyCount].learnedMoveCount++] = yokai->moves[i].move;
    }

    gPartyCount++;
    return 1;
}

// 파티 요괴 목록 출력 함수
void printParty() {
    printText("\n=== 동료 요괴 목록 ===\n");
    printText("[0] 뒤로 가기\n");
    for (int i = 0; i < gPartyCount; i++) {
        char buffer[256];
        float maxHP = calculateHP(&gParty[i]);
        sprintf(buffer, "[%d] %s Lv.%d (HP: %.0f/%.0f)\n", 
            i+1, 
            gParty[i].name, 
            gParty[i].level,
            gParty[i].currentHP,
            maxHP);
        printText(buffer);
    }
    printText("\n숫자를 입력해주세요: ");
    
    int choice = getIntInput();
    if (choice == 0) {
        return;
    }
    
    if (choice > 0 && choice <= gPartyCount) {
        int idx = choice - 1;
        char buffer[2048];
        
        // 기본 정보 출력
        float maxHP = calculateHP(&gParty[idx]);
        sprintf(buffer, "\n=== %s Lv.%d의 정보 ===\n", gParty[idx].name, gParty[idx].level);
        printText(buffer);
        sprintf(buffer, "체력 종족값: %d\n", gParty[idx].stamina);
        printText(buffer);
        sprintf(buffer, "현재 HP: %.0f/%.0f\n", gParty[idx].currentHP, maxHP);
        printText(buffer);
        sprintf(buffer, "공격력: %d\n", gParty[idx].attack);
        printText(buffer);
        sprintf(buffer, "방어력: %d\n", gParty[idx].defense);
        printText(buffer);
        sprintf(buffer, "스피드: %d\n", gParty[idx].speed);
        printText(buffer);
        sprintf(buffer, "상성: %s\n", typeNames[gParty[idx].type]);
        printText(buffer);
        
        // 경험치 정보 출력
        int requiredExp = calculateRequiredExp(gParty[idx].level);
        sprintf(buffer, "경험치: %d/%d\n", gParty[idx].exp, requiredExp);
        printText(buffer);
        
        // 도감 설명을 별도로 출력
        printText("\n도감설명:\n");
        char* desc = gParty[idx].desc;
        while (*desc) {
            char temp[256];
            int i = 0;
            while (*desc && i < 255) {
                temp[i++] = *desc++;
            }
            temp[i] = '\0';
            printText(temp);
            printText("\n");
        }
        
        // 기술 목록 출력
        printText("\n기술 목록:\n");
        for (int i = 0; i < gParty[idx].moveCount; i++) {
            sprintf(buffer, "%d. %s (상성: %s, 공격력: %d, 명중률: %d%%, PP: %d/%d)\n", 
                i+1, gParty[idx].moves[i].move.name, typeToString(gParty[idx].moves[i].move.type), 
                gParty[idx].moves[i].move.power, gParty[idx].moves[i].move.accuracy,
                gParty[idx].moves[i].currentPP, gParty[idx].moves[i].move.pp);
            printText(buffer);
        }
    }
}

// 모든 요괴의 PP를 초기화하는 함수
void resetAllYokaiPP() {
    for (int i = 0; i < gPartyCount; i++) {
        for (int j = 0; j < gParty[i].moveCount; j++) {
            gParty[i].moves[j].currentPP = gParty[i].moves[j].move.pp;
        }
    }
} 