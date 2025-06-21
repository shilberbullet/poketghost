/**
 * @file logger.h
 * @brief 포켓요괴 게임의 로깅 시스템 헤더 파일
 * @details 게임 실행 로그, 오류 로그, 함수 실행 로그 등의 기능 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h> // 표준 입출력 함수
#include <time.h> // 시간 관련 함수
#include <stdarg.h> // 가변 인자 함수

// 로그 파일 경로
#define LOG_FILE_PATH "logs/game.log" // 로그 파일 저장 경로

// 함수 실행 로깅을 위한 매크로
#define LOG_FUNCTION_EXECUTION(func_name) logFunctionExecution(func_name) // 함수 실행 로그 매크로

/**
 * @brief 로그 시스템을 초기화하는 함수
 * @details 로그 파일명 자동 생성 및 분할 관리
 */
void initLogger();

/**
 * @brief 일반 메시지를 로그에 기록하는 함수
 * @param format 메시지 형식 문자열
 * @param ... 가변 인자
 */
void logMessage(const char* format, ...);

/**
 * @brief 출력 내용을 로그에 기록하는 함수
 * @param format 출력 형식 문자열
 * @param ... 가변 인자
 */
void logOutput(const char* format, ...);

/**
 * @brief 입력 내용을 로그에 기록하는 함수
 * @param format 입력 형식 문자열
 * @param ... 가변 인자
 */
void logInput(const char* format, ...);

/**
 * @brief 오류 메시지를 로그에 기록하는 함수
 * @param format 오류 형식 문자열
 * @param ... 가변 인자
 */
void logError(const char* format, ...);

/**
 * @brief 함수 실행을 로그에 기록하는 함수
 * @param funcName 실행된 함수명
 */
void logFunctionExecution(const char* funcName);

/**
 * @brief 로그 시스템을 정리하는 함수
 * @details 로그 파일 닫기 등의 정리 작업
 */
void cleanupLogger();

#endif // LOGGER_H 