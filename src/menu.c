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
#include "../core/state.h"
#include "logger.h"

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

// 아이템 사용 메뉴를 표시하는 함수 (구현 예정)
void showItemMenu(void) {
    LOG_FUNCTION_EXECUTION("showItemMenu");
}

// 맵을 표시하는 함수 (구현 예정)
void showMap(void) {
    LOG_FUNCTION_EXECUTION("showMap");
} 