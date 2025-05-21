#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

void clearInputBuffer() {
    while (_kbhit()) {
        _getch();
    }
}

int getIntInput(int min, int max) {
    int input;
    char buffer[256];
    
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        // 입력 버퍼에서 개행문자 제거
        buffer[strcspn(buffer, "\n")] = 0;
        
        // 입력이 숫자인지 확인
        int isValid = 1;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] < '0' || buffer[i] > '9') {
                isValid = 0;
                break;
            }
        }
        
        if (!isValid) {
            printf("숫자만 입력해주세요.\n");
            continue;
        }
        
        input = atoi(buffer);
        
        if (input >= min && input <= max) {
            return input;
        }
        printf("%d에서 %d 사이의 숫자를 입력해주세요.\n", min, max);
    }
}

void getStringInput(char* buffer, int maxLength) {
    if (fgets(buffer, maxLength, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    
    // 입력 버퍼에서 개행문자 제거
    buffer[strcspn(buffer, "\n")] = 0;
    
    // 입력 버퍼 정리
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void waitForEnter() {
    // 메시지 출력 없이 엔터 대기만
    clearInputBuffer();
    getchar();
} 