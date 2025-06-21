/**
 * @file escape.h
 * @brief 포켓요괴 게임의 도망 시스템 헤더 파일
 * @details 전투 중 도망치기 관련 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef ESCAPE_H
#define ESCAPE_H

#include "types.h" // 기본 타입 정의
#include "stage.h" // 스테이지 관련 헤더

// 도망치기 시도 결과
#define ESCAPE_SUCCESS 103  // 도망 성공
#define ESCAPE_FAIL 0      // 도망 실패

/**
 * @brief 도망치기를 시도하는 함수
 * @return 도망 성공 여부 (ESCAPE_SUCCESS 또는 ESCAPE_FAIL)
 */
int tryToEscape(void);

/**
 * @brief 보스 스테이지에서 도망치기가 가능한지 확인하는 함수
 * @return 도망 가능 여부
 */
int canEscapeFromStage(void);

#endif // ESCAPE_H 