#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "statistics.h"

// 플레이어 구조체
typedef struct {
    int money;  // 보유 전
    GameStatistics stats;
} Player;

// 게임 상태 구조체
typedef struct {
    bool isRunning;
    int currentStage;
    int currentTime;
    bool isNewGame;
    bool isLoadedGame;
    bool isManualSave;  // 수동 저장 플래그
} GameState;


void initGame(void);
void addMoney(int amount);
void showMoney(void);
void resetGameState();
void cleanupGame(void);

// 게임 클리어 후 데이터 초기화 함수
void resetGameAfterClear(void);

#endif // GAME_H 