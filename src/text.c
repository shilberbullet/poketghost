/**
 * @file text.c
 * @brief 포켓요괴 게임의 텍스트 출력 시스템 구현 파일
 * @details 텍스트 출력 속도 조절, 한 글자씩 출력 등의 기능을 구현합니다.
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

// 표준 입출력 함수를 위한 헤더
#include <stdio.h> // 표준 입출력 함수
// Windows API 함수를 위한 헤더
#include <windows.h> // 윈도우 API 함수
// 콘솔 입력 함수를 위한 헤더
#include <conio.h> // 콘솔 입력 함수
// 텍스트 출력 관련 함수와 구조체 정의
#include "text.h" // 텍스트 출력 헤더
// 설정 관련 함수와 구조체 정의
#include "settings.h" // 설정 관련 헤더
// 입력 처리 관련 함수
#include "input.h" // 입력 처리 헤더
// 로깅 관련 함수와 구조체 정의
#include "logger.h" // 로거 헤더

/**
 * @brief 텍스트 출력 속도에 따른 대기 시간을 계산하는 함수
 * @return 대기 시간 (밀리초)
 */
int getTextDelay() { // 텍스트 지연 시간 계산 함수 시작
    LOG_FUNCTION_EXECUTION("getTextDelay"); // 함수 실행 로그
    switch (gameSettings.textSpeed) { // 텍스트 속도 설정에 따라
        case 1: return 8;    // 매우 느림 (8ms) - 기존 15ms에서 빠르게
        case 2: return 5;    // 느림 (5ms) - 기존 10ms에서 빠르게
        case 3: return 3;    // 보통 (3ms) - 기존 5ms에서 빠르게
        case 4: return 2;    // 빠름 (2ms) - 기존 2.5ms에서 빠르게
        case 5: return 1;    // 매우 빠름 (1ms) - 0보다는 느리고 1ms
        default: return 3;   // 기본값: 보통 (3ms)
    }
}

/**
 * @brief 텍스트를 한 글자씩 출력하는 함수
 * @param text 출력할 텍스트
 */
void printText(const char* text) { // 텍스트 출력 함수 시작
    LOG_FUNCTION_EXECUTION("printText"); // 함수 실행 로그
    // 출력 로깅
    logOutput("%s", text); // 출력 내용을 로그에 기록

    int delay = getTextDelay();  // 현재 설정된 텍스트 속도로 대기 시간 계산
    
    // 출력 시작 전 입력 버퍼 초기화
    clearInputBuffer(); // 입력 버퍼를 비워서 이전 입력 무시
    
    switch (gameSettings.textDisplayMode) { // 텍스트 출력 모드에 따라
        case 0:  // 한 글자씩 출력
            for (int i = 0; text[i] != '\0'; i++) { // 텍스트 끝까지 반복
                printf("%c", text[i]);   // 한 글자씩 출력
                fflush(stdout);          // 버퍼 즉시 출력
                
                // 매우 빠름 모드에서는 Sleep(0)과 Sleep(1)을 번갈아 사용
                if (gameSettings.textSpeed == 5) { // 매우 빠름 모드인 경우
                    if (i % 2 == 0) { // 짝수 번째 글자
                        Sleep(0); // 즉시 출력
                    } else { // 홀수 번째 글자
                        Sleep(1); // 1ms 대기
                    }
                } else { // 다른 속도 모드인 경우
                    Sleep(delay);            // 설정된 시간만큼 대기
                }
                
                // 출력 중 입력이 들어오면 무시
                while (_kbhit()) { // 키 입력이 있는 동안
                    _getch(); // 입력을 읽어서 버림
                }
            }
            break; // 한 글자씩 출력 모드 종료
            
        case 1:  // 한 줄씩 출력
            printf("%s", text);   // 한 줄씩 출력
            fflush(stdout);       // 버퍼 즉시 출력
            Sleep(delay*15);    // 한 줄 출력 후 대기 (15배 길게)
            break; // 한 줄씩 출력 모드 종료
            
        case 2:  // 한 번에 출력
            printf("%s", text);   // 한 번에 출력
            fflush(stdout);       // 버퍼 즉시 출력
            break; // 한 번에 출력 모드 종료
    }
    
    // 출력 완료 후 입력 버퍼 초기화
    clearInputBuffer(); // 입력 버퍼를 비워서 다음 입력 준비
}

/**
 * @brief 텍스트를 출력하고 1초 대기 후 자동으로 넘어가는 함수
 * @param text 출력할 텍스트
 */
void printTextAndWait(const char* text) { // 텍스트 출력 후 대기 함수 시작
    LOG_FUNCTION_EXECUTION("printTextAndWait"); // 함수 실행 로그
    int delay = getTextDelay(); // 현재 텍스트 속도로 대기 시간 계산

    printText(text); // 텍스트 출력
    Sleep(delay * 50); // 50배 길게 대기 (약 1초)
}

/**
 * @brief 텍스트 속도에 비례한 sleep 함수
 * @param base 기본 대기 시간 (사용하지 않음)
 */
void fastSleep(int base) { // 빠른 대기 함수 시작
    LOG_FUNCTION_EXECUTION("fastSleep"); // 함수 실행 로그
    (void)base; // base 매개변수 사용하지 않음 (경고 방지)
    int delay = getTextDelay(); // 현재 텍스트 속도로 대기 시간 계산
    Sleep(delay*180); // 180배 길게 대기 (약 3-4초)
}