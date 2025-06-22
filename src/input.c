/**
 * @file input.c
 * @brief 포켓요괴 게임의 입력 처리 시스템 구현 파일
 * @details 사용자 입력 처리, 버퍼 관리, 입력 검증 등의 기능 구현
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

// 표준 입출력 함수를 위한 헤더
#include <stdio.h> // 표준 입출력 함수
// 콘솔 입력 함수를 위한 헤더
#include <conio.h> // 콘솔 입력 함수
// 문자 타입 검사 함수를 위한 헤더
#include <ctype.h> // 문자 타입 검사 함수
// 메모리 할당, 문자열 변환 함수 등을 위한 헤더
#include <stdlib.h> // 표준 라이브러리 함수
// 입력 처리 관련 함수와 구조체 정의
#include "input.h" // 입력 처리 헤더
// 로깅 함수를 위한 헤더
#include "logger.h" // 로거 헤더

/**
 * @brief 입력 버퍼를 비우는 함수
 * @details 이전 입력을 무시하고 새로운 입력을 받기 위해 사용
 */
void clearInputBuffer() { // 입력 버퍼 비우기 함수 시작
    LOG_FUNCTION_EXECUTION("clearInputBuffer"); // 함수 실행 로그
    while (_kbhit()) {  // 버퍼에 문자가 있는 동안
        _getch();       // 문자를 읽어서 버퍼에서 제거
    }
}

/**
 * @brief 안전한 정수 입력을 받는 함수
 * @return 입력받은 정수, 오류 시 -1
 */
int getIntInput() { // 정수 입력 받기 함수 시작
    LOG_FUNCTION_EXECUTION("getIntInput"); // 함수 실행 로그
    char input[256];    // 입력 문자열을 저장할 버퍼
    int number;         // 변환된 숫자를 저장할 변수
    int i = 0;          // 문자열 인덱스
    int isValid = 1;    // 입력 유효성 플래그
    
    // 입력 받기
    if (fgets(input, sizeof(input), stdin) == NULL) { // 입력 실패 확인
        logError("입력 실패"); // 오류 로그
        return -1;      // 입력 실패 시 -1 반환
    }
    
    // 입력된 문자열이 비어있는지 확인
    if (input[0] == '\n') { // 빈 입력 확인
        logError("빈 입력"); // 오류 로그
        return -1;      // 빈 입력 시 -1 반환
    }
    
    // 모든 문자가 숫자인지 확인
    while (input[i] != '\n' && input[i] != '\0') { // 문자열 끝까지 검사
        if (!isdigit(input[i])) {  // 숫자가 아닌 문자가 있으면
            isValid = 0;            // 유효하지 않음으로 표시
            break; // 루프 종료
        }
        i++; // 다음 문자로
    }
    
    // 숫자가 아닌 문자가 있거나 입력이 비어있는 경우
    if (!isValid || i == 0) { // 유효성 검사
        logError("잘못된 입력: %s", input); // 오류 로그
        printf("\n잘못된 선택입니다. 다시 선택하세요.\n"); // 오류 메시지
        return -1;      // 유효하지 않은 입력 시 -1 반환
    }
    
    // 문자열을 숫자로 변환
    number = atoi(input); // 문자열을 정수로 변환
    
    // 입력 로깅
    logInput("정수 입력: %d", number); // 입력 로그
    
    // 입력 버퍼 비우기
    clearInputBuffer(); // 버퍼 정리
    
    return number;      // 변환된 숫자 반환
}

/**
 * @brief 문자 입력을 받는 함수
 * @return 입력받은 문자
 */
char getCharInput() { // 문자 입력 받기 함수 시작
    LOG_FUNCTION_EXECUTION("getCharInput"); // 함수 실행 로그
    char input = _getch(); // 문자 입력 받기
    
    // 입력 로깅
    logInput("문자 입력: %c", input); // 입력 로그
    
    return input; // 입력된 문자 반환
}

/**
 * @brief 엔터 키 입력을 기다리는 함수
 * @details 사용자가 엔터를 누를 때까지 대기
 */
void waitForEnter() { // 엔터키 대기 함수 시작
    LOG_FUNCTION_EXECUTION("waitForEnter"); // 함수 실행 로그
    logInput("엔터키 입력 대기"); // 입력 로그
    clearInputBuffer();  // 입력 버퍼 비우기
    while (_getch() != '\r');  // 엔터키 입력 대기
}

/**
 * @brief 아무 키나 입력을 기다리는 함수
 */
void waitForInput() { // 키 입력 대기 함수 시작
    LOG_FUNCTION_EXECUTION("waitForInput"); // 함수 실행 로그
    logInput("아무 키나 입력 대기"); // 입력 로그
    _getch(); // 키 입력 대기
}

/**
 * @brief 범위 내 정수 입력을 받는 함수
 * @param min 최소값
 * @param max 최대값
 * @return 입력받은 정수, 범위 밖이면 -1
 */
int getNumberInput(int min, int max) { // 범위 내 숫자 입력 함수 시작
    LOG_FUNCTION_EXECUTION("getNumberInput"); // 함수 실행 로그
    int num; // 입력받을 숫자
    while (1) { // 무한 루프
        printf("선택 (%d-%d): ", min, max);  // 입력 범위 안내
        num = getIntInput();                 // 정수 입력 받기
        if (num >= min && num <= max) {      // 입력이 범위 내에 있으면
            return num;                      // 입력값 반환
        } else { // 범위 밖이면
            printf("\n잘못된 선택입니다. 다시 선택하세요.\n"); // 오류 메시지
        }
    }
}
