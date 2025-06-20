// 표준 입출력 함수를 위한 헤더
#include <stdio.h>
// Windows API 함수를 위한 헤더
#include <windows.h>
// 콘솔 입력 함수를 위한 헤더
#include <conio.h>
// 텍스트 출력 관련 함수와 구조체 정의
#include "text.h"
// 설정 관련 함수와 구조체 정의
#include "settings.h"
// 입력 처리 관련 함수
#include "input.h"
// 로깅 관련 함수와 구조체 정의
#include "logger.h"

// 텍스트 출력 속도에 따른 대기 시간 계산 (밀리초)
int getTextDelay() {
    LOG_FUNCTION_EXECUTION("getTextDelay");
    switch (gameSettings.textSpeed) {
        case 1: return 15;   // 매우 느림 (10ms)
        case 2: return 10;   // 느림 (5ms)
        case 3: return 5;    // 보통 (3ms)
        case 4: return 2.5;    // 빠름 (2ms)
        case 5: return 1;    // 매우 빠름 (1ms)
        default: return 5;   // 기본값: 보통 (5ms)
    }
}

// 텍스트를 한 글자씩 출력하는 함수
void printText(const char* text) {
    LOG_FUNCTION_EXECUTION("printText");
    // 출력 로깅
    logOutput("%s", text);

    int delay = getTextDelay();  // 현재 설정된 텍스트 속도
    
    // 출력 시작 전 입력 버퍼 초기화
    clearInputBuffer();
    
    switch (gameSettings.textDisplayMode) {
        case 0:  // 한 글자씩 출력
            for (int i = 0; text[i] != '\0'; i++) {
                printf("%c", text[i]);   // 한 글자씩 출력
                fflush(stdout);          // 버퍼 즉시 출력
                Sleep(delay);            // 설정된 시간만큼 대기
                
                // 출력 중 입력이 들어오면 무시
                while (_kbhit()) {
                    _getch();
                }
            }
            break;
            
        case 1:  // 한 줄씩 출력
            printf("%s", text);   // 한 줄씩 출력
            fflush(stdout);       // 버퍼 즉시 출력
            Sleep(delay*15);    // 한 줄 출력 후 대기
            break;
            
        case 2:  // 한 번에 출력
            printf("%s", text);   // 한 번에 출력
            fflush(stdout);       // 버퍼 즉시 출력
            break;
    }
    
    // 출력 완료 후 입력 버퍼 초기화
    clearInputBuffer();
}

// 텍스트를 출력하고 1초 대기 후 자동으로 넘어가는 함수
void printTextAndWait(const char* text) {
    LOG_FUNCTION_EXECUTION("printTextAndWait");
    int delay = getTextDelay();

    printText(text);
    Sleep(delay * 50);
}

// 텍스트 속도에 비례한 sleep 함수 (base=500이면 매우 빠름)
void fastSleep(int base) {
    LOG_FUNCTION_EXECUTION("fastSleep");
    (void)base;
    int delay = getTextDelay();
    Sleep(delay*180);
}