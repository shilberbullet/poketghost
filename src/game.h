#ifndef GAME_H
#define GAME_H

// 플레이어 구조체
typedef struct {
    int money;  // 보유 전
} Player;

// 게임 상태 구조체
typedef struct {
    int isRunning;
    int isNewGame;
    int isLoadedGame;
    int currentStage;
    int currentTime;
} GameState;

extern GameState gameState;
extern Player player;

void initGame(void);
void addMoney(int amount);
void showMoney(void);

#endif // GAME_H 