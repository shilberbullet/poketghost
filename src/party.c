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
#include "hp_system.h"
#include "logger.h"
#ifndef YOKAI_DESC_MAX
#define YOKAI_DESC_MAX 256
#endif

#define HP_BAR_LENGTH 20  // HP 바 길이 정의
#define MAX_FORGOTTEN_MOVES 10  // 잊은 기술 최대 개수

// 랜덤 기술 할당 함수 선언
void assignRandomMoves(Yokai* y, int level);

// 파티 요괴 배열과 개수는 state 모듈에서 관리

// 파티 초기화 함수
void initParty() {
    LOG_FUNCTION_EXECUTION("initParty");
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
        
        // learnableMoves 복사 (레벨업 시 중급/고급 기술 배우기 위해 원본 유지)
        gParty[0].learnableMoveCount = dokkaebi->learnableMoveCount;
        for (int i = 0; i < dokkaebi->learnableMoveCount; i++) {
            gParty[0].learnableMoves[i] = dokkaebi->learnableMoves[i];
        }
        
        // 도깨비 초기 기술: 하드코딩된 초급 기술 4개만 할당
        Move dokkaebiBasicMoves[] = {
            {"불씨던지기", TYPE_MONSTER, 25, 90, 20, "작은 불씨를 던져 화상을 입힌다", MOVE_BASIC},
            {"돌던지기", TYPE_HUMAN, 25, 95, 25, "돌을 던져 적을 공격한다", MOVE_BASIC},
            {"진흙던지기", TYPE_HUMAN, 20, 100, 25, "진흙을 뿌려 시야를 방해한다", MOVE_BASIC},
            {"호미찌르기", TYPE_HUMAN, 25, 100, 20, "호미날로 적을 강하게 찌른다", MOVE_BASIC},
            {"몸통박치기", TYPE_MONSTER, 25, 100, 20, "몸 전체로 부딪혀 공격한다", MOVE_BASIC},
            {"소리지르기", TYPE_HUMAN, 20, 100, 30, "소리를 질러 적을 공격한다", MOVE_BASIC},
            {"짖기", TYPE_ANIMAL, 15, 100, 30, "소리로 적을 공격한다", MOVE_BASIC},
            {"할퀴기", TYPE_ANIMAL, 25, 100, 20, "날카로운 발톱으로 할퀴어 공격한다", MOVE_BASIC}
        };
        int dokkaebiBasicCount = 8;
        
        // 초급 기술 중 랜덤하게 4개 선택
        gParty[0].moveCount = 0;
        for (int i = 0; i < 4; i++) {
            int randomIndex = rand() % dokkaebiBasicCount;
            gParty[0].moves[i].move = dokkaebiBasicMoves[randomIndex];
            gParty[0].moves[i].currentPP = dokkaebiBasicMoves[randomIndex].pp;
            gParty[0].moveCount++;
            
            // 중복 방지: 선택된 기술을 배열 끝으로 이동
            Move temp = dokkaebiBasicMoves[randomIndex];
            dokkaebiBasicMoves[randomIndex] = dokkaebiBasicMoves[dokkaebiBasicCount - 1];
            dokkaebiBasicMoves[dokkaebiBasicCount - 1] = temp;
            dokkaebiBasicCount--;
        }
        
        // 잊은 기술 목록 초기화 (도깨비는 생성 시 잊은 기술을 추가하지 않음)
        gParty[0].forgottenMoveCount = 0;
        
        gParty[0].learnedMoveCount = gParty[0].moveCount;
        for(int i=0; i<gParty[0].learnedMoveCount; ++i) {
            gParty[0].learnedMoves[i] = gParty[0].moves[i].move;
        }
        
        gPartyCount = 1;
    }
}

// 요괴 성불 처리 함수
void releaseYokai(int index) {
    LOG_FUNCTION_EXECUTION("releaseYokai");
    if (index < 0 || index >= gPartyCount) return;
    if (index == 0) {
        printText("\n도깨비는 성불시킬 수 없습니다.\n");
        return;
    }
    
    // 요괴의 상성에 따른 색상 코드 설정
    const char* colorCode;
    switch (gParty[index].type) {
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
            break;
    }
    gPartyCount--;
}

// 새로운 요괴를 잡았을 때 파티가 가득 찼을 경우의 처리
int handleFullParty(const Yokai* newYokai) {
    LOG_FUNCTION_EXECUTION("handleFullParty");
    if (gPartyCount < MAX_PARTY_SIZE) {
        return addYokaiToParty(newYokai);
    }

    printText("\n동료 요괴가 가득 찼습니다!\n");
    printText("1. 잡은 요괴를 성불시킨다\n");
    printText("2. 동료 요괴를 성불시킨다\n");
    printText("숫자를 입력해주세요: ");

    int choice = getIntInput();
    if (choice == 1) {
        printText("\n잡은 요괴를 성불시켰습니다.");
        fastSleep(500);
        return 0;
    } else if (choice == 2) {
        while (1) {
            printText("\n성불 시킬 동료 요괴를 선택하세요:\n");
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
                // 상태 표시
                const char* statusText = "";
                const char* statusColor = "";
                switch (gParty[i].status) {
                    case YOKAI_NORMAL: 
                        statusText = "정상"; 
                        statusColor = "\033[0m";
                        break;
                    case YOKAI_FAINTED: 
                        statusText = "기절"; 
                        statusColor = "\033[31m";
                        break;
                    default: 
                        statusText = "알 수 없음"; 
                        statusColor = "\033[0m";
                        break;
                }
                sprintf(buffer, "%d. %s (%s%s\033[0m, Lv.%d, HP: %.0f/%.0f, 상태: %s%s\033[0m, 공격력: %d, 방어력: %d)\n",
                    i+1,
                    gParty[i].name,
                    colorCode,
                    typeToString(gParty[i].type),
                    gParty[i].level,
                    gParty[i].currentHP,
                    maxHP,
                    statusColor,
                    statusText,
                    gParty[i].attack,
                    gParty[i].defense);
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
                if (yokaiChoice == 0) {
                    printText("\n도깨비는 성불시킬 수 없습니다. 다른 요괴를 선택하세요.");
                    continue;
                }
                char buffer[128];
                sprintf(buffer, "\n%s를 성불시켰습니다.", gParty[yokaiChoice].name);
                printText(buffer);
                // 선택된 요괴를 성불시키고 새로운 요괴 추가
                releaseYokai(yokaiChoice);
                return addYokaiToParty(newYokai);
            } else {
                printText("\n잘못된 선택입니다. 다시 선택하세요.");
                continue;  // 다시 선택하도록 continue 추가
            }
        }
    } else {
        printText("\n잘못된 선택입니다. 다시 선택하세요.");
        return handleFullParty(newYokai); // 다시 선택 메뉴로
    }
    return 0;
}

// 파티에 새로운 요괴 추가 함수
int addYokaiToParty(const Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("addYokaiToParty");
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

// 요괴 정보 서브메뉴 표시 함수
void showYokaiSubMenu(const Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("showYokaiSubMenu");
    while (1) {
        char buffer[256];
        sprintf(buffer, "\n=== 요괴 정보 ===\n");
        sprintf(buffer + strlen(buffer), "1. 종족값 보기\n");
        sprintf(buffer + strlen(buffer), "2. 도감 설명 보기\n");
        sprintf(buffer + strlen(buffer), "3. HP를 본다\n");
        sprintf(buffer + strlen(buffer), "4. 경험치 보기\n");
        sprintf(buffer + strlen(buffer), "5. 기술 목록 보기\n");
        sprintf(buffer + strlen(buffer), "6. 요괴 인벤토리 보기\n");
        sprintf(buffer + strlen(buffer), "7. 동료요괴를 성불시킨다\n");
        if (gameSettings.debugMode) {
            sprintf(buffer + strlen(buffer), "8. 배울 수 있는 기술 목록 보기\n");
            sprintf(buffer + strlen(buffer), "9. 잊은 기술 보기\n");
            sprintf(buffer + strlen(buffer), "10. 뒤로 가기\n");
        } else {
            sprintf(buffer + strlen(buffer), "8. 뒤로 가기\n");
        }
        sprintf(buffer + strlen(buffer), "\n숫자를 입력해주세요: ");
        printText(buffer);
        
        int choice = getIntInput();
        if ((!gameSettings.debugMode && choice == 8) || (gameSettings.debugMode && choice == 10)) {
            return;
        }
        
        switch (choice) {
            case 1: {  // 종족값 보기
                char buffer[512];
                const char* colorCode;
                switch (yokai->type) {
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
                sprintf(buffer, "\n=== %s의 종족값 ===\n", yokai->name);
                sprintf(buffer + strlen(buffer), "공격력: %d\n", yokai->attack);
                sprintf(buffer + strlen(buffer), "방어력: %d\n", yokai->defense);
                sprintf(buffer + strlen(buffer), "체력: %d\n", yokai->stamina);
                sprintf(buffer + strlen(buffer), "스피드: %d\n", yokai->speed);
                sprintf(buffer + strlen(buffer), "상성: %s%s\033[0m\n", colorCode, typeNames[yokai->type]);
                printText(buffer);
                printText("\n엔터를 눌러 돌아가기...");
                getchar();
                break;
            }
            case 2: {  // 도감 설명 보기
                char buffer[256];
                sprintf(buffer, "\n=== %s의 도감 설명 ===\n", yokai->name);
                sprintf(buffer + strlen(buffer), "%s\n", yokai->desc);
                printText(buffer);
                printText("\n엔터를 눌러 돌아가기...");
                getchar();
                break;
            }
            case 3: {  // HP 보기
                char buffer[256];
                float maxHP = calculateHP(yokai);
                float hpPercentage = (yokai->currentHP / maxHP) * 100.0f;
                int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
                
                sprintf(buffer, "\n=== %s의 HP ===\n", yokai->name);
                sprintf(buffer + strlen(buffer), "현재 HP: %.0f/%.0f\n", yokai->currentHP, maxHP);
                // HP 바 시각화
                strcat(buffer, "HP[");
                if (hpPercentage <= 20.0f) {
                    strcat(buffer, "\033[31m"); // 빨간색
                } else if (hpPercentage <= 50.0f) {
                    strcat(buffer, "\033[33m"); // 노란색
                } else {
                    strcat(buffer, "\033[1;32m"); // 초록색
                }
                for (int i = 0; i < HP_BAR_LENGTH; i++) {
                    if (i < filledLength) {
                        strcat(buffer, "█");
                    } else {
                        strcat(buffer, "░");
                    }
                }
                strcat(buffer, "\033[0m]");
                printText(buffer);
                printText("\n엔터를 눌러 돌아가기...");
                getchar();
                            break;
            }
            case 4: {  // 경험치 보기
                char buffer[256];
                int requiredExp = calculateRequiredExp(yokai->level);
                sprintf(buffer, "\n=== %s의 경험치 ===\n", yokai->name);
                sprintf(buffer + strlen(buffer), "현재 경험치: %d\n", yokai->exp);
                sprintf(buffer + strlen(buffer), "다음 레벨까지: %d\n", requiredExp - yokai->exp);
                printText(buffer);
                printText("\n엔터를 눌러 돌아가기...");
                getchar();
                            break;
            }
            case 5: {  // 기술 목록 보기
                char buffer[1024];
                sprintf(buffer, "\n=== %s의 기술 목록 ===\n", yokai->name);
                for (int i = 0; i < yokai->moveCount; i++) {
                    char* typeColor = "";
                    switch (yokai->moves[i].move.type) {
                        case TYPE_EVIL_SPIRIT: typeColor = "\033[31m"; break; // 빨간색
                        case TYPE_GHOST: typeColor = "\033[35m"; break;      // 보라색
                        case TYPE_MONSTER: typeColor = "\033[33m"; break;    // 노란색
                        case TYPE_HUMAN: typeColor = "\033[36m"; break;      // 청록색
                        case TYPE_ANIMAL: typeColor = "\033[32m"; break;     // 초록색
                        default: typeColor = "\033[0m"; break;               // 기본색
                    }
                    sprintf(buffer + strlen(buffer), "%d. %s%s\033[0m (PP: %d/%d, 공격력: %d, 명중률: %d%%)\n", 
                        i + 1, 
                        typeColor,
                        yokai->moves[i].move.name,
                        yokai->moves[i].currentPP,
                        yokai->moves[i].move.pp,
                        yokai->moves[i].move.power,
                        yokai->moves[i].move.accuracy);
                }
                printText(buffer);
                printText("\n엔터를 눌러 돌아가기...");
                int c;
                while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
                clearInputBuffer(); // 콘솔 입력 버퍼 비우기
                            break;
            }
            case 6: {  // 요괴 인벤토리 보기
                char buffer[1024];
                sprintf(buffer, "\n=== %s의 인벤토리 ===\n", yokai->name);
                if (yokai->yokaiInventoryCount == 0) {
                    sprintf(buffer + strlen(buffer), "인벤토리가 비어있습니다.\n");
                } else {
                    for (int i = 0; i < yokai->yokaiInventoryCount; i++) {
                        sprintf(buffer + strlen(buffer), "%d. %s (수량: %d)\n", 
                            i + 1, 
                            yokai->yokaiInventory[i].item.name,
                            yokai->yokaiInventory[i].count);
                    }
                }
                printText(buffer);
                printText("\n엔터를 눌러 돌아가기...");
                int c;
                while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
                clearInputBuffer(); // 콘솔 입력 버퍼 비우기
                            break;
            }
            case 7: {  // 동료요괴를 성불시킨다
                int idx = -1;
                // 파티 배열에서 해당 요괴의 인덱스 찾기
                for (int i = 0; i < gPartyCount; i++) {
                    if (&gParty[i] == yokai) {
                        idx = i;
                        break;
                    }
                }
                if (idx == 0) {
                    printText("\n도깨비는 성불시킬 수 없습니다.\n");
                } else if (idx > 0 && idx < gPartyCount) {
                    releaseYokai(idx);
                    printText("\n성불이 완료되었습니다. 목록에서 사라집니다.\n");
                    return; // 성불 후 메뉴 종료
                } else {
                    printText("\n오류: 요괴 인덱스를 찾을 수 없습니다.\n");
                }
                break;
            }
            case 8: {  // 배울 수 있는 기술 목록
                sprintf(buffer, "%s의 배울 수 있는 기술 목록:\n\n", yokai->name);
                for (int i = 0; i < yokai->learnableMoveCount; i++) {
                    char* typeColor = "";
                    switch (yokai->learnableMoves[i].type) {
                        case TYPE_EVIL_SPIRIT: typeColor = "\033[31m"; break; // 빨간색
                        case TYPE_GHOST: typeColor = "\033[35m"; break;      // 보라색
                        case TYPE_MONSTER: typeColor = "\033[33m"; break;    // 노란색
                        case TYPE_HUMAN: typeColor = "\033[36m"; break;      // 청록색
                        case TYPE_ANIMAL: typeColor = "\033[32m"; break;     // 초록색
                        default: typeColor = "\033[0m"; break;               // 기본색
                    }
                    sprintf(buffer + strlen(buffer), "%d. %s%s\033[0m (PP: %d/%d, 공격력: %d, 명중률: %d%%)\n", 
                        i + 1, 
                        typeColor,
                        yokai->learnableMoves[i].name,
                        yokai->learnableMoves[i].pp,
                        yokai->learnableMoves[i].pp,
                        yokai->learnableMoves[i].power,
                        yokai->learnableMoves[i].accuracy);
                }
                    printText(buffer);
                printText("\n엔터를 눌러 돌아가기...");
                int c;
                while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
                clearInputBuffer(); // 콘솔 입력 버퍼 비우기
                break;
            }
            case 9: {  // 잊은 기술 목록
                sprintf(buffer, "%s의 잊은 기술 목록:\n\n", yokai->name);
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
                    sprintf(buffer + strlen(buffer), "%d. %s%s\033[0m (PP: %d/%d, 공격력: %d, 명중률: %d%%)\n", 
                        i + 1, 
                        typeColor,
                        yokai->forgottenMoves[i].name,
                        yokai->forgottenMoves[i].pp,
                        yokai->forgottenMoves[i].pp,
                        yokai->forgottenMoves[i].power,
                        yokai->forgottenMoves[i].accuracy);
                }
                printText(buffer);
                printText("\n엔터를 눌러 돌아가기...");
                int c;
                while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
                clearInputBuffer(); // 콘솔 입력 버퍼 비우기
                break;
            }
            default:
                printText("\n잘못된 선택입니다.");
                break;
        }
    }
}

// 파티 요괴 목록 출력 함수
void printParty() {
    LOG_FUNCTION_EXECUTION("printParty");
    char buffer[256];
    sprintf(buffer, "\n=== 동료 요괴 목록 ===\n");
    for (int i = 0; i < gPartyCount; i++) {
        if (gParty[i].status != YOKAI_RELEASED) {
            sprintf(buffer + strlen(buffer), "%d. %s Lv.%d\n", 
                i + 1, gParty[i].name, gParty[i].level);
        }
    }
    sprintf(buffer + strlen(buffer), "0. 뒤로 가기\n\n");
    sprintf(buffer + strlen(buffer), "숫자를 입력해주세요: ");
    printText(buffer);
    
    int choice = getIntInput();
    if (choice == 0) {
        return;
    }
    
    if (choice > 0 && choice <= gPartyCount) {
        if (gParty[choice - 1].status == YOKAI_RELEASED) {
            printText("\n이미 성불한 요괴입니다.");
            return;
        }
        showYokaiSubMenu(&gParty[choice - 1]);
    } else {
        printText("\n잘못된 선택입니다.");
    }
}

// 모든 요괴의 PP를 초기화하는 함수
void resetAllYokaiPP() {
    LOG_FUNCTION_EXECUTION("resetAllYokaiPP");
    for (int i = 0; i < gPartyCount; i++) {
        for (int j = 0; j < gParty[i].moveCount; j++) {
            gParty[i].moves[j].currentPP = gParty[i].moves[j].move.pp;
        }
    }
} 