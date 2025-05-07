#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
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
    // 입력 버퍼 비우기
    while (_kbhit()) {
        _getch();
    }
    
    for (int i = 0; str[i] != '\0'; i++) {
        printf("%c", str[i]);
        Sleep(gameSpeeds[currentSpeedIndex].charDelay);
        
        // 출력 중에 입력된 키 무시
        while (_kbhit()) {
            _getch();
        }
    }
    printf("\n");
    Sleep(gameSpeeds[currentSpeedIndex].lineDelay);
    
    // 줄바꿈 후에도 입력 버퍼 비우기
    while (_kbhit()) {
        _getch();
    }
}

// 게임 속도 저장 함수
void saveGameSpeed(int speedIndex) {
    SaveData* currentSave = loadGame();
    if (currentSave) {
        // 현재 게임 상태가 있으면 그대로 유지하면서 속도만 업데이트
        saveGame(currentSave->stageNumber, 
                currentSave->time,
                currentSave->lastLocation,
                currentSave->lastTerrain,
                speedIndex);
        free(currentSave);
    } else {
        // 저장된 게임이 없으면 새 게임 상태로 저장
        saveGame(1, 0, "시작", "평지", speedIndex);
    }
}

// 게임 속도 로드 함수
void loadGameSpeed() {
    SaveData* savedData = loadGame();
    if (savedData) {
        currentSpeedIndex = savedData->gameSpeed;
        free(savedData);
    }
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
                saveGameSpeed(currentSpeedIndex);  // 속도 변경 즉시 저장
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

        char input[10];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }

        char* endptr;
        int choice = strtol(input, &endptr, 10);
        
        if (*endptr != '\n' && *endptr != '\0') {
            printCharByChar("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            Sleep(1000);
            continue;
        }

        switch (choice) {
            case 1: {
                loadGameSpeed();  // 저장된 게임 속도 불러오기
                startGame();
                break;
            }
            case 2: {
                SaveData* saveData = loadGame();
                if (saveData) {
                    currentSpeedIndex = saveData->gameSpeed;
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
    loadGameSpeed();   // 저장된 게임 속도 불러오기
    mainMenu();
    return 0;
} 