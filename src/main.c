#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "menu.h"
#include "input.h"
#include "settings.h"
#include "move.h"
#include "yokai.h"

int main(void) {
    // 콘솔 출력 인코딩 설정
    system("chcp 65001");
    loadMovesFromFile("data/move.txt");
    loadYokaiFromFile("data/yokai.txt");
    initSettings();
    while (1) {
        initGame(); // 게임 루프 상태 초기화
        showMainMenu();
    }
    return 0;
} 