#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "text.h"
#include "exit.h"
 
void exitGame() {
    gameState.isRunning = 0;
    printTextAndWait("\n게임을 종료합니다.");
    exit(0);
} 