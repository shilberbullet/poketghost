#include "move_learning.h"
#include "text.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include "logger.h"

// 기술 학습 시스템 초기화
void initMoveLearningSystem() {
    LOG_FUNCTION_EXECUTION("initMoveLearningSystem");
    srand((unsigned int)time(NULL));
}

// 레벨업 시 새로운 기술을 배울 수 있는지 확인
bool canLearnNewMove(const Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("canLearnNewMove");
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
        
        // 잊은 기술 목록에서도 체크
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
                    if (yokai->level >= 1) {
                        return true;
                    }
                    break;
                case MOVE_MEDIUM:
                    if (yokai->level >= 10) {
                        return true;
                    }
                    break;
                case MOVE_ADVANCED:
                    if (yokai->level >= 11) {  // 11레벨부터 고급기술 배우기 가능
                        return true;
                    }
                    break;
            }
        }
    }
    return false;
}

// 기술 학습 확률 계산 (레벨에 따라 10% 확률)
bool calculateLearningChance(const Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("calculateLearningChance");
    (void)yokai;
    return true;  // 항상 true를 반환하여 100% 확률로 기술 학습
}

// 기술 목록 출력
void printAvailableMoves(const Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("printAvailableMoves");
    char buffer[256];
    sprintf(buffer, "\n=== %s의 기술 목록 ===\n", yokai->name);
    printTextAndWait(buffer);
    
    for (int i = 0; i < yokai->moveCount; i++) {
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
            i + 1,
            colorCode,
            yokai->moves[i].move.name,
            colorCode,
            yokai->moves[i].move.power,
            yokai->moves[i].move.accuracy,
            yokai->moves[i].currentPP,
            yokai->moves[i].move.pp
        );
        printTextAndWait(buffer);
    }
    printf("0. 뒤로가기\n\n");
}

// 새로운 기술 학습 시도
bool tryLearnNewMove(Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("tryLearnNewMove");
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
                    if (yokai->level >= 1) {
                        availableMoves[availableCount++] = move;
                    }
                    break;
                case MOVE_MEDIUM:
                    if (yokai->level >= 10) {
                        availableMoves[availableCount++] = move;
                    }
                    break;
                case MOVE_ADVANCED:
                    if (yokai->level >= 11) {  // 11레벨부터 고급기술 배우기 가능
                        availableMoves[availableCount++] = move;
                    }
                    break;
            }
        }
    }

    if (availableCount == 0) {
        return false;
    }

    // 랜덤으로 1개의 기술만 선택
    int randomIndex = rand() % availableCount;
    Move selectedMove = availableMoves[randomIndex];

    while (1) {
        // 선택된 기술 출력
        printText("\n=== 배울 수 있는 기술 ===\n");
        const char* colorCode;
        switch (selectedMove.type) {
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
        
        // 기술 등급에 따른 색상 추가
        const char* gradeColor = "";
        const char* gradeText = "";
        switch (selectedMove.grade) {
            case MOVE_BASIC:
                gradeColor = "\033[37m";  // 흰색
                gradeText = "[초급]";
                break;
            case MOVE_MEDIUM:
                gradeColor = "\033[33m";  // 노란색
                gradeText = "[중급]";
                break;
            case MOVE_ADVANCED:
                gradeColor = "\033[35m";  // 보라색
                gradeText = "[고급]";
                break;
        }
        
        char buffer[256];
        sprintf(buffer, "1. %s%s%s\033[0m %s%s%s\033[0m (공격력: %d, 명중률: %d%%, PP: %d)\n", 
            colorCode,
            selectedMove.name,
            colorCode,
            gradeColor,
            gradeText,
            gradeColor,
            selectedMove.power,
            selectedMove.accuracy,
            selectedMove.pp
        );
        printText(buffer);
        printText("0. 기술을 배우지 않는다\n");

        // 사용자 선택
        int choice = getIntInput(0, 1);
        if (choice == 0) {
            // 기술을 배우지 않기로 했을 때, 해당 기술을 잊은 기술 목록에 추가
            yokai->forgottenMoves[yokai->forgottenMoveCount++] = selectedMove;
            char buffer[256];
            sprintf(buffer, "\n%s를 배우지 않았습니다. (나중에 다시 배울 수 있습니다)\n", selectedMove.name);
            printText(buffer);
            return false;
        }

        // 기술 습득 확률 계산
        if (calculateLearningChance(yokai)) {
            // 기술 습득 성공
            if (yokai->moveCount < MAX_MOVES) {
                // 빈 자리가 있는 경우
                yokai->moves[yokai->moveCount].move = selectedMove;
                yokai->moves[yokai->moveCount].currentPP = selectedMove.pp;
                yokai->moveCount++;
                
                // 고급기술 배움 시 특별 메시지
                char buffer[256];
                if (selectedMove.grade == MOVE_ADVANCED) {
                    sprintf(buffer, "\n🎉 %s가 고급기술 '%s'를 배웠습니다! 🎉\n", yokai->name, selectedMove.name);
                    printText(buffer);
                    printText("고급기술은 강력한 위력을 자랑합니다!\n");
                } else {
                    sprintf(buffer, "\n%s를 배웠습니다!\n", selectedMove.name);
                    printText(buffer);
                }
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
                        
                        // 고급기술 배움 시 특별 메시지
                        if (selectedMove.grade == MOVE_ADVANCED) {
                            sprintf(buffer, "🎉 %s가 고급기술 '%s'를 배웠습니다! 🎉\n", yokai->name, selectedMove.name);
                            printText(buffer);
                            printText("고급기술은 강력한 위력을 자랑합니다!\n");
                        } else {
                            sprintf(buffer, "%s를 배웠습니다!\n", selectedMove.name);
                            printText(buffer);
                        }
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
    LOG_FUNCTION_EXECUTION("forgetMove");
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
    LOG_FUNCTION_EXECUTION("showMoveLearningMenu");
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