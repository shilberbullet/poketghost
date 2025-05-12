#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "input.h"
#include "text.h"

#define SETTINGS_FILE "settings.dat"

// 전역 게임 설정 초기화
GameSettings gameSettings = {3};  // 기본값: 보통 속도

// 설정 파일 저장 함수
void saveSettings() {
    FILE* file = fopen(SETTINGS_FILE, "wb");
    if (file != NULL) {
        fwrite(&gameSettings, sizeof(GameSettings), 1, file);
        fclose(file);
    }
}

// 설정 파일 불러오기 함수
void loadSettings() {
    FILE* file = fopen(SETTINGS_FILE, "rb");
    if (file != NULL) {
        fread(&gameSettings, sizeof(GameSettings), 1, file);
        fclose(file);
    }
}

void initSettings() {
    loadSettings();  // 시작할 때 설정 파일 불러오기
}

void showSettingsMenu() {
    int choice;
    
    while (1) {
        system("cls");
        printText("=== 게임 설정 ===\n\n");
        printText("1. 게임 출력 속도\n");
        printText("2. 뒤로 가기\n\n");
        printText("선택하세요 (1-2): ");
        
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
                return;
            default:
                printTextAndWait("\n잘못된 선택입니다. 1-2 사이의 숫자를 입력해주세요.");
                break;
        }
    }
}

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
                printTextAndWait("\n잘못된 선택입니다. 1-6 사이의 숫자를 입력해주세요.");
                break;
        }
    }
} 