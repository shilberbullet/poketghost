// 표준 입출력 함수를 위한 헤더
#include <stdio.h> // 표준 입출력 함수
// 표준 라이브러리 함수를 위한 헤더
#include <stdlib.h> // 표준 라이브러리 함수
// 메뉴 관련 함수와 구조체 정의
#include "menu.h" // 메뉴 관련 헤더
// 게임 관련 함수
#include "game.h" // 게임 헤더
// 입력 처리 관련 함수
#include "input.h" // 입력 처리 헤더
// 설정 관련 함수
#include "settings.h" // 설정 헤더
// 텍스트 출력 관련 함수
#include "text.h" // 텍스트 출력 헤더
// 스테이지 관련 함수
#include "stage.h" // 스테이지 헤더
// 파티 관리 관련 함수
#include "party.h" // 파티 관리 헤더
// 저장 파일 관련 함수
#include "savefile.h" // 저장 파일 헤더
// 아이템 관련 함수
#include "item.h" // 아이템 헤더
// 게임 상태 관련 함수
#include "../core/state.h" // 게임 상태 헤더
// 로거 관련 함수
#include "logger.h" // 로거 헤더

// 지도 아스키 아트를 출력하는 함수
void displayMap() { // 지도 표시 함수 시작
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⢶\n"); // 지도 첫 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣏⠀⠻⢆⣀⠀\n"); // 지도 두 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣤⠼⠃⠀⠀⠀⢉⣛⣷⠆⠀⠀⠀\n"); // 지도 세 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣰⠏⠁⠀⠀⠀⠀⣠⡼⠋⠁⠀⠀⠀⠀⠀\n"); // 지도 네 번째 줄
    printText("⠀⠀⠀⠀⠀⠀ ⠀⠀ ⠀⠀⠀⠀⠀⠀  ⠀⠀⠀ ⠀⠀⠀ ⠀⠀⠀⣴⠶⡴⠗⠛⠉⠁⠀⠀⠀⠀⢀⡶⠋⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 다섯 번째 줄
    printText("⠀⠀⠀   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣤⣤⣄⠀⠀⠀⠀⠀⢾⣀⠀백두산 ⠀⠀⠀⣾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 여섯 번째 줄 (백두산 표시)
    printText("⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⠋⠀⠀⠙⢧⣄⡀⠀⠀⣀⣽⠂⠀⠀⠀⠀⠀⠀⠀⠈⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 일곱 번째 줄
    printText(" ⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀  ⠀⣼⠃⠀⠀⠀⠀⣀⡼⠟⠛⠛⠉⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 여덟 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⠀⠀⠀⢀⣤⠟⠁⠀⠀⠀⢰⡟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⢀⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 아홉 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀   ⠀⢀⣤⠾⠋⠁⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⣀⣤⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 열 번째 줄
    printText("⠀⠀⠀⠀⠀    ⢀⣠⡶⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⡷⠀⠀⠀  함경도 ⠀⠀⢀⣴⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 열한 번째 줄 (함경도 표시)
    printText("⠀⠀   ⠀⢀⣤⠞⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 열두 번째 줄
    printText(" ⠀⠀⠀⣰⠟⠁⠀⠀⠀⠀  평안도⠀⠀⠀⠀⠀⣷⠀⠀⠀⠀⠀⠀  ⠀⢈⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 열세 번째 줄 (평안도 표시)
    printText("⠀⠀⠀⠀⠛⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⠷⡯⠀⠀⠀⠀⠀⠀⠀⣠⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 열네 번째 줄
    printText("⠀⠀⠀⠀⠀⠘⢷⠟⠶⣦⠶⣶⠀⠀⠀⠀⠀⠀⠐⢿⡁⠀⠀⠀⠀⠀⠀⠀⠀⢰⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 열다섯 번째 줄
    printText(" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡯⠀⠀⠀⠀⠀⠀⠀⠸⣇⡀⠀⠀⠀⠀⠀⠀⢠⣾⡅⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 열여섯 번째 줄
    printText("⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⣾⠁⠀⠀⠀⠀⠀  ⣀⣄⣈⢷⠀⠀⠀⠀⠀⣠⡾⠛⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 열일곱 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⠀⠀⠀⠀⢰⡶⡴⠏⠈⠙⢻⣷⢾⡄⢀⡾⠋⠀⠀⢹⣄⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 열여덟 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡷⠲⠚⠻⠶⠶⠋⠀⠀⠀⣴⠛⠀⠈⠷⠋⠁⠀⠀⠀⠀⠹⡆⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 열아홉 번째 줄
    printText("⠀⠀⠀⠀⠀ ⠀⣄⡶⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢧⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢻⣄⠀⠀⠀⠀  ⢀⣄⣤⣄⠀⠀\n"); // 지도 스무 번째 줄
    printText("⠀⠀⠀ ⠀⠀⢨⡏⠀⠀⠀ 황해도⠀⠀⢀⣤⠾⠏⢿⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠹⣆⠀⠀⠀⠀⢸⡇⠁⠹⣆⠀⠀⠰⠆⠀\n"); // 지도 스물한 번째 줄 (황해도 표시)
    printText("⠀⠀ ⠀⠀⠀⠈⠙⢳⣤⣄⠀⠀⣠⡀⠀⠀⢰⡏⠁⠀⠀⠀⠉⠻⢦⡄⠀강원도⠀ ⠙⣦⠀⠀⠀⠀⠻⠦⡴⠋⠀\n"); // 지도 스물두 번째 줄 (강원도 표시)
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⢻⢤⠾⠋⠙⢻⣔⠞⠃⠀⠀⠀⠀⠀⠀⢿⣀⠀⠀⠀⠀⠀⠀⠀⠀ ⠘⣗⠀⠀⠀⠀⠀\n"); // 지도 스물세 번째 줄
    printText("⠀  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣀⡀경기도  ⢙⡇⠀⠀⠀⠀  ⠀⠀ ⠀⠹⣆⠀⠀\n"); // 지도 스물네 번째 줄 (경기도 표시)
    printText("⠀⠀⠀⠀⠀⠀  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢉⣏⠀⠀⠀⠀⠀⠀⣴⢇⣄⣠⣀⡀⠀⠀ ⠀⠀ ⠀⢹⡄⠀\n"); // 지도 스물다섯 번째 줄
    printText("  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⡙⣷⣀⣤⣄⣠⡾⠋⠉⠈⠈⠉⢙⣿⠶⠴⢶⣄⠈⣷⠀\n"); // 지도 스물여섯 번째 줄
    printText("⠀⠀⠀⠀⠀  ⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⣴⠟⠉⠉⠁⠉⠀⠉⠁⠀⠀⠀⣀⡴⠞⠞⠁⠀⠀⠀⠙⢦⣽⠀⠀\n"); // 지도 스물일곱 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢷⣤⣄⠀ 충청도 ⠀⢰⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⣽⠀\n"); // 지도 스물여덟 번째 줄 (충청도 표시)
    printText("⠀⠀  ⠀⠀⠀⠀⠀  ⠀⠀⠀⠀ ⠀⠀⠀⢷⠀⠀⠀⠀⠀⣄⣀⠀⠘⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣰⢶⠀⠀\n"); // 지도 스물아홉 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣠⣾⠴⠶⠞⠋⠙⠷⣄⣽⠇⠀⠀⠀⠀⠀⠀⠀⠀⠙⠃⣺⠁⠀\n"); // 지도 서른 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀  ⠀⠀⠀⠀⣻⠄⠀⠀⠀⠀⠀⢀⣼⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⠟⠀⠀⠀⠀\n"); // 지도 서른한 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀  ⠀⠀⠀⠀⢀⡾⠋⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀경상도   ⠀⢸⡟⠀\n"); // 지도 서른두 번째 줄 (경상도 표시)
    printText("⠀⠀   ⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡾⠁⠀⠀⠀⠀⠀⠀⠀⣗⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡟⠀\n"); // 지도 서른세 번째 줄
    printText("⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⠀⠀⠀⠀⠸⡇⠀전라도⠀⠀⣷⠀⠀⠀⠀⠀⠀⢀⣠⣤⡴⠟⠀⠀⠀\n"); // 지도 서른네 번째 줄 (전라도 표시)
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣤⡨⡇⠀⠀⠀⠀⠀⠀⢀⣠⣿⠁⠀⢀⣤⣄⡀⢠⡗⠀⠀⠀⠀\n"); // 지도 서른다섯 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠸⣏⠛⠁⠀⠀⠀⢀⠀⠀⢽⡷⣍⣟⠳⠞⠁⠉⠙⠛⠁⠀⠀\n"); // 지도 서른여섯 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠻⣄⣤⡄⠀⠀⣾⠳⣦⣸⠇⠙⠃⠀⠀⠀\n"); // 지도 서른일곱 번째 줄
    printText("⠀  ⠀⠀⠀⠀  ⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠙⣦⠾⠃⠀⠈⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 서른여덟 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"); // 지도 서른아홉 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n"); // 지도 마지막 줄
    printText(" ⠀⠀⠀⠀⠀⠀  ⠀⠀⠀⠀ ⠀⠀⣀⡴⣾⠿⠷⠦⠶⡤⣤⡀⠀\n"); // 제주도 첫 번째 줄
    printText("⠀⠀⠀⠀ ⠀ ⠀⠀⠀⠀⠀⠀⠀⣰⠏⠀ 제주도⠀⠀⣨⠀⠀\n"); // 제주도 두 번째 줄 (제주도 표시)
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⡄       ⣀⣴⠏⠀⠀\n"); // 제주도 세 번째 줄
    printText("⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⠀⠈⠙⠛⠶⠛⠋⠋⠉⠀⠀⠀⠀\n"); // 제주도 네 번째 줄
    printText("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\n"); // 제주도 마지막 줄
}

// 게임 메뉴를 표시하는 함수
void showGameMenu(void) { // 게임 메뉴 표시 함수 시작
    LOG_FUNCTION_EXECUTION("showGameMenu"); // 함수 실행 로그 기록
    int choice; // 선택 변수
    
    while (1) { // 무한 루프
        system("cls");  // 화면 지우기
        printText("=== 게임 메뉴 ===\n\n"); // 메뉴 제목 출력
        printText("1. 파티 관리\n");      // 파티 관리 메뉴
        printText("2. 아이템 사용\n");    // 아이템 사용 메뉴
        printText("3. 맵 보기\n");        // 맵 보기 메뉴
        printText("4. 게임 저장\n");      // 게임 저장 메뉴
        printText("5. 설정\n");           // 설정 메뉴
        printText("6. 게임 종료\n\n");    // 게임 종료 메뉴
        printText("숫자를 입력해주세요: ");  // 사용자 입력 요청
        
        choice = getIntInput();  // 정수 입력 받기
        if (choice == -1) { // 잘못된 입력인 경우
            printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
            continue; // 다시 루프 시작
        }
        
        handleGameMenuChoice(choice);  // 메뉴 선택 처리
    }
}

// 게임 메뉴 선택을 처리하는 함수
void handleGameMenuChoice(int choice) { // 게임 메뉴 선택 처리 함수 시작
    LOG_FUNCTION_EXECUTION("handleGameMenuChoice"); // 함수 실행 로그 기록
    switch (choice) { // 선택에 따른 분기
        case 1: // 파티 관리 선택
            showPartyMenu();  // 파티 관리 메뉴 표시
            break; // 케이스 종료
        case 2: // 아이템 사용 선택
            showItemMenu();   // 아이템 사용 메뉴 표시
            break; // 케이스 종료
        case 3: // 맵 보기 선택
            showMap();        // 맵 보기 메뉴 표시
            break; // 케이스 종료
        case 4: // 게임 저장 선택
            saveGame();       // 게임 저장
            break; // 케이스 종료
        case 5: // 설정 선택
            // showSettings(); 함수 완전히 삭제
            break; // 케이스 종료
        case 6: // 게임 종료 선택
            gGameState.isRunning = 0;  // 게임 실행 상태 종료
            printTextAndWait("\n게임을 종료합니다.");  // 종료 메시지 출력
            exit(0);  // 프로그램 종료
            break; // 케이스 종료
        default: // 기본 케이스
            printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");  // 잘못된 선택 처리
            break; // 케이스 종료
    }
}

// 파티 관리 메뉴를 표시하는 함수 (구현 예정)
void showPartyMenu(void) { // 파티 관리 메뉴 표시 함수 시작
    LOG_FUNCTION_EXECUTION("showPartyMenu"); // 함수 실행 로그 기록
}

// 아이템 사용 메뉴를 표시하는 함수
void showItemMenu(void) { // 아이템 사용 메뉴 표시 함수 시작
    LOG_FUNCTION_EXECUTION("showItemMenu"); // 함수 실행 로그 기록
    int choice; // 선택 변수

    while (1) { // 무한 루프
        system("cls"); // 화면 지우기
        printText("=== 인벤토리 ===\n"); // 인벤토리 제목 출력

        if (inventoryCount == 0) { // 인벤토리가 비어있는 경우
            printText("인벤토리가 비어있습니다.\n"); // 빈 인벤토리 메시지 출력
        } else { // 인벤토리에 아이템이 있는 경우
            for (int i = 0; i < inventoryCount; i++) { // 모든 인벤토리 아이템 출력
                char gradeStr[10]; // 등급 문자열 버퍼
                const char* colorCode; // 색상 코드 변수
                switch (inventory[i].item.grade) { // 아이템 등급에 따른 색상 설정
                    case ITEM_COMMON: // 일반 등급
                        strcpy(gradeStr, "[일반]"); // 등급 문자열 설정
                        colorCode = "\033[0m"; // 기본색
                        break; // 케이스 종료
                    case ITEM_RARE: // 희귀 등급
                        strcpy(gradeStr, "[희귀]"); // 등급 문자열 설정
                        colorCode = "\033[33m"; // 노란색
                        break; // 케이스 종료
                    case ITEM_SUPERRARE: // 초희귀 등급
                        strcpy(gradeStr, "[초희귀]"); // 등급 문자열 설정
                        colorCode = "\033[31m"; // 빨간색
                        break; // 케이스 종료
                    default: // 기본 등급
                        strcpy(gradeStr, ""); // 등급 문자열 초기화
                        colorCode = "\033[0m"; // 기본색
                        break; // 케이스 종료
                }
                char itemLine[256]; // 아이템 라인 버퍼
                snprintf(itemLine, sizeof(itemLine), "%d. %s%s\033[0m %s x%d\n", i + 1, colorCode, inventory[i].item.name, gradeStr, inventory[i].count); // 아이템 정보 문자열 생성
                printText(itemLine); // 아이템 정보 출력
            }
        }
        
        char backLine[50]; // 뒤로가기 라인 버퍼
        snprintf(backLine, sizeof(backLine), "%d. 뒤로가기\n", inventoryCount + 1); // 뒤로가기 문자열 생성
        printText(backLine); // 뒤로가기 출력
        
        printText("\n아이템 번호를 선택하세요: "); // 선택 안내 메시지 출력
        choice = getIntInput(); // 정수 입력 받기

        if (choice > 0 && choice <= inventoryCount) { // 유효한 아이템 선택인 경우
            system("cls"); // 화면 지우기

            // 아이템 이름이 "지도"인 경우 아스키 아트 출력
            if (strcmp(inventory[choice - 1].item.name, "지도") == 0) { // 지도 아이템인 경우
                displayMap(); // 지도 표시
            }

            char descText[256]; // 설명 텍스트 버퍼
            snprintf(descText, sizeof(descText), "아이템: %s\n\n설명: %s\n\n", inventory[choice - 1].item.name, inventory[choice - 1].item.desc); // 아이템 설명 문자열 생성
            printText(descText); // 아이템 설명 출력
            printText("엔터를 눌러 돌아가기..."); // 안내 메시지 출력
            clearInputBuffer(); // 입력 버퍼 비우기
            while (getchar() != '\n'); // 엔터 입력 대기
        } else if (choice == inventoryCount + 1) { // 뒤로가기 선택인 경우
            break; // 루프 종료
        } else { // 잘못된 선택인 경우
            printTextAndWait("잘못된 선택입니다. 다시 입력해주세요."); // 오류 메시지 출력
        }
    }
}

// 맵을 표시하는 함수 (구현 예정)
void showMap(void) { // 맵 표시 함수 시작
    LOG_FUNCTION_EXECUTION("showMap"); // 함수 실행 로그 기록
} 