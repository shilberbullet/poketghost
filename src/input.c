#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include "input.h"

void clearInputBuffer() {
    while (_kbhit()) {
        _getch();
    }
}

int getIntInput() {
    char input[256];
    int number;
    int i = 0;
    int isValid = 1;
    
    // 입력 받기
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    // 입력된 문자열이 비어있는지 확인
    if (input[0] == '\n') {
        return -1;
    }
    
    // 모든 문자가 숫자인지 확인
    while (input[i] != '\n' && input[i] != '\0') {
        if (!isdigit(input[i])) {
            isValid = 0;
            break;
        }
        i++;
    }
    
    // 숫자가 아닌 문자가 있거나 입력이 비어있는 경우
    if (!isValid || i == 0) {
        printf("\n숫자만 입력해주세요.\n");
        return -1;
    }
    
    // 문자열을 숫자로 변환
    number = atoi(input);
    
    // 입력 버퍼 비우기
    clearInputBuffer();
    
    return number;
}

void waitForEnter() {
    // 메시지 출력 없이 엔터 대기만
    clearInputBuffer();
    getchar();
}

int getNumberInput(int min, int max) {
    int num;
    while (1) {
        printf("선택 (%d-%d): ", min, max);
        num = getIntInput();
        if (num >= min && num <= max) {
            return num;
        } else {
            printf("\n%d부터 %d 사이의 숫자를 입력하세요.\n", min, max);
        }
    }
} 