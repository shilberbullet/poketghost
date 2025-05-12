#include "game.h"

// 전역 게임 상태 초기화
GameState gameState = {0};
 
void initGame() {
    gameState.isRunning = 1;
    gameState.isNewGame = 0;
    gameState.isLoadedGame = 0;
} 