#include "move_learning.h"
#include "text.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

// 기술 학습 시스템 초기화
void initMoveLearningSystem() {
    srand((unsigned int)time(NULL));
}

// 레벨업 시 새로운 기술을 배울 수 있는지 확인
bool canLearnNewMove(const Yokai* yokai) {
    // 레벨에 따른 기술 등급 확인
    for (int i = 0; i < yokai->learnableMoveCount; i++) {
        Move move = yokai->learnableMoves[i];
        
        // 이미 배운 기술인지 확인
        bool alreadyLearned = false;
        for (int j = 0; j < yokai->moveCount; j++) {
            if (strcmp(yokai->moves[j].move.name, move.name) == 0) {
                alreadyLearned = true;
                break;
            }
        }
        
        if (!alreadyLearned) {
            // 레벨 범위 확인
            switch (move.grade) {
                case MOVE_BASIC:
                    if (yokai->level >= 5 && yokai->level <= 10) return true;
                    break;
                case MOVE_MEDIUM:
                    if (yokai->level >= 10 && yokai->level <= 30) return true;
                    break;
                case MOVE_ADVANCED:
                    if (yokai->level >= 30 && yokai->level <= 50) return true;
                    break;
            }
        }
    }
    return false;
}

// 기술 학습 확률 계산 (레벨에 따라 10% 확률)
bool calculateLearningChance(const Yokai* yokai) {
    return (rand() % 100) < 100; // 10% 확률
}

// 기술 목록 출력
void printAvailableMoves(const Yokai* yokai) {
    printf("\n=== %s의 기술 목록 ===\n", yokai->name);
    for (int i = 0; i < yokai->moveCount; i++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
            yokai->moves[i].move.type == TYPE_EVIL_SPIRIT ? 12 :  // 빨강
            yokai->moves[i].move.type == TYPE_GHOST ? 11 :        // 하늘색
            yokai->moves[i].move.type == TYPE_MONSTER ? 14 :      // 노랑
            yokai->moves[i].move.type == TYPE_HUMAN ? 10 :        // 초록
            13);                                                   // 분홍
        
        printf("%d. %s", i + 1, yokai->moves[i].move.name);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // 흰색으로 복귀
        printf(" (공격력: %d, 명중률: %d%%, PP: %d/%d)\n", 
            yokai->moves[i].move.power,
            yokai->moves[i].move.accuracy,
            yokai->moves[i].currentPP,
            yokai->moves[i].move.pp
        );
    }
    printf("0. 뒤로가기\n\n");
}

// 새로운 기술 학습 시도
bool tryLearnNewMove(Yokai* yokai) {
    if (!canLearnNewMove(yokai)) {
        return false;
    }

    // 배울 수 있는 기술 목록 생성
    Move availableMoves[MAX_LEARNABLE_MOVES];
    int availableCount = 0;

    for (int i = 0; i < yokai->learnableMoveCount; i++) {
        Move move = yokai->learnableMoves[i];
        bool alreadyLearned = false;

        // 현재 배운 기술 목록에서 체크
        for (int j = 0; j < yokai->moveCount; j++) {
            if (strcmp(yokai->moves[j].move.name, move.name) == 0) {
                alreadyLearned = true;
                break;
            }
        }

        // 잊은 기술 목록에서 체크
        for (int j = 0; j < yokai->forgottenMoveCount; j++) {
            if (strcmp(yokai->forgottenMoves[j].name, move.name) == 0) {
                alreadyLearned = true;
                break;
            }
        }

        if (!alreadyLearned) {
            // 레벨 범위 확인
            switch (move.grade) {
                case MOVE_BASIC:
                    if (yokai->level >= 5 && yokai->level <= 10) {
                        availableMoves[availableCount++] = move;
                    }
                    break;
                case MOVE_MEDIUM:
                    if (yokai->level >= 10 && yokai->level <= 20) {
                        availableMoves[availableCount++] = move;
                    }
                    break;
                case MOVE_ADVANCED:
                    if (yokai->level >= 20 && yokai->level <= 30) {
                        availableMoves[availableCount++] = move;
                    }
                    break;
            }
        }
    }

    if (availableCount == 0) {
        return false;
    }

    while (1) {
        // 배울 수 있는 기술 목록 출력
        printText("\n=== 배울 수 있는 기술 목록 ===\n");
        for (int i = 0; i < availableCount; i++) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                availableMoves[i].type == TYPE_EVIL_SPIRIT ? 12 :  // 빨강
                availableMoves[i].type == TYPE_GHOST ? 11 :        // 하늘색
                availableMoves[i].type == TYPE_MONSTER ? 14 :      // 노랑
                availableMoves[i].type == TYPE_HUMAN ? 10 :        // 초록
                13);                                               // 분홍
            
            printf("%d. %s", i + 1, availableMoves[i].name);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // 흰색으로 복귀
            printf(" (공격력: %d, 명중률: %d%%, PP: %d)\n", 
                availableMoves[i].power,
                availableMoves[i].accuracy,
                availableMoves[i].pp
            );
        }
        printText("0. 기술을 배우지 않는다\n");

        // 사용자 선택
        int choice = getIntInput(0, availableCount);
        if (choice == 0) {
            printText("\n기술을 배우지 않기로 했습니다.\n");
            return false;
        }

        Move selectedMove = availableMoves[choice - 1];

        // 기술 습득 확률 계산
        if (calculateLearningChance(yokai)) {
            // 기술 습득 성공
            if (yokai->moveCount < MAX_MOVES) {
                // 빈 자리가 있는 경우
                yokai->moves[yokai->moveCount].move = selectedMove;
                yokai->moves[yokai->moveCount].currentPP = selectedMove.pp;
                yokai->moveCount++;
                char buffer[256];
                sprintf(buffer, "\n%s를 배웠습니다!\n", selectedMove.name);
                printText(buffer);
                return true;
            } else {
                // 기술 잊기 메뉴 표시
                while (1) {
                    printText("\n기술 슬롯이 가득 찼습니다. 어떤 기술을 잊으시겠습니까?\n");
                    printAvailableMoves(yokai);
                    
                    int forgetChoice = getIntInput(0, yokai->moveCount);
                    if (forgetChoice == 0) {
                        // 뒤로가기 선택 시 배울 수 있는 기술 목록으로 돌아감
                        break;
                    }
                    
                    char oldMoveName[YOKAI_NAME_MAX];
                    strcpy(oldMoveName, yokai->moves[forgetChoice - 1].move.name);
                    
                    if (forgetMove(yokai, forgetChoice - 1)) {
                        char buffer[256];
                        sprintf(buffer, "\n%s를 잊었습니다.\n", oldMoveName);
                        printText(buffer);
                        
                        // 새로운 기술 배우기
                        yokai->moves[yokai->moveCount].move = selectedMove;
                        yokai->moves[yokai->moveCount].currentPP = selectedMove.pp;
                        yokai->moveCount++;
                        
                        sprintf(buffer, "%s를 배웠습니다!\n", selectedMove.name);
                        printText(buffer);
                        return true;
                    }
                }
            }
        } else {
            printText("\n기술을 배우지 못했습니다.\n");
            return false;
        }
    }
    return false;
}

// 기술 잊기
bool forgetMove(Yokai* yokai, int moveIndex) {
    if (moveIndex < 0 || moveIndex >= yokai->moveCount) {
        return false;
    }

    // 잊은 기술 목록에 추가
    yokai->forgottenMoves[yokai->forgottenMoveCount++] = yokai->moves[moveIndex].move;

    // 기술 제거
    for (int i = moveIndex; i < yokai->moveCount - 1; i++) {
        yokai->moves[i] = yokai->moves[i + 1];
    }
    yokai->moveCount--;
    return true;
}

// 기술 학습 메뉴 표시
void showMoveLearningMenu(Yokai* yokai) {
    if (yokai->moveCount >= MAX_MOVES) {
        printText("\n새로운 기술을 배우기 위해서는 기존 기술 중 하나를 잊어야 합니다.\n");
        printText("어떤 기술을 잊으시겠습니까?\n");
        
        printAvailableMoves(yokai);
        
        int choice = getIntInput(1, yokai->moveCount);
        if (choice > 0) {
            if (forgetMove(yokai, choice - 1)) {
                printText("기술을 잊었습니다.\n");
            }
        }
    }
} 