// 표준 입출력, 게임 및 메뉴 관련 헤더
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
#include "region.h"

// 메인 메뉴를 표시하는 함수
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

// 메인 메뉴 선택을 처리하는 함수
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
            printTextAndWait("\n잘못된 선택입니다. 다시 시도하세요.");
            break;
    }
}

// 새 게임 시작 함수
void startNewGame(void) {
    gameState.isNewGame = 1;  // 새 게임 플래그 설정
    gameState.isLoadedGame = 0;  // 이어하기 플래그 해제
    resetItemRewardSystem(); // 아이템 보상 시스템 상태 초기화
    
    // 시작 지역 선택
    system("cls");
    printText("=== 시작 지역 선택 ===\n\n");
    printText("1. 경상도\n");
    printText("2. 전라도\n\n");
    printText("숫자를 입력하세요: ");
    
    int choice = getIntInput();
    while (choice != 1 && choice != 2) {
        printText("\n잘못된 입력입니다. 1 또는 2를 입력해주세요: ");
        choice = getIntInput();
    }
    
    if (!setInitialRegion(choice)) {
        printTextAndWait("\n지역 설정에 실패했습니다.");
        return;
    }
    
    initGame(); // 게임 상태 초기화
    initStage(&currentStage, 1);  // 첫 번째 스테이지로 시작
    initParty();
    
    char buffer[256];
    sprintf(buffer, "\n%s에서 모험이 시작됩니다!\n", getCurrentRegion());
    printText(buffer);
    printTextAndWait("아무 키나 누르면 계속합니다...");
    
    while (gameState.isRunning) {
        showStageInfo();
        showBattleInterface();
    }
}

// 저장된 게임 불러오기 함수
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

// 설정 메뉴 표시 함수
void showSettings(void) {
    // 기존 설정 메뉴 표시 함수 호출
    showSettingsMenu();
}

// 게임 종료 함수는 exit.c에서 관리 