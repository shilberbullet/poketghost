#include "logger.h"
#include <windows.h>
#include <direct.h>
#include <errno.h>
#include <string.h>

#define LOG_FILENAME_BASE "poketghostlog.txt"
#define LOG_MAX_LINES 1500

static FILE* logFile = NULL;
static int logLineCount = 0;
static int logFileIndex = 0;
static char currentLogFileName[256] = "";

// 현재 시간 기반 파일명 생성
static void makeLogFileName(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(buffer, size, "logs/%04d%02d%02d_%02d%02d%02d_%d,%s",
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
        t->tm_hour, t->tm_min, t->tm_sec,
        logFileIndex, LOG_FILENAME_BASE);
}

// 로그 파일 열기 (분할 포함)
static void openLogFile() {
    makeLogFileName(currentLogFileName, sizeof(currentLogFileName));
    logFile = fopen(currentLogFileName, "a");
    // 파일의 기존 줄 수를 세서 logLineCount에 반영
    logLineCount = 0;
    FILE* checkFile = fopen(currentLogFileName, "r");
    if (checkFile) {
        int c;
        while ((c = fgetc(checkFile)) != EOF) {
            if (c == '\n') logLineCount++;
        }
        fclose(checkFile);
    }
}

// 로그 파일 분할 체크 및 분할
static void checkLogFileSplit() {
    if (logLineCount >= LOG_MAX_LINES) {
        if (logFile) fclose(logFile);
        logFileIndex++;
        openLogFile();
    }
}

// 로그 초기화 함수
void initLogger() {
    // logs 디렉토리 생성
    if (_mkdir("logs") == -1 && errno != EEXIST) {
        printf("로그 디렉토리 생성 실패\n");
        return;
    }
    logFileIndex = 0;
    openLogFile();
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
    logLineCount++;
}

// 문자열 내 \n 개수 세기
static int count_newlines(const char* str) {
    int count = 0;
    while (*str) {
        if (*str == '\n') count++;
        str++;
    }
    return count;
}

// 로그 기록 함수
void logMessage(const char* format, ...) {
    if (logFile == NULL) return;
    checkLogFileSplit();
    char buffer[2048];
    time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0';
    int n = snprintf(buffer, sizeof(buffer), "[%s] ", timeStr);
    va_list args;
    va_start(args, format);
    n += vsnprintf(buffer + n, sizeof(buffer) - n, format, args);
    va_end(args);
    n += snprintf(buffer + n, sizeof(buffer) - n, "\n");
    int lines = count_newlines(buffer);
    fwrite(buffer, 1, strlen(buffer), logFile);
    fflush(logFile);
    logLineCount += lines;
    checkLogFileSplit();
}

// 출력 로깅 함수
void logOutput(const char* format, ...) {
    if (logFile == NULL) return;
    checkLogFileSplit();
    char buffer[2048];
    time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0';
    int n = snprintf(buffer, sizeof(buffer), "[%s] [OUTPUT] ", timeStr);
    va_list args;
    va_start(args, format);
    n += vsnprintf(buffer + n, sizeof(buffer) - n, format, args);
    va_end(args);
    n += snprintf(buffer + n, sizeof(buffer) - n, "\n");
    int lines = count_newlines(buffer);
    fwrite(buffer, 1, strlen(buffer), logFile);
    fflush(logFile);
    logLineCount += lines;
    checkLogFileSplit();
}

// 입력 로깅 함수
void logInput(const char* format, ...) {
    if (logFile == NULL) return;
    checkLogFileSplit();
    char buffer[2048];
    time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0';
    int n = snprintf(buffer, sizeof(buffer), "[%s] [INPUT] ", timeStr);
    va_list args;
    va_start(args, format);
    n += vsnprintf(buffer + n, sizeof(buffer) - n, format, args);
    va_end(args);
    n += snprintf(buffer + n, sizeof(buffer) - n, "\n");
    int lines = count_newlines(buffer);
    fwrite(buffer, 1, strlen(buffer), logFile);
    fflush(logFile);
    logLineCount += lines;
    checkLogFileSplit();
}

// 오류 로깅 함수
void logError(const char* format, ...) {
    if (logFile == NULL) return;
    checkLogFileSplit();
    char buffer[2048];
    time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0';
    int n = snprintf(buffer, sizeof(buffer), "[%s] [ERROR] ", timeStr);
    va_list args;
    va_start(args, format);
    n += vsnprintf(buffer + n, sizeof(buffer) - n, format, args);
    va_end(args);
    n += snprintf(buffer + n, sizeof(buffer) - n, "\n");
    int lines = count_newlines(buffer);
    fwrite(buffer, 1, strlen(buffer), logFile);
    fflush(logFile);
    logLineCount += lines;
    checkLogFileSplit();
}

// 함수 실행 로깅 함수
void logFunctionExecution(const char* funcName) {
    if (logFile == NULL) return;
    checkLogFileSplit();
    char buffer[2048];
    time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0';
    int n = snprintf(buffer, sizeof(buffer), "[%s] [함수실행] %s\n", timeStr, funcName);
    int lines = count_newlines(buffer);
    fwrite(buffer, 1, strlen(buffer), logFile);
    fflush(logFile);
    logLineCount += lines;
    checkLogFileSplit();
}

// 로그 시스템 정리 함수
void cleanupLogger() {
    if (logFile != NULL) {
        time_t now = time(NULL);
        char* timeStr = ctime(&now);
        timeStr[strlen(timeStr) - 1] = '\0';
        fprintf(logFile, "=== 게임 종료: %s ===\n", timeStr);
        fflush(logFile);
        fclose(logFile);
        logFile = NULL;
    }
} 