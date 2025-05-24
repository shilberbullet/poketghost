#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "game.h"
#include "input.h"
#include "settings.h"
#include "text.h"
#include "stage.h"
#include "party.h"
#include "savefile.h"

void showGameMenu(void) {
    int choice;
    
    while (1) {
        system("cls");
        printText("=== 게임 메뉴 ===\n\n");
        printText("1. 파티 관리\n");
        printText("2. 아이템 사용\n");
        printText("3. 맵 보기\n");
        printText("4. 게임 저장\n");
        printText("5. 설정\n");
        printText("6. 게임 종료\n\n");
        printText("숫자를 입력하세요: ");
        
        choice = getIntInput();
        if (choice == -1) {
            printTextAndWait("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;
        }
        
        handleGameMenuChoice(choice);
    }
}

void handleGameMenuChoice(int choice) {
    switch (choice) {
        case 1:
            showPartyMenu();
            break;
        case 2:
            showItemMenu();
            break;
        case 3:
            showMap();
            break;
        case 4:
            saveGame();
            break;
        case 5:
            // showSettings(); 함수 완전히 삭제
            break;
        case 6:
            gameState.isRunning = 0;
            printTextAndWait("\n게임을 종료합니다.");
            exit(0);
            break;
        default:
            printTextAndWait("\n잘못된 선택입니다. 1-6 사이의 숫자를 입력하세요.");
            break;
    }
}

void showPartyMenu(void) {}
void showItemMenu(void) {}
void showMap(void) {} 