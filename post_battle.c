#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "game_db.h"

// 외부 함수 선언
extern void printCharByChar(const char* str);
extern void clearInputBuffer();  // clearInputBuffer를 외부 함수로 선언

// 전투 후 실행될 코드
void postBattleSystem() {
    system("cls");
    printCharByChar("\n=== 전투 후 시스템 ===");
    printCharByChar("\n임시 시스템입니다. 'a'를 입력하여 다음으로 진행하세요.");
    
    clearInputBuffer();  // 입력 버퍼 비우기
    
    char input;
    while (1) {
        if (_kbhit()) {
            input = _getch();
            if (input == 'a' || input == 'A') {
                break;
            }
        }
    }
    
    printCharByChar("\n다음 단계로 진행합니다...");
    Sleep(1000);
} 