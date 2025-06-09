// 표준 입출력 함수를 위한 헤더
#include <stdio.h>
// 게임 관련 함수와 구조체 정의
#include "game.h"
// 텍스트 출력 관련 함수
#include "text.h"
// 파티 관리 관련 함수
#include "party.h"
// 아이템 관련 함수
#include "item.h"
// 로그라이트 시스템 관련 함수
#include "roguelite.h"
#include "../core/state.h"

// 전역 게임 상태와 플레이어 정보는 state 모듈에서 관리
 
// 게임 초기화 함수
void initGame() {
    // 게임의 기본 상태를 초기화
    gameState.isRunning = 1;        // 게임 실행 상태를 true로 설정
    gameState.currentStage = 1;     // 현재 스테이지를 1로 설정
    gameState.currentTime = 0;      // 게임 시간을 0으로 초기화
    
    // 이어하기가 아닌 새로운 게임인 경우에만 플레이어 정보 초기화
    if (!gameState.isLoadedGame) {
        gameState.isNewGame = 1;     // 새 게임 플래그를 true로 설정
        player.money = 0;            // 초기 전을 0으로 설정
        inventoryCount = 0;          // 인벤토리 아이템 수를 0으로 초기화
        
        // 아이템 데이터를 파일에서 로드
        loadItemsFromFile("data/items.txt");
        
        // 게임 시작 시 기본 아이템인 '낡은 부적' 5개를 인벤토리에 추가
        Item* oldTalisman = NULL;
        for (int i = 0; i < itemListCount; i++) {
            if (strcmp(itemList[i].name, "낡은부적") == 0) {
                oldTalisman = &itemList[i];
                break;
            }
        }
        if (oldTalisman) {
            for (int i = 0; i < 5; i++) {
                addItemToInventory(oldTalisman);
            }
        }
    }
}

// 전을 추가하는 함수
void addMoney(int amount) {
    // 무당방울 아이템의 효과를 계산 (최대 5개까지 효과 적용)
    int mudangCount = 0;
    for (int i = 0; i < inventoryCount; i++) {
        if (strcmp(inventory[i].item.name, "무당방울") == 0) {
            mudangCount = inventory[i].count;
            if (mudangCount > 5) mudangCount = 5;
            break;
        }
    }
    // 무당방울 효과로 추가 전 계산 (무당방울 1개당 10% 보너스)
    int bonus = (amount * 10 * mudangCount) / 100;
    int total = amount + bonus;
    player.money += total;
    
    // 획득한 전에 대한 메시지 출력
    char buffer[128];
    if (mudangCount > 0) {
        sprintf(buffer, "\n%d전을 획득했습니다! (무당방울 보너스 +%d%%, 총 %d전, 현재 보유: %d전)\n", 
                amount, mudangCount*10, total, player.money);
    } else {
        sprintf(buffer, "\n%d전을 획득했습니다! (현재 보유: %d전)\n", amount, player.money);
    }
    printText(buffer);
}

// 현재 보유한 전을 보여주는 함수
void showMoney() {
    char buffer[128];
    sprintf(buffer, "\n현재 보유 전: %d전\n", player.money);
    printText(buffer);
}

// 게임 상태를 완전히 초기화하는 함수
void resetGameState() {
    // 게임 상태를 기본값으로 초기화
    gameState.isRunning = 1;        // 게임 실행 상태를 true로 설정
    gameState.currentStage = 1;     // 현재 스테이지를 1로 설정
    gameState.currentTime = 0;      // 게임 시간을 0으로 초기화
    gameState.isNewGame = 1;        // 새 게임 플래그를 true로 설정
    gameState.isLoadedGame = 0;     // 이어하기 플래그를 false로 설정
    player.money = 0;               // 전을 0으로 초기화
    inventoryCount = 0;             // 인벤토리 아이템 수를 0으로 초기화
    
    // 아이템 데이터를 다시 로드
    loadItemsFromFile("data/items.txt");
    
    // 낡은 부적 5개를 다시 추가
    Item* oldTalisman = NULL;
    for (int i = 0; i < itemListCount; i++) {
        if (strcmp(itemList[i].name, "낡은부적") == 0) {
            oldTalisman = &itemList[i];
            break;
        }
    }
    if (oldTalisman) {
        for (int i = 0; i < 5; i++) {
            addItemToInventory(oldTalisman);
        }
    }
} 