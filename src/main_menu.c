#include <stdio.h>
#include <stdlib.h>
#include "main_menu.h"
#include "game.h"
#include "input.h"
#include "settings.h"
#include "settings_ui.h"
#include "text.h"
#include "stage.h"
#include "party.h"
#include "savefile.h"
#include "exit.h"
#include "reward.h"

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
            exitGame();
            break;
        default:
            printTextAndWait("\n잘못된 선택입니다. 숫자를 입력하세요.");
            break;
    }
}

void startNewGame(void) {
    gameState.isNewGame = 1;  // 새 게임 플래그 설정
    gameState.isLoadedGame = 0;  // 이어하기 플래그 해제
    resetItemRewardSystem(); // 아이템 보상 시스템 상태 초기화
    initGame(); // 게임 상태 초기화
    initStage(&currentStage, 1);  // 첫 번째 스테이지로 시작
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

// 게임 종료 함수 삭제 