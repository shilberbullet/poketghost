#ifndef GAME_H
#define GAME_H

typedef struct {
    int isRunning;
    int isNewGame;
    int isLoadedGame;
} GameState;

extern GameState gameState;

void initGame();

#endif // GAME_H 