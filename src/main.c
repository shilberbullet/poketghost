#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "menu.h"
#include "input.h"
#include "settings.h"
#include "move.h"
#include "yokai.h"

int main() {
    // 콘솔 출력 인코딩 설정
    system("chcp 65001");
    loadMovesFromFile("move.txt");
    loadYokaiFromFile("yokai.txt");
    initSettings();
    while (1) {
        initGame(); // 게임 루프 상태 초기화
        showMainMenu();
    }
    return 0;
} 