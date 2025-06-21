// 표준 입출력, 메뉴 및 게임 관련 헤더
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
#include "item.h"
#include "../core/state.h"
#include "logger.h"

// 지도 아스키 아트를 출력하는 함수
void displayMap() {
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⢶\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣏⠀⠻⢆⣀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣤⠼⠃⠀⠀⠀⢉⣛⣷⠆⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣰⠏⠁⠀⠀⠀⠀⣠⡼⠋⠁⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀ ⠀⠀ ⠀⠀⠀⠀⠀⠀  ⠀⠀⠀ ⠀⠀⠀ ⠀⠀⠀⣴⠶⡴⠗⠛⠉⠁⠀⠀⠀⠀⢀⡶⠋⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣤⣤⣄⠀⠀⠀⠀⠀⢾⣀⠀백두산 ⠀⠀⠀⣾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⠋⠀⠀⠙⢧⣄⡀⠀⠀⣀⣽⠂⠀⠀⠀⠀⠀⠀⠀⠈⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText(" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀  ⠀⣼⠃⠀⠀⠀⠀⣀⡼⠟⠛⠛⠉⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⠀⠀⠀⢀⣤⠟⠁⠀⠀⠀⢰⡟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⢀⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀   ⠀⢀⣤⠾⠋⠁⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⣀⣤⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀    ⢀⣠⡶⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⡷⠀⠀⠀  함경도 ⠀⠀⢀⣴⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀   ⠀⢀⣤⠞⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText(" ⠀⠀⠀⣰⠟⠁⠀⠀⠀⠀  평안도⠀⠀⠀⠀⠀⣷⠀⠀⠀⠀⠀⠀  ⠀⢈⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠛⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⠷⡯⠀⠀⠀⠀⠀⠀⠀⣠⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠘⢷⠟⠶⣦⠶⣶⠀⠀⠀⠀⠀⠀⠐⢿⡁⠀⠀⠀⠀⠀⠀⠀⠀⢰⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText(" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡯⠀⠀⠀⠀⠀⠀⠀⠸⣇⡀⠀⠀⠀⠀⠀⠀⢠⣾⡅⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⣾⠁⠀⠀⠀⠀⠀  ⣀⣄⣈⢷⠀⠀⠀⠀⠀⣠⡾⠛⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⠀⠀⠀⠀⢰⡶⡴⠏⠈⠙⢻⣷⢾⡄⢀⡾⠋⠀⠀⢹⣄⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡷⠲⠚⠻⠶⠶⠋⠀⠀⠀⣴⠛⠀⠈⠷⠋⠁⠀⠀⠀⠀⠹⡆⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀ ⠀⣄⡶⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢧⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢻⣄⠀⠀⠀⠀  ⢀⣄⣤⣄⠀⠀\n");
    printText("⠀⠀⠀ ⠀⠀⢨⡏⠀⠀⠀ 황해도⠀⠀⢀⣤⠾⠏⢿⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠹⣆⠀⠀⠀⠀⢸⡇⠁⠹⣆⠀⠀⠰⠆⠀\n");
    printText("⠀⠀ ⠀⠀⠀⠈⠙⢳⣤⣄⠀⠀⣠⡀⠀⠀⢰⡏⠁⠀⠀⠀⠉⠻⢦⡄⠀강원도⠀ ⠙⣦⠀⠀⠀⠀⠻⠦⡴⠋⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⢻⢤⠾⠋⠙⢻⣔⠞⠃⠀⠀⠀⠀⠀⠀⢿⣀⠀⠀⠀⠀⠀⠀⠀⠀ ⠘⣗⠀⠀⠀⠀⠀\n");
    printText("⠀  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣀⡀경기도  ⢙⡇⠀⠀⠀⠀  ⠀⠀ ⠀⠹⣆⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢉⣏⠀⠀⠀⠀⠀⠀⣴⢇⣄⣠⣀⡀⠀⠀ ⠀⠀ ⠀⢹⡄⠀\n");
    printText("  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⡙⣷⣀⣤⣄⣠⡾⠋⠉⠈⠈⠉⢙⣿⠶⠴⢶⣄⠈⣷⠀\n");
    printText("⠀⠀⠀⠀⠀  ⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⣴⠟⠉⠉⠁⠉⠀⠉⠁⠀⠀⠀⣀⡴⠞⠞⠁⠀⠀⠀⠙⢦⣽⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢷⣤⣄⠀ 충청도 ⠀⢰⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⣽⠀\n");
    printText("⠀⠀  ⠀⠀⠀⠀⠀  ⠀⠀⠀⠀ ⠀⠀⠀⢷⠀⠀⠀⠀⠀⣄⣀⠀⠘⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣰⢶⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣠⣾⠴⠶⠞⠋⠙⠷⣄⣽⠇⠀⠀⠀⠀⠀⠀⠀⠀⠙⠃⣺⠁⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀  ⠀⠀⠀⠀⣻⠄⠀⠀⠀⠀⠀⢀⣼⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⠟⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀  ⠀⠀⠀⠀⢀⡾⠋⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀경상도   ⠀⢸⡟⠀\n");
    printText("⠀⠀   ⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡾⠁⠀⠀⠀⠀⠀⠀⠀⣗⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡟⠀\n");
    printText("⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⠀⠀⠀⠀⠸⡇⠀전라도⠀⠀⣷⠀⠀⠀⠀⠀⠀⢀⣠⣤⡴⠟⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣤⡨⡇⠀⠀⠀⠀⠀⠀⢀⣠⣿⠁⠀⢀⣤⣄⡀⢠⡗⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠸⣏⠛⠁⠀⠀⠀⢀⠀⠀⢽⡷⣍⣟⠳⠞⠁⠉⠙⠛⠁⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠻⣄⣤⡄⠀⠀⣾⠳⣦⣸⠇⠙⠃⠀⠀⠀\n");
    printText("⠀  ⠀⠀⠀⠀  ⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠙⣦⠾⠃⠀⠈⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n");
    printText(" ⠀⠀⠀⠀⠀⠀  ⠀⠀⠀⠀ ⠀⠀⣀⡴⣾⠿⠷⠦⠶⡤⣤⡀⠀\n");
    printText("⠀⠀⠀⠀ ⠀ ⠀⠀⠀⠀⠀⠀⠀⣰⠏⠀ 제주도⠀⠀⣨⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⡄       ⣀⣴⠏⠀⠀\n");
    printText("⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⠀⠈⠙⠛⠶⠛⠋⠋⠉⠀⠀⠀⠀\n");
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\n");
}

// 게임 메뉴를 표시하는 함수
void showGameMenu(void) {
    LOG_FUNCTION_EXECUTION("showGameMenu");
    int choice;
    
    while (1) {
        system("cls");  // 화면 지우기
        printText("=== 게임 메뉴 ===\n\n");
        printText("1. 파티 관리\n");      // 파티 관리 메뉴
        printText("2. 아이템 사용\n");    // 아이템 사용 메뉴
        printText("3. 맵 보기\n");        // 맵 보기 메뉴
        printText("4. 게임 저장\n");      // 게임 저장 메뉴
        printText("5. 설정\n");           // 설정 메뉴
        printText("6. 게임 종료\n\n");    // 게임 종료 메뉴
        printText("숫자를 입력해주세요: ");  // 사용자 입력 요청
        
        choice = getIntInput();  // 정수 입력 받기
        if (choice == -1) {
            printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
            continue;
        }
        
        handleGameMenuChoice(choice);  // 메뉴 선택 처리
    }
}

// 게임 메뉴 선택을 처리하는 함수
void handleGameMenuChoice(int choice) {
    LOG_FUNCTION_EXECUTION("handleGameMenuChoice");
    switch (choice) {
        case 1:
            showPartyMenu();  // 파티 관리 메뉴 표시
            break;
        case 2:
            showItemMenu();   // 아이템 사용 메뉴 표시
            break;
        case 3:
            showMap();        // 맵 보기 메뉴 표시
            break;
        case 4:
            saveGame();       // 게임 저장
            break;
        case 5:
            // showSettings(); 함수 완전히 삭제
            break;
        case 6:
            gGameState.isRunning = 0;  // 게임 실행 상태 종료
            printTextAndWait("\n게임을 종료합니다.");  // 종료 메시지 출력
            exit(0);  // 프로그램 종료
            break;
        default:
            printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");  // 잘못된 선택 처리
            break;
    }
}

// 파티 관리 메뉴를 표시하는 함수 (구현 예정)
void showPartyMenu(void) {
    LOG_FUNCTION_EXECUTION("showPartyMenu");
}

// 아이템 사용 메뉴를 표시하는 함수
void showItemMenu(void) {
    LOG_FUNCTION_EXECUTION("showItemMenu");
    int choice;

    while (1) {
        system("cls");
        printText("=== 인벤토리 ===\n");

        if (inventoryCount == 0) {
            printText("인벤토리가 비어있습니다.\n");
        } else {
            for (int i = 0; i < inventoryCount; i++) {
                char gradeStr[10];
                const char* colorCode;
                switch (inventory[i].item.grade) {
                    case ITEM_COMMON:
                        strcpy(gradeStr, "[일반]");
                        colorCode = "\033[0m"; // 기본색
                        break;
                    case ITEM_RARE:
                        strcpy(gradeStr, "[희귀]");
                        colorCode = "\033[33m"; // 노란색
                        break;
                    case ITEM_SUPERRARE:
                        strcpy(gradeStr, "[초희귀]");
                        colorCode = "\033[31m"; // 빨간색
                        break;
                    default:
                        strcpy(gradeStr, "");
                        colorCode = "\033[0m";
                        break;
                }
                char itemLine[256];
                snprintf(itemLine, sizeof(itemLine), "%d. %s%s\033[0m %s x%d\n", i + 1, colorCode, inventory[i].item.name, gradeStr, inventory[i].count);
                printText(itemLine);
            }
        }
        
        char backLine[50];
        snprintf(backLine, sizeof(backLine), "%d. 뒤로가기\n", inventoryCount + 1);
        printText(backLine);
        
        printText("\n아이템 번호를 선택하세요: ");
        choice = getIntInput();

        if (choice > 0 && choice <= inventoryCount) {
            system("cls");

            // 아이템 이름이 "지도"인 경우 아스키 아트 출력
            if (strcmp(inventory[choice - 1].item.name, "지도") == 0) {
                displayMap();
            }

            char descText[256];
            snprintf(descText, sizeof(descText), "아이템: %s\n\n설명: %s\n\n", inventory[choice - 1].item.name, inventory[choice - 1].item.desc);
            printText(descText);
            printText("엔터를 눌러 돌아가기...");
            clearInputBuffer();
            while (getchar() != '\n');
        } else if (choice == inventoryCount + 1) {
            break;
        } else {
            printTextAndWait("잘못된 선택입니다. 다시 입력해주세요.");
        }
    }
}

// 맵을 표시하는 함수 (구현 예정)
void showMap(void) {
    LOG_FUNCTION_EXECUTION("showMap");
} 