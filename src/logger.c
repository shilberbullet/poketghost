#include "logger.h"
#include <windows.h>
#include <direct.h>
#include <errno.h>

static FILE* logFile = NULL;

// 로그 초기화 함수
void initLogger() {
    // logs 디렉토리 생성
    if (_mkdir("logs") == -1 && errno != EEXIST) {
        printf("로그 디렉토리 생성 실패\n");
        return;
    }

    // 로그 파일 열기 (추가 모드)
    logFile = fopen(LOG_FILE_PATH, "a");
    if (logFile == NULL) {
        printf("로그 파일 열기 실패\n");
        return;
    }

    // 로그 시작 메시지 기록
    time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0';  // 개행 문자 제거
    fprintf(logFile, "\n=== 게임 시작: %s ===\n", timeStr);
    fflush(logFile);
}

// 로그 기록 함수
void logMessage(const char* format, ...) {
    if (logFile == NULL) return;

    // 현재 시간 기록
    time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0';  // 개행 문자 제거
    fprintf(logFile, "[%s] ", timeStr);

    // 가변 인자 처리
    va_list args;
    va_start(args, format);
    vfprintf(logFile, format, args);
    va_end(args);

    fprintf(logFile, "\n");
    fflush(logFile);
}

// 출력 로깅 함수
void logOutput(const char* format, ...) {
    if (logFile == NULL) return;

    // 현재 시간 기록
    time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0';  // 개행 문자 제거
    fprintf(logFile, "[%s] [OUTPUT] ", timeStr);

    // 가변 인자 처리
    va_list args;
    va_start(args, format);
    vfprintf(logFile, format, args);
    va_end(args);

    fprintf(logFile, "\n");
    fflush(logFile);
}

// 입력 로깅 함수
void logInput(const char* format, ...) {
    if (logFile == NULL) return;

    // 현재 시간 기록
    time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0';  // 개행 문자 제거
    fprintf(logFile, "[%s] [INPUT] ", timeStr);

    // 가변 인자 처리
    va_list args;
    va_start(args, format);
    vfprintf(logFile, format, args);
    va_end(args);

    fprintf(logFile, "\n");
    fflush(logFile);
}

// 오류 로깅 함수
void logError(const char* format, ...) {
    if (logFile == NULL) return;

    // 현재 시간 기록
    time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0';  // 개행 문자 제거
    fprintf(logFile, "[%s] [ERROR] ", timeStr);

    // 가변 인자 처리
    va_list args;
    va_start(args, format);
    vfprintf(logFile, format, args);
    va_end(args);

    fprintf(logFile, "\n");
    fflush(logFile);
}

// 로그 시스템 정리 함수
void cleanupLogger() {
    if (logFile != NULL) {
        // 로그 종료 메시지 기록
        time_t now = time(NULL);
        char* timeStr = ctime(&now);
        timeStr[strlen(timeStr) - 1] = '\0';  // 개행 문자 제거
        fprintf(logFile, "=== 게임 종료: %s ===\n", timeStr);
        fflush(logFile);

        fclose(logFile);
        logFile = NULL;
    }
} 