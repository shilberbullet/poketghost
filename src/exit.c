// 표준 입출력, 게임 종료 관련 헤더
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "text.h"
#include "exit.h"
#include "../core/state.h"
 
// 게임 종료 함수
void exitGame() {
    gameState.isRunning = 0;  // 게임 루프 종료
    printTextAndWait("\n게임을 종료합니다.");
    exit(0);  // 프로그램 종료
} 