#include <stdio.h>
#include <conio.h>
#include "input.h"

void clearInputBuffer() {
    while (_kbhit()) {
        _getch();
    }
}

int getIntInput() {
    int input;
    int result;
    
    // 입력 받기
    result = scanf("%d", &input);
    
    // 입력이 실패한 경우
    if (result != 1) {
        clearInputBuffer();
        return -1;
    }
    
    // 남은 입력 버퍼 비우기
    clearInputBuffer();
    
    return input;
}

void waitForEnter() {
    // 메시지 출력 없이 엔터 대기만
    clearInputBuffer();
    getchar();
} 