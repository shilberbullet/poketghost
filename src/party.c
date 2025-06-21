// 표준 입출력 함수를 위한 헤더
#include <stdio.h> // 표준 입출력 함수
// 표준 라이브러리 함수를 위한 헤더
#include <stdlib.h> // 표준 라이브러리 함수
// 문자열 처리 함수를 위한 헤더
#include <string.h> // 문자열 처리 함수
// 파티 관리 관련 함수와 구조체 정의
#include "party.h" // 파티 관리 헤더
// 요괴 관련 함수
#include "yokai.h" // 요괴 헤더
// 기술 관련 함수
#include "move.h" // 기술 헤더
// 텍스트 출력 관련 함수
#include "text.h" // 텍스트 출력 헤더
// 입력 처리 관련 함수
#include "input.h" // 입력 처리 헤더
// 게임 관련 함수
#include "game.h" // 게임 헤더
// 경험치 시스템 관련 함수
#include "exp_system.h" // 경험치 시스템 헤더
// 설정 관련 함수
#include "settings.h" // 설정 헤더
// 게임 상태 관련 함수
#include "../core/state.h" // 게임 상태 헤더
// 윈도우 API를 위한 헤더
#include <windows.h> // 윈도우 API
// HP 시스템 관련 함수
#include "hp_system.h" // HP 시스템 헤더
// 로거 관련 함수
#include "logger.h" // 로거 헤더
// 대화 시스템 관련 함수
#include "dialogue.h" // 대화 시스템 헤더
// 요괴 설명 최대 길이 정의
#ifndef YOKAI_DESC_MAX
#define YOKAI_DESC_MAX 256 // 요괴 설명 최대 길이
#endif

// HP 바 길이 정의
#define HP_BAR_LENGTH 20  // HP 바 길이 정의
// 잊은 기술 최대 개수 정의
#define MAX_FORGOTTEN_MOVES 10  // 잊은 기술 최대 개수

// 랜덤 기술 할당 함수 선언
void assignRandomMoves(Yokai* y, int level); // 요괴에 랜덤 기술을 할당하는 함수 선언

// 파티 요괴 배열과 개수는 state 모듈에서 관리

// 파티 초기화 함수
void initParty() { // 파티 초기화 함수 시작
    LOG_FUNCTION_EXECUTION("initParty"); // 함수 실행 로그 기록
    gPartyCount = 0; // 파티 요괴 수를 0으로 초기화
    // 도깨비 요괴를 파일에서 불러와 추가
    Yokai* dokkaebi = findYokaiByName("도깨비"); // 도깨비 요괴를 이름으로 찾기
    if (dokkaebi) { // 도깨비 요괴를 찾았으면
        memset(&gParty[0], 0, sizeof(Yokai));  // 구조체 전체를 0으로 초기화
        gParty[0] = *dokkaebi;                 // 기본 정보 복사
        gParty[0].level = 5;                   // 초기 레벨 5로 설정
        gParty[0].currentHP = calculateHP(&gParty[0]);  // HP 초기화
        
        // 도감 설명 명시적 복사
        strncpy(gParty[0].desc, dokkaebi->desc, YOKAI_DESC_MAX - 1); // 도감 설명 복사
        gParty[0].desc[YOKAI_DESC_MAX - 1] = '\0'; // 문자열 끝에 null 문자 추가
        
        // learnableMoves 복사 (레벨업 시 중급/고급 기술 배우기 위해 원본 유지)
        gParty[0].learnableMoveCount = dokkaebi->learnableMoveCount; // 배울 수 있는 기술 수 복사
        for (int i = 0; i < dokkaebi->learnableMoveCount; i++) { // 모든 배울 수 있는 기술에 대해
            gParty[0].learnableMoves[i] = dokkaebi->learnableMoves[i]; // 기술 정보 복사
        }
        
        // 도깨비 초기 기술: 하드코딩된 초급 기술 4개만 할당
        Move dokkaebiBasicMoves[] = { // 도깨비 초급 기술 배열 정의
            {"불씨던지기", TYPE_MONSTER, 25, 90, 20, "작은 불씨를 던져 화상을 입힌다", MOVE_BASIC}, // 불씨던지기 기술
            {"돌던지기", TYPE_HUMAN, 25, 95, 25, "돌을 던져 적을 공격한다", MOVE_BASIC}, // 돌던지기 기술
            {"진흙던지기", TYPE_HUMAN, 20, 100, 25, "진흙을 뿌려 시야를 방해한다", MOVE_BASIC}, // 진흙던지기 기술
            {"호미찌르기", TYPE_HUMAN, 25, 100, 20, "호미날로 적을 강하게 찌른다", MOVE_BASIC}, // 호미찌르기 기술
            {"몸통박치기", TYPE_MONSTER, 25, 100, 20, "몸 전체로 부딪혀 공격한다", MOVE_BASIC}, // 몸통박치기 기술
            {"소리지르기", TYPE_HUMAN, 20, 100, 30, "소리를 질러 적을 공격한다", MOVE_BASIC}, // 소리지르기 기술
            {"짖기", TYPE_ANIMAL, 15, 100, 30, "소리로 적을 공격한다", MOVE_BASIC}, // 짖기 기술
            {"할퀴기", TYPE_ANIMAL, 25, 100, 20, "날카로운 발톱으로 할퀴어 공격한다", MOVE_BASIC} // 할퀴기 기술
        };
        int dokkaebiBasicCount = 8; // 초급 기술 총 개수
        
        // 초급 기술 중 랜덤하게 4개 선택
        gParty[0].moveCount = 0; // 기술 수 초기화
        for (int i = 0; i < 4; i++) { // 4개의 기술을 선택
            int randomIndex = rand() % dokkaebiBasicCount; // 랜덤 인덱스 생성
            gParty[0].moves[i].move = dokkaebiBasicMoves[randomIndex]; // 선택된 기술을 파티에 추가
            gParty[0].moves[i].currentPP = dokkaebiBasicMoves[randomIndex].pp; // 현재 PP 설정
            gParty[0].moveCount++; // 기술 수 증가
            
            // 중복 방지: 선택된 기술을 배열 끝으로 이동
            Move temp = dokkaebiBasicMoves[randomIndex]; // 선택된 기술을 임시 변수에 저장
            dokkaebiBasicMoves[randomIndex] = dokkaebiBasicMoves[dokkaebiBasicCount - 1]; // 끝의 기술을 선택된 위치로 이동
            dokkaebiBasicMoves[dokkaebiBasicCount - 1] = temp; // 선택된 기술을 끝으로 이동
            dokkaebiBasicCount--; // 사용 가능한 기술 수 감소
        }
        
        // 잊은 기술 목록 초기화 (도깨비는 생성 시 잊은 기술을 추가하지 않음)
        gParty[0].forgottenMoveCount = 0; // 잊은 기술 수 초기화
        
        gParty[0].learnedMoveCount = gParty[0].moveCount; // 배운 기술 수를 현재 기술 수로 설정
        for(int i=0; i<gParty[0].learnedMoveCount; ++i) { // 모든 배운 기술에 대해
            gParty[0].learnedMoves[i] = gParty[0].moves[i].move; // 배운 기술 목록에 추가
        }
        
        gPartyCount = 1; // 파티 요괴 수를 1로 설정
    }
}

// 요괴 성불 처리 함수
void releaseYokai(int index) { // 요괴 성불 처리 함수 시작
    LOG_FUNCTION_EXECUTION("releaseYokai"); // 함수 실행 로그 기록
    if (index < 0 || index >= gPartyCount) return; // 인덱스가 유효하지 않으면 함수 종료
    if (index == 0) { // 도깨비(인덱스 0)인 경우
        printText("\n도깨비는 성불시킬 수 없습니다.\n"); // 오류 메시지 출력
        startDialogue(40); // 대화 시작
        return; // 함수 종료
    }
    
    gPartyCount--; // 파티 요괴 수 감소
}

// 새로운 요괴를 잡았을 때 파티가 가득 찼을 경우의 처리
int handleFullParty(const Yokai* newYokai) { // 파티 가득참 처리 함수 시작
    LOG_FUNCTION_EXECUTION("handleFullParty"); // 함수 실행 로그 기록
    if (gPartyCount < MAX_PARTY_SIZE) { // 파티가 가득 차지 않았으면
        return addYokaiToParty(newYokai); // 요괴를 파티에 추가
    }

    printText("\n동료 요괴가 가득 찼습니다!\n"); // 파티 가득참 메시지 출력
    printText("1. 잡은 요괴를 성불시킨다\n"); // 선택지 1 출력
    printText("2. 동료 요괴를 성불시킨다\n"); // 선택지 2 출력
    printText("숫자를 입력해주세요: "); // 입력 요청

    int choice = getIntInput(); // 사용자 선택 입력 받기
    if (choice == 1) { // 잡은 요괴를 성불시키는 경우
        printText("\n잡은 요괴를 성불시켰습니다."); // 성불 메시지 출력
        fastSleep(500); // 0.5초 대기
        return 0; // 0 반환 (요괴 추가 안함)
    } else if (choice == 2) { // 동료 요괴를 성불시키는 경우
        while (1) { // 무한 루프
            printText("\n성불 시킬 동료 요괴를 선택하세요:\n"); // 요괴 선택 안내
            for (int i = 0; i < gPartyCount; i++) { // 모든 파티 요괴에 대해
                char buffer[256]; // 텍스트 버퍼
                float maxHP = calculateHP(&gParty[i]); // 최대 HP 계산
                // 타입에 따른 색상 설정
                const char* colorCode; // 색상 코드 변수
                switch (gParty[i].type) { // 요괴 타입에 따라
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
                const char* statusText = ""; // 상태 텍스트 변수
                const char* statusColor = ""; // 상태 색상 변수
                switch (gParty[i].status) { // 요괴 상태에 따라
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
                    i+1, // 요괴 번호
                    gParty[i].name, // 요괴 이름
                    colorCode, // 타입 색상
                    typeToString(gParty[i].type), // 타입 문자열
                    gParty[i].level, // 레벨
                    gParty[i].currentHP, // 현재 HP
                    maxHP, // 최대 HP
                    statusColor, // 상태 색상
                    statusText, // 상태 텍스트
                    gParty[i].attack, // 공격력
                    gParty[i].defense); // 방어력
                printText(buffer); // 요괴 정보 출력
            }
            printText("0. 뒤로 돌아간다\n"); // 뒤로가기 옵션 추가
            printText("숫자를 입력해주세요: "); // 입력 요청 추가

            int yokaiChoice = getIntInput(); // 요괴 선택 입력 받기
            if (yokaiChoice == 0) { // 뒤로가기 선택
                return handleFullParty(newYokai); // 다시 선택 메뉴로
            }
            yokaiChoice--; // 0-based index로 변환

            if (yokaiChoice >= 0 && yokaiChoice < gPartyCount) { // 유효한 선택인 경우
                if (yokaiChoice == 0) { // 도깨비(인덱스 0)인 경우
                    printText("\n도깨비는 성불시킬 수 없습니다. 다른 요괴를 선택하세요."); // 오류 메시지 출력
                    startDialogue(40); // 대화 시작
                    continue; // 다시 선택하도록 continue
                }
                char buffer[128]; // 텍스트 버퍼
                sprintf(buffer, "\n%s를 성불시켰습니다.", gParty[yokaiChoice].name); // 성불 메시지 생성
                printText(buffer); // 성불 메시지 출력
                // 선택된 요괴를 성불시키고 새로운 요괴 추가
                releaseYokai(yokaiChoice); // 요괴 성불 처리
                return addYokaiToParty(newYokai); // 새로운 요괴 추가
            } else { // 잘못된 선택인 경우
                printText("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
                continue;  // 다시 선택하도록 continue 추가
            }
        }
    } else { // 잘못된 선택인 경우
        printText("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
        return handleFullParty(newYokai); // 다시 선택 메뉴로
    }
    return 0; // 기본 반환값
}

// 파티에 새로운 요괴 추가 함수
int addYokaiToParty(const Yokai* yokai) { // 파티에 요괴 추가 함수 시작
    LOG_FUNCTION_EXECUTION("addYokaiToParty"); // 함수 실행 로그 기록
    // 성불된 요괴의 자리를 찾아서 재사용
    int releasedSlot = -1; // 성불된 슬롯 인덱스 초기화
    for (int i = 0; i < gPartyCount; i++) { // 모든 파티 요괴에 대해
        if (gParty[i].status == YOKAI_RELEASED) { // 성불된 요괴인 경우
            releasedSlot = i; // 성불된 슬롯 인덱스 저장
            break; // 루프 종료
        }
    }

    if (releasedSlot != -1) { // 성불된 슬롯이 있으면
        // 성불된 요괴의 자리에 새로운 요괴 추가
        gParty[releasedSlot] = *yokai; // 요괴 정보 복사
        gParty[releasedSlot].status = YOKAI_NORMAL;  // 상태를 정상으로 설정
        
        // 도감 설명 명시적 복사
        strncpy(gParty[releasedSlot].desc, yokai->desc, YOKAI_DESC_MAX - 1); // 도감 설명 복사
        gParty[releasedSlot].desc[YOKAI_DESC_MAX - 1] = '\0'; // 문자열 끝에 null 문자 추가
        
        // learnableMoves 복사
        gParty[releasedSlot].learnableMoveCount = yokai->learnableMoveCount; // 배울 수 있는 기술 수 복사
        for (int i = 0; i < yokai->learnableMoveCount; i++) { // 모든 배울 수 있는 기술에 대해
            gParty[releasedSlot].learnableMoves[i] = yokai->learnableMoves[i]; // 기술 정보 복사
        }

        // 현재 가지고 있는 기술과 PP 복사
        gParty[releasedSlot].moveCount = yokai->moveCount; // 기술 수 복사
        for (int i = 0; i < yokai->moveCount; i++) { // 모든 기술에 대해
            gParty[releasedSlot].moves[i].move = yokai->moves[i].move; // 기술 정보 복사
            gParty[releasedSlot].moves[i].currentPP = yokai->moves[i].currentPP; // 현재 PP 복사
            // 배운 기술 목록에도 추가
            gParty[releasedSlot].learnedMoves[gParty[releasedSlot].learnedMoveCount++] = yokai->moves[i].move; // 배운 기술 목록에 추가
        }
        return 1; // 성공 반환
    }

    if (gPartyCount >= MAX_PARTY_SIZE) { // 파티가 가득 찬 경우
        return handleFullParty(yokai); // 파티 가득참 처리 함수 호출
    }
    
    // 요괴 정보 직접 복사
    gParty[gPartyCount] = *yokai;  // 기본 정보 복사
    gParty[gPartyCount].status = YOKAI_NORMAL;  // 상태를 정상으로 설정
    
    // 도감 설명 명시적 복사
    strncpy(gParty[gPartyCount].desc, yokai->desc, YOKAI_DESC_MAX - 1); // 도감 설명 복사
    gParty[gPartyCount].desc[YOKAI_DESC_MAX - 1] = '\0'; // 문자열 끝에 null 문자 추가
    
    // learnableMoves 복사
    gParty[gPartyCount].learnableMoveCount = yokai->learnableMoveCount; // 배울 수 있는 기술 수 복사
    for (int i = 0; i < yokai->learnableMoveCount; i++) { // 모든 배울 수 있는 기술에 대해
        gParty[gPartyCount].learnableMoves[i] = yokai->learnableMoves[i]; // 기술 정보 복사
    }

    // 현재 가지고 있는 기술과 PP 복사
    gParty[gPartyCount].moveCount = yokai->moveCount; // 기술 수 복사
    for (int i = 0; i < yokai->moveCount; i++) { // 모든 기술에 대해
        gParty[gPartyCount].moves[i].move = yokai->moves[i].move; // 기술 정보 복사
        gParty[gPartyCount].moves[i].currentPP = yokai->moves[i].currentPP; // 현재 PP 복사
        // 배운 기술 목록에도 추가
        gParty[gPartyCount].learnedMoves[gParty[gPartyCount].learnedMoveCount++] = yokai->moves[i].move; // 배운 기술 목록에 추가
    }

    gPartyCount++; // 파티 요괴 수 증가
    return 1; // 성공 반환
}

// 요괴 정보 서브메뉴 표시 함수
void showYokaiSubMenu(const Yokai* yokai) { // 요괴 정보 서브메뉴 표시 함수 시작
    LOG_FUNCTION_EXECUTION("showYokaiSubMenu"); // 함수 실행 로그 기록
    while (1) { // 무한 루프
        char buffer[256]; // 텍스트 버퍼
        sprintf(buffer, "\n=== 요괴 정보 ===\n"); // 제목 생성
        sprintf(buffer + strlen(buffer), "1. 종족값 보기\n"); // 메뉴 1 추가
        sprintf(buffer + strlen(buffer), "2. 도감 설명 보기\n"); // 메뉴 2 추가
        sprintf(buffer + strlen(buffer), "3. HP를 본다\n"); // 메뉴 3 추가
        sprintf(buffer + strlen(buffer), "4. 경험치 보기\n"); // 메뉴 4 추가
        sprintf(buffer + strlen(buffer), "5. 기술 목록 보기\n"); // 메뉴 5 추가
        sprintf(buffer + strlen(buffer), "6. 요괴 인벤토리 보기\n"); // 메뉴 6 추가
        sprintf(buffer + strlen(buffer), "7. 동료요괴를 성불시킨다\n"); // 메뉴 7 추가
        if (gameSettings.debugMode) { // 디버그 모드인 경우
            sprintf(buffer + strlen(buffer), "8. 배울 수 있는 기술 목록 보기\n"); // 디버그 메뉴 8 추가
            sprintf(buffer + strlen(buffer), "9. 잊은 기술 보기\n"); // 디버그 메뉴 9 추가
            sprintf(buffer + strlen(buffer), "10. 뒤로 가기\n"); // 디버그 메뉴 10 추가
        } else { // 일반 모드인 경우
            sprintf(buffer + strlen(buffer), "8. 뒤로 가기\n"); // 일반 메뉴 8 추가
        }
        sprintf(buffer + strlen(buffer), "\n숫자를 입력해주세요: "); // 입력 요청 추가
        printText(buffer); // 메뉴 출력
        
        int choice = getIntInput(); // 사용자 선택 입력 받기
        if ((!gameSettings.debugMode && choice == 8) || (gameSettings.debugMode && choice == 10)) { // 뒤로가기 선택
            return; // 함수 종료
        }
        
        switch (choice) { // 선택에 따른 처리
            case 1: {  // 종족값 보기
                char buffer[512]; // 텍스트 버퍼
                const char* colorCode; // 색상 코드 변수
                switch (yokai->type) { // 요괴 타입에 따라
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
                sprintf(buffer, "\n=== %s의 종족값 ===\n", yokai->name); // 제목 생성
                sprintf(buffer + strlen(buffer), "공격력: %d\n", yokai->attack); // 공격력 정보 추가
                sprintf(buffer + strlen(buffer), "방어력: %d\n", yokai->defense); // 방어력 정보 추가
                sprintf(buffer + strlen(buffer), "체력: %d\n", yokai->stamina); // 체력 정보 추가
                sprintf(buffer + strlen(buffer), "스피드: %d\n", yokai->speed); // 스피드 정보 추가
                sprintf(buffer + strlen(buffer), "상성: %s%s\033[0m\n", colorCode, typeNames[yokai->type]); // 상성 정보 추가
                printText(buffer); // 종족값 정보 출력
                printText("\n엔터를 눌러 돌아가기..."); // 안내 메시지 출력
                int c; // 문자 변수
                while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
                clearInputBuffer(); // 콘솔 입력 버퍼 비우기
                break; // case 1 종료
            }
            case 2: {  // 도감 설명 보기
                char buffer[256]; // 텍스트 버퍼
                sprintf(buffer, "\n=== %s의 도감 설명 ===\n", yokai->name); // 제목 생성
                sprintf(buffer + strlen(buffer), "%s\n", yokai->desc); // 도감 설명 추가
                printText(buffer); // 도감 설명 출력
                printText("\n엔터를 눌러 돌아가기..."); // 안내 메시지 출력
                int c; // 문자 변수
                while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
                clearInputBuffer(); // 콘솔 입력 버퍼 비우기
                break; // case 2 종료
            }
            case 3: {  // HP 보기
                char buffer[256]; // 텍스트 버퍼
                float maxHP = calculateHP(yokai); // 최대 HP 계산
                float hpPercentage = (yokai->currentHP / maxHP) * 100.0f; // HP 백분율 계산
                int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH); // HP 바 채워진 길이 계산
                
                sprintf(buffer, "\n=== %s의 HP ===\n", yokai->name); // 제목 생성
                sprintf(buffer + strlen(buffer), "현재 HP: %.0f/%.0f\n", yokai->currentHP, maxHP); // HP 정보 추가
                // HP 바 시각화
                strcat(buffer, "HP["); // HP 바 시작 추가
                if (hpPercentage <= 20.0f) { // HP가 20% 이하인 경우
                    strcat(buffer, "\033[31m"); // 빨간색
                } else if (hpPercentage <= 50.0f) { // HP가 50% 이하인 경우
                    strcat(buffer, "\033[33m"); // 노란색
                } else { // HP가 50% 초과인 경우
                    strcat(buffer, "\033[1;32m"); // 초록색
                }
                for (int i = 0; i < HP_BAR_LENGTH; i++) { // HP 바 길이만큼 반복
                    if (i < filledLength) { // 채워진 부분인 경우
                        strcat(buffer, "█"); // 채워진 블록 추가
                    } else { // 빈 부분인 경우
                        strcat(buffer, "░"); // 빈 블록 추가
                    }
                }
                strcat(buffer, "\033[0m]"); // 색상 초기화 및 HP 바 종료
                printText(buffer); // HP 정보 출력
                printText("\n엔터를 눌러 돌아가기..."); // 안내 메시지 출력
                int c; // 문자 변수
                while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
                clearInputBuffer(); // 콘솔 입력 버퍼 비우기
                break; // case 3 종료
            }
            case 4: {  // 경험치 보기
                char buffer[256]; // 텍스트 버퍼
                int requiredExp = calculateRequiredExp(yokai->level); // 다음 레벨까지 필요한 경험치 계산
                sprintf(buffer, "\n=== %s의 경험치 ===\n", yokai->name); // 제목 생성
                sprintf(buffer + strlen(buffer), "현재 경험치: %d\n", yokai->exp); // 현재 경험치 정보 추가
                sprintf(buffer + strlen(buffer), "다음 레벨까지: %d\n", requiredExp - yokai->exp); // 다음 레벨까지 필요한 경험치 정보 추가
                printText(buffer); // 경험치 정보 출력
                printText("\n엔터를 눌러 돌아가기..."); // 안내 메시지 출력
                int c; // 문자 변수
                while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
                clearInputBuffer(); // 콘솔 입력 버퍼 비우기
                break; // case 4 종료
            }
            case 5: {  // 기술 목록 보기
                char buffer[1024]; // 텍스트 버퍼
                sprintf(buffer, "\n=== %s의 기술 목록 ===\n", yokai->name); // 제목 생성
                for (int i = 0; i < yokai->moveCount; i++) { // 모든 기술에 대해
                    char* typeColor = ""; // 타입 색상 변수
                    switch (yokai->moves[i].move.type) { // 기술 타입에 따라
                        case TYPE_EVIL_SPIRIT: typeColor = "\033[31m"; break; // 빨간색
                        case TYPE_GHOST: typeColor = "\033[35m"; break;      // 보라색
                        case TYPE_MONSTER: typeColor = "\033[33m"; break;    // 노란색
                        case TYPE_HUMAN: typeColor = "\033[36m"; break;      // 청록색
                        case TYPE_ANIMAL: typeColor = "\033[32m"; break;     // 초록색
                        default: typeColor = "\033[0m"; break;               // 기본색
                    }
                    sprintf(buffer + strlen(buffer), "%d. %s%s\033[0m (PP: %d/%d, 공격력: %d, 명중률: %d%%)\n", 
                        i + 1, // 기술 번호
                        typeColor, // 타입 색상
                        yokai->moves[i].move.name, // 기술 이름
                        yokai->moves[i].currentPP, // 현재 PP
                        yokai->moves[i].move.pp, // 최대 PP
                        yokai->moves[i].move.power, // 공격력
                        yokai->moves[i].move.accuracy); // 명중률
                }
                printText(buffer); // 기술 목록 출력
                printText("\n엔터를 눌러 돌아가기..."); // 안내 메시지 출력
                int c; // 문자 변수
                while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
                clearInputBuffer(); // 콘솔 입력 버퍼 비우기
                break; // case 5 종료
            }
            case 6: {  // 요괴 인벤토리 보기
                char buffer[1024]; // 텍스트 버퍼
                sprintf(buffer, "\n=== %s의 인벤토리 ===\n", yokai->name); // 제목 생성
                if (yokai->yokaiInventoryCount == 0) { // 인벤토리가 비어있는 경우
                    sprintf(buffer + strlen(buffer), "인벤토리가 비어있습니다.\n"); // 빈 인벤토리 메시지 추가
                } else { // 인벤토리에 아이템이 있는 경우
                    for (int i = 0; i < yokai->yokaiInventoryCount; i++) { // 모든 아이템에 대해
                        sprintf(buffer + strlen(buffer), "%d. %s (수량: %d)\n", 
                            i + 1, // 아이템 번호
                            yokai->yokaiInventory[i].item.name, // 아이템 이름
                            yokai->yokaiInventory[i].count); // 아이템 수량
                    }
                }
                printText(buffer); // 인벤토리 정보 출력
                printText("\n엔터를 눌러 돌아가기..."); // 안내 메시지 출력
                int c; // 문자 변수
                while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
                clearInputBuffer(); // 콘솔 입력 버퍼 비우기
                break; // case 6 종료
            }
            case 7: {  // 동료요괴를 성불시킨다
                int idx = -1; // 요괴 인덱스 초기화
                // 파티 배열에서 해당 요괴의 인덱스 찾기
                for (int i = 0; i < gPartyCount; i++) { // 모든 파티 요괴에 대해
                    if (&gParty[i] == yokai) { // 요괴 포인터가 일치하는 경우
                        idx = i; // 인덱스 저장
                        break; // 루프 종료
                    }
                }
                if (idx == 0) { // 도깨비(인덱스 0)인 경우
                    printText("\n도깨비는 성불시킬 수 없습니다.\n"); // 오류 메시지 출력
                    startDialogue(40); // 대화 시작
                } else if (idx > 0 && idx < gPartyCount) { // 유효한 요괴 인덱스인 경우
                    releaseYokai(idx); // 요괴 성불 처리
                    printText("\n성불이 완료되었습니다. 목록에서 사라집니다.\n"); // 성불 완료 메시지 출력
                    return; // 성불 후 메뉴 종료
                } else { // 요괴를 찾을 수 없는 경우
                    printText("\n오류: 요괴 인덱스를 찾을 수 없습니다.\n"); // 오류 메시지 출력
                }
                break; // case 7 종료
            }
            case 8: {  // 배울 수 있는 기술 목록
                sprintf(buffer, "%s의 배울 수 있는 기술 목록:\n\n", yokai->name); // 제목 생성
                for (int i = 0; i < yokai->learnableMoveCount; i++) { // 모든 배울 수 있는 기술에 대해
                    char* typeColor = ""; // 타입 색상 변수
                    switch (yokai->learnableMoves[i].type) { // 기술 타입에 따라
                        case TYPE_EVIL_SPIRIT: typeColor = "\033[31m"; break; // 빨간색
                        case TYPE_GHOST: typeColor = "\033[35m"; break;      // 보라색
                        case TYPE_MONSTER: typeColor = "\033[33m"; break;    // 노란색
                        case TYPE_HUMAN: typeColor = "\033[36m"; break;      // 청록색
                        case TYPE_ANIMAL: typeColor = "\033[32m"; break;     // 초록색
                        default: typeColor = "\033[0m"; break;               // 기본색
                    }
                    sprintf(buffer + strlen(buffer), "%d. %s%s\033[0m (PP: %d/%d, 공격력: %d, 명중률: %d%%)\n", 
                        i + 1, // 기술 번호
                        typeColor, // 타입 색상
                        yokai->learnableMoves[i].name, // 기술 이름
                        yokai->learnableMoves[i].pp, // PP
                        yokai->learnableMoves[i].pp, // 최대 PP
                        yokai->learnableMoves[i].power, // 공격력
                        yokai->learnableMoves[i].accuracy); // 명중률
                }
                printText(buffer); // 배울 수 있는 기술 목록 출력
                printText("\n엔터를 눌러 돌아가기..."); // 안내 메시지 출력
                int c; // 문자 변수
                while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
                clearInputBuffer(); // 콘솔 입력 버퍼 비우기
                break; // case 8 종료
            }
            case 9: {  // 잊은 기술 목록
                sprintf(buffer, "%s의 잊은 기술 목록:\n\n", yokai->name); // 제목 생성
                for (int i = 0; i < yokai->forgottenMoveCount; i++) { // 모든 잊은 기술에 대해
                    char* typeColor = ""; // 타입 색상 변수
                    switch (yokai->forgottenMoves[i].type) { // 기술 타입에 따라
                        case TYPE_EVIL_SPIRIT: typeColor = "\033[31m"; break; // 빨간색
                        case TYPE_GHOST: typeColor = "\033[35m"; break;      // 보라색
                        case TYPE_MONSTER: typeColor = "\033[33m"; break;    // 노란색
                        case TYPE_HUMAN: typeColor = "\033[36m"; break;      // 청록색
                        case TYPE_ANIMAL: typeColor = "\033[32m"; break;     // 초록색
                        default: typeColor = "\033[0m"; break;               // 기본색
                    }
                    sprintf(buffer + strlen(buffer), "%d. %s%s\033[0m (PP: %d/%d, 공격력: %d, 명중률: %d%%)\n", 
                        i + 1, // 기술 번호
                        typeColor, // 타입 색상
                        yokai->forgottenMoves[i].name, // 기술 이름
                        yokai->forgottenMoves[i].pp, // PP
                        yokai->forgottenMoves[i].pp, // 최대 PP
                        yokai->forgottenMoves[i].power, // 공격력
                        yokai->forgottenMoves[i].accuracy); // 명중률
                }
                printText(buffer); // 잊은 기술 목록 출력
                printText("\n엔터를 눌러 돌아가기..."); // 안내 메시지 출력
                int c; // 문자 변수
                while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
                clearInputBuffer(); // 콘솔 입력 버퍼 비우기
                break; // case 9 종료
            }
            default: // 잘못된 선택인 경우
                printText("\n잘못된 선택입니다."); // 오류 메시지 출력
                break; // default 종료
        }
    }
}

// 파티 요괴 목록 출력 함수
void printParty() { // 파티 요괴 목록 출력 함수 시작
    LOG_FUNCTION_EXECUTION("printParty"); // 함수 실행 로그 기록
    char buffer[256]; // 텍스트 버퍼
    sprintf(buffer, "\n=== 동료 요괴 목록 ===\n"); // 제목 생성
    for (int i = 0; i < gPartyCount; i++) { // 모든 파티 요괴에 대해
        if (gParty[i].status != YOKAI_RELEASED) { // 성불되지 않은 요괴인 경우
            sprintf(buffer + strlen(buffer), "%d. %s Lv.%d\n", 
                i + 1, gParty[i].name, gParty[i].level);
        }
    }
    sprintf(buffer + strlen(buffer), "0. 뒤로 가기\n\n"); // 뒤로가기 옵션 추가
    sprintf(buffer + strlen(buffer), "숫자를 입력해주세요: "); // 입력 요청 추가
    printText(buffer); // 파티 목록 출력
    
    int choice = getIntInput(); // 사용자 선택 입력 받기
    if (choice == 0) { // 0번 선택 시
        return; // 함수 종료
    }
    
    if (choice > 0 && choice <= gPartyCount) { // 유효한 선택인 경우
        if (gParty[choice - 1].status == YOKAI_RELEASED) { // 이미 성불한 요괴인 경우
            printText("\n이미 성불한 요괴입니다."); // 오류 메시지 출력
            return; // 함수 종료
        }
        showYokaiSubMenu(&gParty[choice - 1]); // 요괴 서브메뉴 표시
    } else { // 잘못된 선택인 경우
        printText("\n잘못된 선택입니다."); // 오류 메시지 출력
    }
}

// 모든 요괴의 PP를 초기화하는 함수
void resetAllYokaiPP() { // 모든 요괴 PP 초기화 함수 시작
    LOG_FUNCTION_EXECUTION("resetAllYokaiPP"); // 함수 실행 로그 기록
    for (int i = 0; i < gPartyCount; i++) { // 모든 파티 요괴에 대해
        for (int j = 0; j < gParty[i].moveCount; j++) { // 해당 요괴의 모든 기술에 대해
            gParty[i].moves[j].currentPP = gParty[i].moves[j].move.pp; // 현재 PP를 최대 PP로 초기화
        }
    }
} 