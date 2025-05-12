#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "text.h"
#include "settings.h"
#include "input.h"

// 텍스트 출력 속도에 따른 대기 시간 계산 (밀리초)
int getTextDelay() {
    switch (gameSettings.textSpeed) {
        case 1: return 50;   // 매우 느림
        case 2: return 37;   // 느림
        case 3: return 25;   // 보통
        case 4: return 12;   // 빠름
        case 5: return 5;    // 매우 빠름
        default: return 25;  // 기본값: 보통
    }
}

void printText(const char* text) {
    int delay = getTextDelay();
    
    // 출력 시작 전 입력 버퍼 초기화
    clearInputBuffer();
    
    for (int i = 0; text[i] != '\0'; i++) {
        printf("%c", text[i]);
        fflush(stdout);  // 버퍼 즉시 출력
        Sleep(delay);    // 설정된 시간만큼 대기
        
        // 출력 중 입력이 들어오면 무시
        while (_kbhit()) {
            _getch();
        }
    }
    
    // 출력 완료 후 입력 버퍼 초기화
    clearInputBuffer();
}

void printTextAndWait(const char* text) {
    printText(text);
    waitForEnter();
} 