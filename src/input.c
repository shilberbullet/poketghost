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

// 입력 버퍼를 비우는 함수
void clearInputBuffer() {
    while (_kbhit()) {  // 버퍼에 문자가 있는 동안
        _getch();       // 문자를 읽어서 버퍼에서 제거
    }
}

// 정수 입력을 받는 함수
int getIntInput() {
    char input[256];    // 입력 문자열을 저장할 버퍼
    int number;         // 변환된 숫자를 저장할 변수
    int i = 0;          // 문자열 인덱스
    int isValid = 1;    // 입력 유효성 플래그
    
    // 입력 받기
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;      // 입력 실패 시 -1 반환
    }
    
    // 입력된 문자열이 비어있는지 확인
    if (input[0] == '\n') {
        return -1;      // 빈 입력 시 -1 반환
    }
    
    // 모든 문자가 숫자인지 확인
    while (input[i] != '\n' && input[i] != '\0') {
        if (!isdigit(input[i])) {  // 숫자가 아닌 문자가 있으면
            isValid = 0;            // 유효하지 않음으로 표시
            break;
        }
        i++;
    }
    
    // 숫자가 아닌 문자가 있거나 입력이 비어있는 경우
    if (!isValid || i == 0) {
        printf("\n숫자만 입력해주세요.\n");
        return -1;      // 유효하지 않은 입력 시 -1 반환
    }
    
    // 문자열을 숫자로 변환
    number = atoi(input);
    
    // 입력 버퍼 비우기
    clearInputBuffer();
    
    return number;      // 변환된 숫자 반환
}

// 엔터 키 입력을 기다리는 함수
void waitForEnter() {
    // 메시지 출력 없이 엔터 대기만
    clearInputBuffer();  // 입력 버퍼 비우기
    getchar();          // 엔터 키 입력 대기
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
            printf("\n%d부터 %d 사이의 숫자를 입력하세요.\n", min, max);  // 범위 초과 시 안내
        }
    }
} 