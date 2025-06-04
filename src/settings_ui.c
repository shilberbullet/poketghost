// 표준 입출력, 설정 관련 헤더
#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "settings_ui.h"
#include "settings_file.h"
#include "input.h"
#include "text.h"

// 설정 메뉴를 표시하는 함수
void showSettingsMenu() {
    int choice;
    
    while (1) {
        system("cls");
        printText("=== 게임 설정 ===\n\n");
        printText("1. 게임 출력 속도\n");
        printText("2. 텍스트 출력 방식\n");
        printText("3. 뒤로 가기\n\n");
        printText("선택하세요 (1-3): ");
        
        choice = getIntInput();
        if (choice == -1) {
            printTextAndWait("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;
        }
        
        switch (choice) {
            case 1:
                setTextSpeed();
                break;
            case 2:
                setTextDisplayMode();
                break;
            case 3:
                return;
            default:
                printTextAndWait("\n잘못된 선택입니다. 1-3 사이의 숫자를 입력해주세요.");
                break;
        }
    }
}

// 텍스트 출력 속도 설정 함수
void setTextSpeed() {
    int choice;
    
    while (1) {
        system("cls");
        printText("=== 게임 출력 속도 설정 ===\n\n");
        
        // 현재 속도 표시
        printText("현재 속도: ");
        printText(gameSettings.textSpeed == 1 ? "매우 느림" :
                 gameSettings.textSpeed == 2 ? "느림" :
                 gameSettings.textSpeed == 3 ? "보통" :
                 gameSettings.textSpeed == 4 ? "빠름" : "매우 빠름");
        printText("\n\n");
        
        printText("1. 매우 느림\n");
        printText("2. 느림\n");
        printText("3. 보통\n");
        printText("4. 빠름\n");
        printText("5. 매우 빠름\n");
        printText("6. 뒤로 가기\n\n");
        printText("선택하세요 (1-6): ");
        
        choice = getIntInput();
        if (choice == -1) {
            printTextAndWait("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;
        }
        
        switch (choice) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                gameSettings.textSpeed = choice;
                saveSettings();  // 설정 변경 시 파일에 저장
                printTextAndWait("\n게임 출력 속도가 변경되었습니다.");
                return;
            case 6:
                return;
            default:
                printTextAndWait("\n잘못된 선택입니다. 다시 시도하세요.");
                break;
        }
    }
}

// 텍스트 출력 방식 설정 함수
void setTextDisplayMode() {
    int choice;
    
    while (1) {
        system("cls");
        printText("=== 텍스트 출력 방식 설정 ===\n\n");
        
        // 현재 출력 방식 표시
        printText("현재 출력 방식: ");
        printText(gameSettings.textDisplayMode == 0 ? "한 글자씩" :
                 gameSettings.textDisplayMode == 1 ? "한 줄씩" : "한 번에");
        printText("\n\n");
        
        printText("1. 한 글자씩 출력\n");
        printText("2. 한 줄씩 출력\n");
        printText("3. 한 번에 출력\n");
        printText("4. 뒤로 가기\n\n");
        printText("선택하세요 (1-4): ");
        
        choice = getIntInput();
        if (choice == -1) {
            printTextAndWait("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;
        }
        
        switch (choice) {
            case 1:
                gameSettings.textDisplayMode = 0;
                saveSettings();  // 설정 변경 시 파일에 저장
                printTextAndWait("\n텍스트 출력 방식이 '한 글자씩'으로 변경되었습니다.");
                return;
            case 2:
                gameSettings.textDisplayMode = 1;
                saveSettings();  // 설정 변경 시 파일에 저장
                printTextAndWait("\n텍스트 출력 방식이 '한 줄씩'으로 변경되었습니다.");
                return;
            case 3:
                gameSettings.textDisplayMode = 2;
                saveSettings();  // 설정 변경 시 파일에 저장
                printTextAndWait("\n텍스트 출력 방식이 '한 번에'로 변경되었습니다.");
                return;
            case 4:
                return;
            default:
                printTextAndWait("\n잘못된 선택입니다. 다시 시도하세요.");
                break;
        }
    }
} 