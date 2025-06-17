#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

// 플레이어 구조체
typedef struct {
    int money;  // 보유 전
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
void cleanupGame();

#endif // GAME_H 