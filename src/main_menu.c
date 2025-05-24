#include <stdio.h>
#include <stdlib.h>
#include "main_menu.h"
#include "game.h"
#include "input.h"
#include "settings.h"
#include "text.h"
#include "stage.h"
#include "party.h"
#include "savefile.h"

void showMainMenu(void) {
    int choice;
    
    while (gameState.isRunning) {
        system("cls");  // 화면 지우기
        printText("=== 포켓요괴 ===\n\n");
        printText("1. 새 게임 시작\n");
        printText("2. 이어하기\n");
        printText("3. 게임 설정\n");
        printText("4. 종료\n\n");
        printText("숫자를 입력하세요: ");
        
        choice = getIntInput();
        if (choice == -1) {
            printTextAndWait("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;
        }
        
        handleMainMenuChoice((MainMenuOption)choice);
    }
}

void handleMainMenuChoice(MainMenuOption choice) {
    switch (choice) {
        case MAIN_MENU_NEW_GAME:
            startNewGame();
            break;
        case MAIN_MENU_LOAD_GAME:
            loadGame();
            break;
        case MAIN_MENU_SETTINGS:
            showSettings();
            break;
        case MAIN_MENU_EXIT:
            gameState.isRunning = 0;
            printTextAndWait("\n게임을 종료합니다.");
            exit(0);
            break;
        default:
            printTextAndWait("\n잘못된 선택입니다. 1-4 사이의 숫자를 입력하세요.");
            break;
    }
}

void startNewGame(void) {
    initGame(); // 게임 상태 초기화
    initStage();
    initParty();
    while (gameState.isRunning) {
        showStageInfo();
        showBattleInterface();
    }
}

void loadGame(void) {
    if (loadGameData()) {
        gameState.isLoadedGame = 1;
        initGame(); // 게임 상태 초기화
        printTextAndWait("\n저장된 게임을 불러왔습니다!");
        while (gameState.isRunning) {
            showStageInfo();
            showBattleInterface();
        }
    } else {
        printTextAndWait("\n저장된 게임 데이터가 없습니다.");
    }
}

void showSettings(void) {
    // 기존 설정 메뉴 표시 함수 호출
    showSettingsMenu();
} 