#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "game_db.h"
#include "battle_system.h"

// 외부 변수 선언
extern GameSpeed gameSpeeds[];
extern int speedCount;
extern int currentSpeedIndex;
extern SaveData currentGameData;

// 외부 함수 선언
extern void startGame();  // 메인 게임 시작 함수
extern void loadGameSpeeds();  // 게임 속도 설정 로드
extern void continueGame(int stageNumber, int gameTime, const char* location, const char* terrain, int savedSpeed);  // 게임 이어하기 함수
extern void printCharByChar(const char* str);
extern void saveGame(int stageNumber, int gameTime, const char* location, const char* terrain, int gameSpeed);
extern SaveData* loadGame();

// 한 글자씩 출력하는 함수
void printCharByChar(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        printf("%c", str[i]);
        Sleep(gameSpeeds[currentSpeedIndex].charDelay);
    }
    printf("\n");
    Sleep(gameSpeeds[currentSpeedIndex].lineDelay);
}

// 게임 시작 화면 표시
void displayStartScreen() {
    system("cls");
    printCharByChar("\n=== 포켓 요괴 ===");
    printCharByChar("\n1. 새 게임 시작");
    printCharByChar("2. 이어하기");
    printCharByChar("3. 게임 설정");
    printCharByChar("4. 종료");
    printCharByChar("\n선택: ");
}

// 게임 설정 화면 표시
void displaySettings() {
    system("cls");
    printCharByChar("\n=== 게임 설정 ===");
    printCharByChar("\n게임 속도를 선택하세요:");
    printCharByChar("1. 0.5배속");
    printCharByChar("2. 1배속 (기본)");
    printCharByChar("3. 2배속");
    printCharByChar("4. 4배속");
    printCharByChar("\n선택: ");
}

// 게임 설정 처리
void handleSettings() {
    int choice;
    displaySettings();
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            currentSpeedIndex = 0;  // 0.5배속
            printCharByChar("\n게임 속도가 0.5배속으로 설정되었습니다.");
            break;
        case 2:
            currentSpeedIndex = 1;  // 1배속
            printCharByChar("\n게임 속도가 1배속으로 설정되었습니다.");
            break;
        case 3:
            currentSpeedIndex = 2;  // 2배속
            printCharByChar("\n게임 속도가 2배속으로 설정되었습니다.");
            break;
        case 4:
            currentSpeedIndex = 3;  // 4배속
            printCharByChar("\n게임 속도가 4배속으로 설정되었습니다.");
            break;
        default:
            printCharByChar("\n잘못된 선택입니다. 기본 속도(1배속)로 설정됩니다.");
            currentSpeedIndex = 1;
    }
    
    // 설정 변경 후 즉시 저장
    saveGame(1, 0, "시작", "평지", currentSpeedIndex);
    Sleep(1000);
}

// 게임 설정 함수
void gameSettings() {
    while (1) {
        system("cls");
        printCharByChar("\n=== 게임 설정 ===");
        char buffer[100];
        sprintf(buffer, "\n현재 게임 속도: %s", gameSpeeds[currentSpeedIndex].name);
        printCharByChar(buffer);
        printCharByChar("\n\n1. 게임 속도 변경");
        printCharByChar("2. 돌아가기");
        printCharByChar("\n선택: ");

        int choice;
        scanf("%d", &choice);
        getchar();  // 버퍼 비우기

        if (choice == 1) {
            system("cls");
            printCharByChar("\n=== 게임 속도 설정 ===");
            for (int i = 0; i < speedCount; i++) {
                sprintf(buffer, "\n%d. %s", i + 1, gameSpeeds[i].name);
                printCharByChar(buffer);
            }
            printCharByChar("\n선택: ");

            int speedChoice;
            scanf("%d", &speedChoice);
            getchar();  // 버퍼 비우기

            if (speedChoice >= 1 && speedChoice <= speedCount) {
                currentSpeedIndex = speedChoice - 1;
                
                // 현재 게임 상태 저장
                SaveData* currentSave = loadGame();
                if (currentSave) {
                    saveGame(currentSave->stageNumber, 
                            currentSave->time,
                            currentSave->lastLocation,
                            currentSave->lastTerrain,
                            currentSpeedIndex);
                    free(currentSave);
                } else {
                    // 저장된 게임이 없어도 속도 설정은 저장
                    saveGame(1, 0, "시작", "평지", currentSpeedIndex);
                }
                
                printCharByChar("\n게임 속도가 변경되었습니다!");
                Sleep(1000);
            } else {
                printCharByChar("\n잘못된 선택입니다!");
                Sleep(1000);
            }
        } else if (choice == 2) {
            break;
        } else {
            printCharByChar("\n잘못된 선택입니다!");
            Sleep(1000);
        }
    }
}

// 메인 메뉴 함수
void mainMenu() {
    while (1) {
        system("cls");
        printCharByChar("\n=== 포켓요괴 ===");
        printCharByChar("\n1. 새 게임 시작");
        printCharByChar("2. 이어하기");
        printCharByChar("3. 게임 설정");
        printCharByChar("4. 종료");
        printCharByChar("\n선택: ");

        int choice;
        scanf("%d", &choice);
        getchar();  // 버퍼 비우기

        switch (choice) {
            case 1: {
                // 저장된 게임 속도 불러오기
                SaveData* savedData = loadGame();
                if (savedData) {
                    currentSpeedIndex = savedData->gameSpeed;
                    free(savedData);
                }
                startGame();
                break;
            }
            case 2: {
                SaveData* saveData = loadGame();
                if (saveData) {
                    currentSpeedIndex = saveData->gameSpeed;  // 저장된 게임 속도 적용
                    continueGame(saveData->stageNumber, saveData->time, 
                               saveData->lastLocation, saveData->lastTerrain,
                               saveData->gameSpeed);
                    free(saveData);
                } else {
                    printCharByChar("\n저장된 게임이 없습니다!");
                    Sleep(2000);
                }
                break;
            }
            case 3:
                gameSettings();
                break;
            case 4:
                printCharByChar("\n게임을 종료합니다.");
                return;
            default:
                printCharByChar("\n잘못된 선택입니다!");
                Sleep(1000);
        }
    }
}

int main() {
    system("chcp 65001");  // UTF-8 인코딩 설정
    loadGameSpeeds();  // 게임 속도 설정 로드
    
    // 저장된 게임 속도 불러오기
    SaveData* savedData = loadGame();
    if (savedData) {
        currentSpeedIndex = savedData->gameSpeed;
        free(savedData);
    }
    
    mainMenu();
    return 0;
} 