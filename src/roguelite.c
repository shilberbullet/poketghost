// 표준 입출력 함수를 위한 헤더
#include <stdio.h>
// 메모리 할당, 문자열 처리, Sleep 함수 사용을 위한 헤더
#include <stdlib.h>
#include <string.h>
#include <windows.h>  // Sleep 함수를 위해 추가
#include "roguelite.h"
#include "party.h"
#include "game.h"
#include "text.h"
#include "input.h"
#include "yokai.h"
#include "menu.h"  // 메인 메뉴를 위해 추가
#include "main.h"  // main 함수를 위해 추가
#include "savefile.h" // 세이브 파일 삭제를 위해 추가

// 모든 요괴가 기절했는지 확인하는 함수
bool areAllYokaiFainted() {
    // HP와 상태 동기화
    for (int i = 0; i < partyCount; i++) {
        if (party[i].currentHP > 0) {
            party[i].status = YOKAI_NORMAL;
        } else {
            party[i].status = YOKAI_FAINTED;
        }
    }
    for (int i = 0; i < partyCount; i++) {
        if (party[i].status != YOKAI_FAINTED) {
            return false;
        }
    }
    return true;
}

// 게임 초기화 함수
void resetGame() {
    // 파티 초기화
    initParty();
    
    // 게임 상태 초기화
    resetGameState();
}

// 로그라이트 시스템 메인 함수
void handleRogueliteSystem() {
    printText("\n모든 요괴가 기절했습니다!\n");
    printText("더 이상 싸울 수 있는 요괴가 없습니다.\n");
    printText("잠시 후 게임이 초기화됩니다...\n");
    
    // 1초 대기
    Sleep(1000);
    
    // 세이브 파일 삭제
    removeSaveFile();
    // 게임 상태 초기화
    resetGame();
    
    printText("\n게임이 초기화되었습니다.\n");
    printText("새로운 모험을 시작하세요!\n");
    
    // 초기화면으로 이동
    gameState.isRunning = 0;  // 게임 루프 종료
    main();  // 초기화면으로 이동
} 