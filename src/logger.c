/**
 * @file logger.c
 * @brief 포켓요괴 게임의 로깅 시스템 구현 파일
 * @details 게임 실행 로그, 오류 로그, 함수 실행 로그 등의 기능 구현
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

// 로거 관련 함수와 구조체 정의
#include "logger.h" // 로거 헤더
// 윈도우 API를 위한 헤더
#include <windows.h> // 윈도우 API
// 디렉토리 생성 함수를 위한 헤더
#include <direct.h> // 디렉토리 생성 함수
// 오류 번호를 위한 헤더
#include <errno.h> // 오류 번호
// 문자열 처리 함수를 위한 헤더
#include <string.h> // 문자열 처리 함수

// 상수 정의
#define LOG_FILENAME_BASE "poketghostlog.txt" // 로그 파일 기본 이름
#define LOG_MAX_LINES 1500 // 로그 파일당 최대 줄 수

// 전역 변수 선언
static FILE* logFile = NULL; // 로그 파일 포인터
static int logLineCount = 0; // 현재 로그 파일의 줄 수
static int logFileIndex = 0; // 로그 파일 인덱스
static char currentLogFileName[256] = ""; // 현재 로그 파일명

/**
 * @brief 현재 시간 기반 로그 파일명을 생성하는 함수
 * @param buffer 파일명을 저장할 버퍼
 * @param size 버퍼 크기
 */
static void makeLogFileName(char* buffer, size_t size) { // 로그 파일명 생성 함수 시작
    time_t now = time(NULL); // 현재 시간 가져오기
    struct tm* t = localtime(&now); // 로컬 시간으로 변환
    snprintf(buffer, size, "logs/%04d%02d%02d_%02d%02d%02d_%d,%s", // 파일명 형식
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, // 년월일
        t->tm_hour, t->tm_min, t->tm_sec, // 시분초
        logFileIndex, LOG_FILENAME_BASE); // 인덱스와 기본 이름
}

/**
 * @brief 로그 파일을 여는 함수 (분할 포함)
 */
static void openLogFile() { // 로그 파일 열기 함수 시작
    makeLogFileName(currentLogFileName, sizeof(currentLogFileName)); // 파일명 생성
    logFile = fopen(currentLogFileName, "a"); // 추가 모드로 파일 열기
    // 파일의 기존 줄 수를 세서 logLineCount에 반영
    logLineCount = 0; // 줄 수 초기화
    FILE* checkFile = fopen(currentLogFileName, "r"); // 읽기 모드로 파일 열기
    if (checkFile) { // 파일이 열렸으면
        int c; // 문자 변수
        while ((c = fgetc(checkFile)) != EOF) { // 파일 끝까지 읽기
            if (c == '\n') logLineCount++; // 개행 문자 카운트
        }
        fclose(checkFile); // 파일 닫기
    }
}

/**
 * @brief 로그 파일 분할을 체크하고 분할하는 함수
 */
static void checkLogFileSplit() { // 로그 파일 분할 체크 함수 시작
    if (logLineCount >= LOG_MAX_LINES) { // 최대 줄 수에 도달했으면
        if (logFile) fclose(logFile); // 현재 파일 닫기
        logFileIndex++; // 파일 인덱스 증가
        openLogFile(); // 새 파일 열기
    }
}

/**
 * @brief 로그 시스템을 초기화하는 함수
 * @details 로그 파일명 자동 생성 및 분할 관리
 */
void initLogger() { // 로거 초기화 함수 시작
    // logs 디렉토리 생성
    if (_mkdir("logs") == -1 && errno != EEXIST) { // 디렉토리 생성 실패 확인
        printf("로그 디렉토리 생성 실패\n"); // 오류 메시지
        return; // 함수 종료
    }
    logFileIndex = 0; // 파일 인덱스 초기화
    openLogFile(); // 로그 파일 열기
    if (logFile == NULL) { // 파일 열기 실패 확인
        printf("로그 파일 열기 실패\n"); // 오류 메시지
        return; // 함수 종료
    }
    // 로그 시작 메시지 기록
    time_t now = time(NULL); // 현재 시간
    char* timeStr = ctime(&now); // 시간 문자열
    timeStr[strlen(timeStr) - 1] = '\0';  // 개행 문자 제거
    fprintf(logFile, "\n=== 게임 시작: %s ===\n", timeStr); // 시작 메시지 기록
    fflush(logFile); // 버퍼 즉시 출력
    logLineCount++; // 줄 수 증가
}

/**
 * @brief 문자열 내 개행 문자 개수를 세는 함수
 * @param str 검사할 문자열
 * @return 개행 문자 개수
 */
static int count_newlines(const char* str) { // 개행 문자 개수 세기 함수 시작
    int count = 0; // 카운터 초기화
    while (*str) { // 문자열 끝까지
        if (*str == '\n') count++; // 개행 문자 카운트
        str++; // 다음 문자로
    }
    return count; // 개수 반환
}

/**
 * @brief 일반 메시지를 로그에 기록하는 함수
 * @param format 메시지 형식 문자열
 * @param ... 가변 인자
 */
void logMessage(const char* format, ...) { // 일반 메시지 로그 함수 시작
    if (logFile == NULL) return; // 로그 파일이 없으면 종료
    checkLogFileSplit(); // 파일 분할 체크
    char buffer[2048]; // 메시지 버퍼
    time_t now = time(NULL); // 현재 시간
    char* timeStr = ctime(&now); // 시간 문자열
    timeStr[strlen(timeStr) - 1] = '\0'; // 개행 문자 제거
    int n = snprintf(buffer, sizeof(buffer), "[%s] ", timeStr); // 시간 정보 추가
    va_list args; // 가변 인자 리스트
    va_start(args, format); // 가변 인자 시작
    n += vsnprintf(buffer + n, sizeof(buffer) - n, format, args); // 메시지 추가
    va_end(args); // 가변 인자 종료
    n += snprintf(buffer + n, sizeof(buffer) - n, "\n"); // 개행 추가
    int lines = count_newlines(buffer); // 줄 수 계산
    fwrite(buffer, 1, strlen(buffer), logFile); // 파일에 쓰기
    fflush(logFile); // 버퍼 즉시 출력
    logLineCount += lines; // 줄 수 증가
    checkLogFileSplit(); // 파일 분할 체크
}

/**
 * @brief 출력 내용을 로그에 기록하는 함수
 * @param format 출력 형식 문자열
 * @param ... 가변 인자
 */
void logOutput(const char* format, ...) { // 출력 로그 함수 시작
    if (logFile == NULL) return; // 로그 파일이 없으면 종료
    checkLogFileSplit(); // 파일 분할 체크
    char buffer[2048]; // 메시지 버퍼
    time_t now = time(NULL); // 현재 시간
    char* timeStr = ctime(&now); // 시간 문자열
    timeStr[strlen(timeStr) - 1] = '\0'; // 개행 문자 제거
    int n = snprintf(buffer, sizeof(buffer), "[%s] [OUTPUT] ", timeStr); // 시간과 타입 추가
    va_list args; // 가변 인자 리스트
    va_start(args, format); // 가변 인자 시작
    n += vsnprintf(buffer + n, sizeof(buffer) - n, format, args); // 메시지 추가
    va_end(args); // 가변 인자 종료
    n += snprintf(buffer + n, sizeof(buffer) - n, "\n"); // 개행 추가
    int lines = count_newlines(buffer); // 줄 수 계산
    fwrite(buffer, 1, strlen(buffer), logFile); // 파일에 쓰기
    fflush(logFile); // 버퍼 즉시 출력
    logLineCount += lines; // 줄 수 증가
    checkLogFileSplit(); // 파일 분할 체크
}

/**
 * @brief 입력 내용을 로그에 기록하는 함수
 * @param format 입력 형식 문자열
 * @param ... 가변 인자
 */
void logInput(const char* format, ...) { // 입력 로그 함수 시작
    if (logFile == NULL) return; // 로그 파일이 없으면 종료
    checkLogFileSplit(); // 파일 분할 체크
    char buffer[2048]; // 메시지 버퍼
    time_t now = time(NULL); // 현재 시간
    char* timeStr = ctime(&now); // 시간 문자열
    timeStr[strlen(timeStr) - 1] = '\0'; // 개행 문자 제거
    int n = snprintf(buffer, sizeof(buffer), "[%s] [INPUT] ", timeStr); // 시간과 타입 추가
    va_list args; // 가변 인자 리스트
    va_start(args, format); // 가변 인자 시작
    n += vsnprintf(buffer + n, sizeof(buffer) - n, format, args); // 메시지 추가
    va_end(args); // 가변 인자 종료
    n += snprintf(buffer + n, sizeof(buffer) - n, "\n"); // 개행 추가
    int lines = count_newlines(buffer); // 줄 수 계산
    fwrite(buffer, 1, strlen(buffer), logFile); // 파일에 쓰기
    fflush(logFile); // 버퍼 즉시 출력
    logLineCount += lines; // 줄 수 증가
    checkLogFileSplit(); // 파일 분할 체크
}

/**
 * @brief 오류 메시지를 로그에 기록하는 함수
 * @param format 오류 형식 문자열
 * @param ... 가변 인자
 */
void logError(const char* format, ...) { // 오류 로그 함수 시작
    if (logFile == NULL) return; // 로그 파일이 없으면 종료
    checkLogFileSplit(); // 파일 분할 체크
    char buffer[2048]; // 메시지 버퍼
    time_t now = time(NULL); // 현재 시간
    char* timeStr = ctime(&now); // 시간 문자열
    timeStr[strlen(timeStr) - 1] = '\0'; // 개행 문자 제거
    int n = snprintf(buffer, sizeof(buffer), "[%s] [ERROR] ", timeStr); // 시간과 타입 추가
    va_list args; // 가변 인자 리스트
    va_start(args, format); // 가변 인자 시작
    n += vsnprintf(buffer + n, sizeof(buffer) - n, format, args); // 메시지 추가
    va_end(args); // 가변 인자 종료
    n += snprintf(buffer + n, sizeof(buffer) - n, "\n"); // 개행 추가
    int lines = count_newlines(buffer); // 줄 수 계산
    fwrite(buffer, 1, strlen(buffer), logFile); // 파일에 쓰기
    fflush(logFile); // 버퍼 즉시 출력
    logLineCount += lines; // 줄 수 증가
    checkLogFileSplit(); // 파일 분할 체크
}

/**
 * @brief 함수 실행을 로그에 기록하는 함수
 * @param funcName 실행된 함수명
 */
void logFunctionExecution(const char *funcName) {
    if (!logFile) return; // 로그 파일이 없으면 종료

    checkLogFileSplit(); // 파일 분할 체크

    char buffer[2048]; // 메시지 버퍼
    time_t now = time(NULL); // 현재 시간
    char *timeStr = ctime(&now); // 시간 문자열
    timeStr[strlen(timeStr) - 1] = '\0';  // 개행 문자 제거

    snprintf(buffer, sizeof(buffer), "[%s] [함수실행] %s\n", timeStr, funcName); // 함수 실행 메시지 생성
    
    // 로그 파일에 쓰기
    fputs(buffer, logFile); // 파일에 쓰기
    fflush(logFile); // 버퍼 즉시 출력

    // 로그 라인 수 업데이트
    logLineCount++; // 줄 수 증가
}

/**
 * @brief 로그 시스템을 정리하는 함수
 * @details 로그 파일 닫기 등의 정리 작업
 */
void cleanupLogger() {
    if (logFile != NULL) { // 로그 파일이 열려있으면
        time_t now = time(NULL); // 현재 시간
        char* timeStr = ctime(&now); // 시간 문자열
        timeStr[strlen(timeStr) - 1] = '\0'; // 개행 문자 제거
        fprintf(logFile, "=== 게임 종료: %s ===\n", timeStr); // 종료 메시지 기록
        fflush(logFile); // 버퍼 즉시 출력
        fclose(logFile); // 파일 닫기
        logFile = NULL; // 포인터 초기화
    }
} 