// 표준 입출력, 게임 종료 관련 헤더
#include <stdio.h> // 표준 입출력 함수
#include <stdlib.h> // 표준 라이브러리 함수
#include "game.h" // 게임 관련 헤더
#include "text.h" // 텍스트 출력 헤더
#include "exit.h" // 게임 종료 헤더
#include "../core/state.h" // 게임 상태 헤더
#include "logger.h" // 로거 헤더
 
// 게임 종료 함수
void exitGame() { // 게임 종료 함수 시작
    LOG_FUNCTION_EXECUTION("exitGame"); // 함수 실행 로그 기록
    gGameState.isRunning = 0;  // 게임 루프 종료 플래그 설정
    printTextAndWait("\n게임을 종료합니다."); // 종료 메시지 출력 및 대기
    
    // 프로그램 종료 로그 기록
    logMessage("프로그램 종료"); // 종료 로그 기록
    cleanupLogger();  // 로그 시스템 정리
    
    exit(0);  // 프로그램 종료 (정상 종료 코드 0)
} 