/**
 * @file game.h
 * @brief 포켓요괴 게임의 핵심 게임 로직 헤더 파일
 * @details 게임 상태, 플레이어 정보, 게임 초기화 등의 함수를 선언합니다.
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef GAME_H
#define GAME_H

#include <stdbool.h> // 불린 타입 사용을 위한 헤더
#include "statistics.h" // 게임 통계 관련 헤더

// 플레이어 구조체
typedef struct {
    int money;  // 보유 전화
    GameStatistics stats; // 플레이어 통계 정보
} Player; // 플레이어 정보를 담는 구조체

// 게임 상태 구조체
typedef struct {
    bool isRunning; // 게임 실행 상태
    int currentStage; // 현재 스테이지 번호
    int currentTime; // 현재 게임 시간
    bool isNewGame; // 새 게임 여부
    bool isLoadedGame; // 로드된 게임 여부
    bool isManualSave;  // 수동 저장 플래그
    bool skipStageInfo; // 이벤트 처리 후 스테이지 정보 출력 건너뛰기 플래그
    bool skipCurrentStageAutoSave; // 현재 스테이지 자동 저장 건너뛰기 플래그
} GameState; // 게임의 전반적인 상태를 관리하는 구조체

/**
 * @brief 게임을 초기화하는 함수
 * @details 게임 시작 시 필요한 모든 시스템을 초기화합니다.
 */
void initGame(void);

/**
 * @brief 플레이어의 전화를 증가시키는 함수
 * @param amount 증가시킬 전화 양
 */
void addMoney(int amount);

/**
 * @brief 현재 보유 전화를 화면에 표시하는 함수
 */
void showMoney(void);

/**
 * @brief 게임 상태를 초기화하는 함수
 * @details 게임 재시작 시 상태를 리셋합니다.
 */
void resetGameState();

/**
 * @brief 게임 종료 시 정리 작업을 수행하는 함수
 * @details 메모리 해제 등의 정리 작업을 수행합니다.
 */
void cleanupGame(void);

// 게임 클리어 후 데이터 초기화 함수
/**
 * @brief 게임 클리어 후 데이터를 초기화하는 함수
 * @details 게임 클리어 후 다음 게임을 위한 초기화를 수행합니다.
 */
void resetGameAfterClear(void);

#endif // GAME_H 