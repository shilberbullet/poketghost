#include <stdio.h>
#include "game.h"
#include "text.h"
#include "party.h"
#include "item.h"

// 전역 게임 상태 초기화
GameState gameState = {0};
Player player = {0};
 
void initGame() {
    // 게임 상태 초기화
    gameState.isRunning = 1;
    gameState.currentStage = 1;
    
    // 이어하기가 아닌 경우에만 플레이어 초기화
    if (!gameState.isLoadedGame) {
        player.money = 0;  // 초기 전을 0으로 설정
        inventoryCount = 0;  // 인벤토리 초기화
    }
    // 기본 요괴 추가 코드 삭제 (initParty에서 처리)
}

void addMoney(int amount) {
    player.money += amount;
    char buffer[128];
    sprintf(buffer, "\n%d전을 획득했습니다! (현재 보유: %d전)\n", amount, player.money);
    printText(buffer);
}

void showMoney() {
    char buffer[128];
    sprintf(buffer, "\n현재 보유 전: %d전\n", player.money);
    printText(buffer);
} 