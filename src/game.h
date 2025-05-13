#ifndef GAME_H
#define GAME_H

typedef struct {
    int isRunning;
    int isNewGame;
    int isLoadedGame;
    int currentStage;
    int currentTime;
} GameState;

extern GameState gameState;

void initGame(void);

#endif // GAME_H 