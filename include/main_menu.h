#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "game.h"

// 메인 메뉴 옵션 열거형
typedef enum {
    MAIN_MENU_NEW_GAME = 1,
    MAIN_MENU_LOAD_GAME = 2,
    MAIN_MENU_SETTINGS = 3,
    MAIN_MENU_STATISTICS = 4,
    MAIN_MENU_EXIT = 5
} MainMenuOption;

// 메인 메뉴 표시 함수
void showMainMenu(void);

// 메뉴 선택 처리 함수
void handleMainMenuChoice(MainMenuOption choice);

// 새 게임 시작 함수
void startNewGame(void);

// 게임 불러오기 함수
void loadGame(void);

// 게임 설정 함수
void showSettings(void);

#endif // MAIN_MENU_H 