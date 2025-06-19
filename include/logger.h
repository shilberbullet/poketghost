#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

// 로그 파일 경로
#define LOG_FILE_PATH "logs/game.log"

// 로그 초기화 함수 (파일명 자동 생성 및 분할 관리)
void initLogger();

// 로그 기록 함수
void logMessage(const char* format, ...);

// 출력 로깅 함수
void logOutput(const char* format, ...);

// 입력 로깅 함수
void logInput(const char* format, ...);

// 오류 로깅 함수
void logError(const char* format, ...);

// 로그 시스템 정리 함수
void cleanupLogger();

#endif // LOGGER_H 