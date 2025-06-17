// 표준 입출력 함수를 위한 헤더
#include <stdio.h>
// 콘솔 입력 함수를 위한 헤더
#include <conio.h>
// 문자 타입 검사 함수를 위한 헤더
#include <ctype.h>
// 메모리 할당, 문자열 변환 함수 등을 위한 헤더
#include <stdlib.h>
// 입력 처리 관련 함수와 구조체 정의
#include "input.h"
// 로깅 함수를 위한 헤더
#include "logger.h"

// 입력 버퍼를 비우는 함수
void clearInputBuffer() {
    while (_kbhit()) {  // 버퍼에 문자가 있는 동안
        _getch();       // 문자를 읽어서 버퍼에서 제거
    }
}

// 정수 입력을 받는 함수
int getIntInput() {
    int input;
    int result = scanf("%d", &input);
    clearInputBuffer();
    
    // 입력 로깅
    if (result == 1) {
        logInput("정수 입력: %d", input);
    } else {
        logError("잘못된 정수 입력");
    }
    
    return input;
}

// 문자 입력을 받는 함수
char getCharInput() {
    char input = _getch();
    
    // 입력 로깅
    logInput("문자 입력: %c", input);
    
    return input;
}

// 엔터 키 입력을 기다리는 함수
void waitForEnter() {
    logInput("엔터키 입력 대기");
    while (_getch() != '\r');
}

// 입력 대기 함수
void waitForInput() {
    logInput("아무 키나 입력 대기");
    _getch();
}

// 특정 범위 내의 숫자 입력을 받는 함수
int getNumberInput(int min, int max) {
    int num;
    while (1) {
        printf("선택 (%d-%d): ", min, max);  // 입력 범위 안내
        num = getIntInput();                 // 정수 입력 받기
        if (num >= min && num <= max) {      // 입력이 범위 내에 있으면
            return num;                      // 입력값 반환
        } else {
            printf("\n잘못된 선택입니다. 다시 선택하세요.\n");
        }
    }
}
