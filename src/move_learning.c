#include "move_learning.h"  // 기술 학습 시스템 헤더 포함
#include "text.h"  // 텍스트 출력 함수 헤더 포함
#include "input.h"  // 입력 처리 함수 헤더 포함
#include <stdio.h>  // 표준 입출력 함수 헤더 포함
#include <stdlib.h>  // 표준 라이브러리 함수 헤더 포함
#include <time.h>  // 시간 관련 함수 헤더 포함
#include <string.h>  // 문자열 처리 함수 헤더 포함
#include <windows.h>  // Windows API 헤더 포함
#include "logger.h"  // 로깅 시스템 헤더 포함

// 기술 학습 시스템 초기화
void initMoveLearningSystem() {  // 기술 학습 시스템 초기화 함수 정의
    LOG_FUNCTION_EXECUTION("initMoveLearningSystem");  // 함수 실행 로그 기록
    srand((unsigned int)time(NULL));  // 랜덤 시드 초기화
}

// 레벨업 시 새로운 기술을 배울 수 있는지 확인
bool canLearnNewMove(const Yokai* yokai) {  // 새로운 기술 학습 가능 여부 확인 함수 정의
    LOG_FUNCTION_EXECUTION("canLearnNewMove");  // 함수 실행 로그 기록
    // 레벨에 따른 기술 등급 확인
    for (int i = 0; i < yokai->learnableMoveCount; i++) {  // 배울 수 있는 기술 목록 순회
        Move move = yokai->learnableMoves[i];  // 현재 확인할 기술 가져오기
        
        // 이미 배운 기술인지 확인
        bool alreadyLearned = false;  // 이미 배운 기술 여부 플래그 초기화
        for (int j = 0; j < yokai->moveCount; j++) {  // 현재 배운 기술 목록 순회
            if (strcmp(yokai->moves[j].move.name, move.name) == 0) {  // 기술명 비교
                alreadyLearned = true;  // 이미 배운 기술로 설정
                break;  // 루프 종료
            }
        }
        
        // 잊은 기술 목록에서도 체크
        for (int j = 0; j < yokai->forgottenMoveCount; j++) {  // 잊은 기술 목록 순회
            if (strcmp(yokai->forgottenMoves[j].name, move.name) == 0) {  // 기술명 비교
                alreadyLearned = true;  // 이미 배운 기술로 설정
                break;  // 루프 종료
            }
        }
        
        if (!alreadyLearned) {  // 아직 배우지 않은 기술인 경우
            // 레벨 범위 확인 - 수정된 조건
            switch (move.grade) {  // 기술 등급에 따른 조건 확인
                case MOVE_BASIC:  // 초급 기술인 경우
                    if (yokai->level >= 1) {  // 레벨 1 이상이면 배울 수 있음
                        return true;  // 학습 가능 반환
                    }
                    break;  // 케이스 종료
                case MOVE_MEDIUM:  // 중급 기술인 경우
                    if (yokai->level >= 10) {  // 레벨 10 이상이면 배울 수 있음
                        return true;  // 학습 가능 반환
                    }
                    break;  // 케이스 종료
                case MOVE_ADVANCED:  // 고급 기술인 경우
                    if (yokai->level >= 30) {  // 30레벨부터 고급기술 배우기 가능
                        return true;  // 학습 가능 반환
                    }
                    break;  // 케이스 종료
            }
        }
    }
    return false;  // 배울 수 있는 기술이 없음
}

// 기술 학습 확률 계산 (레벨에 따라 10% 확률)
bool calculateLearningChance(const Yokai* yokai) {  // 기술 학습 확률 계산 함수 정의
    LOG_FUNCTION_EXECUTION("calculateLearningChance");  // 함수 실행 로그 기록
    (void)yokai;  // 매개변수 사용하지 않음을 명시
    return true;  // 항상 true를 반환하여 100% 확률로 기술 학습
}

// 기술 목록 출력
void printAvailableMoves(const Yokai* yokai) {  // 사용 가능한 기술 목록 출력 함수 정의
    LOG_FUNCTION_EXECUTION("printAvailableMoves");  // 함수 실행 로그 기록
    char buffer[256];  // 출력용 버퍼 선언
    sprintf(buffer, "\n=== %s의 기술 목록 ===\n", yokai->name);  // 요괴 이름과 함께 제목 출력
    printTextAndWait(buffer);  // 텍스트 출력 및 대기
    
    for (int i = 0; i < yokai->moveCount; i++) {  // 현재 배운 기술 목록 순회
        const char* colorCode;  // 색상 코드 변수 선언
        switch (yokai->moves[i].move.type) {  // 기술 타입에 따른 색상 설정
            case TYPE_EVIL_SPIRIT:  // 악령 타입인 경우
                colorCode = "\033[31m";  // 빨간색
                break;  // 케이스 종료
            case TYPE_GHOST:  // 유령 타입인 경우
                colorCode = "\033[35m";  // 보라색
                break;  // 케이스 종료
            case TYPE_MONSTER:  // 괴물 타입인 경우
                colorCode = "\033[33m";  // 노란색
                break;  // 케이스 종료
            case TYPE_HUMAN:  // 인간 타입인 경우
                colorCode = "\033[36m";  // 청록색
                break;  // 케이스 종료
            case TYPE_ANIMAL:  // 동물 타입인 경우
                colorCode = "\033[32m";  // 초록색
                break;  // 케이스 종료
            default:  // 기본 타입인 경우
                colorCode = "\033[0m";   // 기본색
        }
        
        sprintf(buffer, "%d. %s%s%s\033[0m (공격력: %d, 명중률: %d%%, PP: %d/%d)\n",  // 기술 정보 포맷팅
            i + 1,  // 기술 번호
            colorCode,  // 색상 코드
            yokai->moves[i].move.name,  // 기술명
            colorCode,  // 색상 코드
            yokai->moves[i].move.power,  // 공격력
            yokai->moves[i].move.accuracy,  // 명중률
            yokai->moves[i].currentPP,  // 현재 PP
            yokai->moves[i].move.pp  // 최대 PP
        );
        printText(buffer);  // 포맷된 텍스트 출력
    }
    printText("0. 뒤로가기\n");  // 뒤로가기 옵션 출력
    printText("숫자를 입력하세요: ");  // 입력 안내 메시지 출력
}

// 새로운 기술 학습 시도
bool tryLearnNewMove(Yokai* yokai) {  // 새로운 기술 학습 시도 함수 정의
    LOG_FUNCTION_EXECUTION("tryLearnNewMove");  // 함수 실행 로그 기록
    if (!canLearnNewMove(yokai)) {  // 새로운 기술을 배울 수 없는 경우
        return false;  // 실패 반환
    }

    // 배울 수 있는 기술 목록 생성
    Move availableMoves[MAX_LEARNABLE_MOVES];  // 배울 수 있는 기술 배열 선언
    int availableCount = 0;  // 배울 수 있는 기술 개수 초기화

    for (int i = 0; i < yokai->learnableMoveCount; i++) {  // 배울 수 있는 기술 목록 순회
        Move move = yokai->learnableMoves[i];  // 현재 확인할 기술 가져오기
        bool alreadyLearned = false;  // 이미 배운 기술 여부 플래그 초기화

        // 현재 배운 기술 목록에서 체크
        for (int j = 0; j < yokai->moveCount; j++) {  // 현재 배운 기술 목록 순회
            if (strcmp(yokai->moves[j].move.name, move.name) == 0) {  // 기술명 비교
                alreadyLearned = true;  // 이미 배운 기술로 설정
                break;  // 루프 종료
            }
        }

        // 잊은 기술 목록에서 체크
        for (int j = 0; j < yokai->forgottenMoveCount; j++) {  // 잊은 기술 목록 순회
            if (strcmp(yokai->forgottenMoves[j].name, move.name) == 0) {  // 기술명 비교
                alreadyLearned = true;  // 이미 배운 기술로 설정
                break;  // 루프 종료
            }
        }

        if (!alreadyLearned) {  // 아직 배우지 않은 기술인 경우
            // 레벨 범위 확인 - 수정된 조건
            switch (move.grade) {  // 기술 등급에 따른 조건 확인
                case MOVE_BASIC:  // 초급 기술인 경우
                    if (yokai->level >= 1) {  // 레벨 1 이상이면 배울 수 있음
                        availableMoves[availableCount++] = move;  // 배울 수 있는 기술 목록에 추가
                        // 디버그 정보 추가
                        char debugBuffer[256];  // 디버그용 버퍼 선언
                        sprintf(debugBuffer, "[DEBUG] 추가된 기술: %s, 공격력: %d, 등급: %d",  // 디버그 메시지 포맷팅
                                move.name, move.power, move.grade);  // 기술 정보
                        LOG_FUNCTION_EXECUTION(debugBuffer);  // 디버그 로그 기록
                    }
                    break;  // 케이스 종료
                case MOVE_MEDIUM:  // 중급 기술인 경우
                    if (yokai->level >= 10) {  // 레벨 10 이상이면 배울 수 있음
                        availableMoves[availableCount++] = move;  // 배울 수 있는 기술 목록에 추가
                        // 디버그 정보 추가
                        char debugBuffer[256];  // 디버그용 버퍼 선언
                        sprintf(debugBuffer, "[DEBUG] 추가된 기술: %s, 공격력: %d, 등급: %d",  // 디버그 메시지 포맷팅
                                move.name, move.power, move.grade);  // 기술 정보
                        LOG_FUNCTION_EXECUTION(debugBuffer);  // 디버그 로그 기록
                    }
                    break;  // 케이스 종료
                case MOVE_ADVANCED:  // 고급 기술인 경우
                    if (yokai->level >= 30) {  // 30레벨부터 고급기술 배우기 가능
                        availableMoves[availableCount++] = move;  // 배울 수 있는 기술 목록에 추가
                        // 디버그 정보 추가
                        char debugBuffer[256];  // 디버그용 버퍼 선언
                        sprintf(debugBuffer, "[DEBUG] 추가된 기술: %s, 공격력: %d, 등급: %d",  // 디버그 메시지 포맷팅
                                move.name, move.power, move.grade);  // 기술 정보
                        LOG_FUNCTION_EXECUTION(debugBuffer);  // 디버그 로그 기록
                    }
                    break;  // 케이스 종료
            }
        }
    }

    if (availableCount == 0) {  // 배울 수 있는 기술이 없는 경우
        return false;  // 실패 반환
    }

    // 랜덤으로 1개의 기술만 선택
    int randomIndex = rand() % availableCount;  // 랜덤 인덱스 생성
    Move selectedMove = availableMoves[randomIndex];  // 선택된 기술 가져오기

    // 디버그 정보 추가 - 선택된 기술의 출처 확인
    char debugBuffer[256];  // 디버그용 버퍼 선언
    sprintf(debugBuffer, "[DEBUG] 선택된 기술: %s, 공격력: %d, 등급: %d",  // 디버그 메시지 포맷팅
            selectedMove.name, selectedMove.power, selectedMove.grade);  // 선택된 기술 정보
    LOG_FUNCTION_EXECUTION(debugBuffer);  // 디버그 로그 기록

    while (1) {  // 무한 루프 시작
        // 선택된 기술 출력
        printText("\n=== 배울 수 있는 기술 ===\n");  // 제목 출력
        const char* colorCode;  // 색상 코드 변수 선언
        switch (selectedMove.type) {  // 기술 타입에 따른 색상 설정
            case TYPE_EVIL_SPIRIT:  // 악령 타입인 경우
                colorCode = "\033[31m";  // 빨간색
                break;  // 케이스 종료
            case TYPE_GHOST:  // 유령 타입인 경우
                colorCode = "\033[35m";  // 보라색
                break;  // 케이스 종료
            case TYPE_MONSTER:  // 괴물 타입인 경우
                colorCode = "\033[33m";  // 노란색
                break;  // 케이스 종료
            case TYPE_HUMAN:  // 인간 타입인 경우
                colorCode = "\033[36m";  // 청록색
                break;  // 케이스 종료
            case TYPE_ANIMAL:  // 동물 타입인 경우
                colorCode = "\033[32m";  // 초록색
                break;  // 케이스 종료
            default:  // 기본 타입인 경우
                colorCode = "\033[0m";   // 기본색
        }
        
        // 기술 등급에 따른 색상 추가
        const char* gradeColor = "";  // 등급 색상 코드 변수 선언
        const char* gradeText = "";  // 등급 텍스트 변수 선언
        switch (selectedMove.grade) {  // 기술 등급에 따른 색상 및 텍스트 설정
            case MOVE_BASIC:  // 초급 기술인 경우
                gradeColor = "\033[37m";  // 흰색
                gradeText = "[초급]";  // 초급 텍스트
                break;  // 케이스 종료
            case MOVE_MEDIUM:  // 중급 기술인 경우
                gradeColor = "\033[33m";  // 노란색
                gradeText = "[중급]";  // 중급 텍스트
                break;  // 케이스 종료
            case MOVE_ADVANCED:  // 고급 기술인 경우
                gradeColor = "\033[35m";  // 보라색
                gradeText = "[고급]";  // 고급 텍스트
                break;  // 케이스 종료
        }
        
        char buffer[256];  // 출력용 버퍼 선언
        sprintf(buffer, "1. %s%s%s\033[0m %s%s%s\033[0m (공격력: %d, 명중률: %d%%, PP: %d)\n",  // 기술 정보 포맷팅
            colorCode,  // 타입 색상 코드
            selectedMove.name,  // 기술명
            colorCode,  // 타입 색상 코드
            gradeColor,  // 등급 색상 코드
            gradeText,  // 등급 텍스트
            gradeColor,  // 등급 색상 코드
            selectedMove.power,  // 공격력
            selectedMove.accuracy,  // 명중률
            selectedMove.pp  // PP
        );
        printText(buffer);  // 포맷된 텍스트 출력
        
        // 디버그 정보 추가
        char debugBuffer[256];  // 디버그용 버퍼 선언
        sprintf(debugBuffer, "[DEBUG] 기술명: %s, 실제 공격력: %d\n", selectedMove.name, selectedMove.power);  // 디버그 메시지 포맷팅
        printText(debugBuffer);  // 디버그 메시지 출력
        
        printText("0. 기술을 배우지 않는다\n");  // 배우지 않기 옵션 출력
        printText("숫자를 입력하세요: ");  // 입력 안내 메시지 출력

        // 사용자 선택
        int choice = getIntInput(0, 1);  // 사용자 입력 받기
        if (choice == 0) {  // 기술을 배우지 않기로 선택한 경우
            // 기술을 배우지 않기로 했을 때, 해당 기술을 잊은 기술 목록에 추가
            yokai->forgottenMoves[yokai->forgottenMoveCount++] = selectedMove;  // 잊은 기술 목록에 추가
            char buffer[256];  // 출력용 버퍼 선언
            sprintf(buffer, "\n%s를 배우지 않았습니다. \n", selectedMove.name);  // 메시지 포맷팅
            printText(buffer);  // 메시지 출력
            return false;  // 실패 반환
        }

        // 기술 습득 확률 계산
        if (calculateLearningChance(yokai)) {  // 기술 습득 성공한 경우
            // 기술 습득 성공
            if (yokai->moveCount < MAX_MOVES) {  // 빈 자리가 있는 경우
                // 빈 자리가 있는 경우
                yokai->moves[yokai->moveCount].move = selectedMove;  // 새로운 기술 추가
                yokai->moves[yokai->moveCount].currentPP = selectedMove.pp;  // 현재 PP 설정
                yokai->moveCount++;  // 기술 개수 증가
                
                // 고급기술 배움 시 특별 메시지
                char buffer[256];  // 출력용 버퍼 선언
                if (selectedMove.grade == MOVE_ADVANCED) {  // 고급 기술인 경우
                    sprintf(buffer, "\n🎉 %s가 고급기술 '%s'를 배웠습니다! 🎉\n", yokai->name, selectedMove.name);  // 특별 메시지 포맷팅
                    printText(buffer);  // 특별 메시지 출력
                    printText("고급기술은 강력한 위력을 자랑합니다!\n");  // 설명 메시지 출력
                } else {  // 일반 기술인 경우
                    sprintf(buffer, "\n%s를 배웠습니다!\n", selectedMove.name);  // 일반 메시지 포맷팅
                    printText(buffer);  // 일반 메시지 출력
                }
                return true;  // 성공 반환
            } else {  // 기술 슬롯이 가득 찬 경우
                // 기술 잊기 메뉴 표시
                while (1) {  // 무한 루프 시작
                    printText("\n기술 슬롯이 가득 찼습니다. 어떤 기술을 잊으시겠습니까?\n");  // 안내 메시지 출력
                    printAvailableMoves(yokai);  // 기술 목록 출력
                    
                    int forgetChoice = getIntInput(0, yokai->moveCount);  // 사용자 선택 받기
                    if (forgetChoice == 0) {  // 뒤로가기 선택한 경우
                        // 뒤로가기 선택 시 배울 수 있는 기술 목록으로 돌아감
                        break;  // 루프 종료
                    }
                    
                    char oldMoveName[YOKAI_NAME_MAX];  // 잊을 기술명 저장용 변수 선언
                    strcpy(oldMoveName, yokai->moves[forgetChoice - 1].move.name);  // 잊을 기술명 복사
                    
                    if (forgetMove(yokai, forgetChoice - 1)) {  // 기술 잊기 성공한 경우
                        char buffer[256];  // 출력용 버퍼 선언
                        sprintf(buffer, "\n%s를 잊었습니다.\n", oldMoveName);  // 메시지 포맷팅
                        printText(buffer);  // 메시지 출력
                        
                        // 새로운 기술 배우기
                        yokai->moves[yokai->moveCount].move = selectedMove;  // 새로운 기술 추가
                        yokai->moves[yokai->moveCount].currentPP = selectedMove.pp;  // 현재 PP 설정
                        yokai->moveCount++;  // 기술 개수 증가
                        
                        // 고급기술 배움 시 특별 메시지
                        if (selectedMove.grade == MOVE_ADVANCED) {  // 고급 기술인 경우
                            sprintf(buffer, "🎉 %s가 고급기술 '%s'를 배웠습니다! 🎉\n", yokai->name, selectedMove.name);  // 특별 메시지 포맷팅
                            printText(buffer);  // 특별 메시지 출력
                            printText("고급기술은 강력한 위력을 자랑합니다!\n");  // 설명 메시지 출력
                        } else {  // 일반 기술인 경우
                            sprintf(buffer, "%s를 배웠습니다!\n", selectedMove.name);  // 일반 메시지 포맷팅
                            printText(buffer);  // 일반 메시지 출력
                        }
                        return true;  // 성공 반환
                    }
                }
            }
        } else {  // 기술 습득 실패한 경우
            printText("\n기술을 배우지 못했습니다.\n");  // 실패 메시지 출력
            return false;  // 실패 반환
        }
    }
    return false;  // 기본 실패 반환
}

// 기술 잊기
bool forgetMove(Yokai* yokai, int moveIndex) {  // 기술 잊기 함수 정의
    LOG_FUNCTION_EXECUTION("forgetMove");  // 함수 실행 로그 기록
    if (moveIndex < 0 || moveIndex >= yokai->moveCount) {  // 유효하지 않은 인덱스인 경우
        return false;  // 실패 반환
    }

    // 잊은 기술 목록에 추가
    yokai->forgottenMoves[yokai->forgottenMoveCount++] = yokai->moves[moveIndex].move;  // 잊은 기술 목록에 추가

    // 기술 제거
    for (int i = moveIndex; i < yokai->moveCount - 1; i++) {  // 제거할 기술 이후의 기술들을 앞으로 이동
        yokai->moves[i] = yokai->moves[i + 1];  // 다음 기술을 현재 위치로 이동
    }
    yokai->moveCount--;  // 기술 개수 감소
    return true;  // 성공 반환
}

// 기술 학습 메뉴 표시
void showMoveLearningMenu(Yokai* yokai) {  // 기술 학습 메뉴 표시 함수 정의
    LOG_FUNCTION_EXECUTION("showMoveLearningMenu");  // 함수 실행 로그 기록
    if (yokai->moveCount >= MAX_MOVES) {  // 기술 슬롯이 가득 찬 경우
        printText("\n새로운 기술을 배우기 위해서는 기존 기술 중 하나를 잊어야 합니다.\n");  // 안내 메시지 출력
        printText("어떤 기술을 잊으시겠습니까?\n");  // 질문 메시지 출력
        
        printAvailableMoves(yokai);  // 기술 목록 출력
        
        int choice = getIntInput(1, yokai->moveCount);  // 사용자 선택 받기
        if (choice > 0) {  // 유효한 선택인 경우
            if (forgetMove(yokai, choice - 1)) {  // 기술 잊기 성공한 경우
                printText("기술을 잊었습니다.\n");  // 성공 메시지 출력
            }
        }
    }
} 