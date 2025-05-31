#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "main_menu.h"
#include "input.h"
#include "settings.h"
#include "move.h"
#include "yokai.h"
#include "item.h"
#include "heal_system.h"
#include "sikhye_system.h"

int main(void) {
    // 콘솔 출력 인코딩 설정
    system("chcp 65001");
    
    // 랜덤 시드 초기화
    srand((unsigned int)time(NULL));
    
    // 아이템 시스템 초기화
    initItemSystem();
    
    // 회복 시스템 초기화
    initHealSystem();
    
    // 데이터 파일 로드
    loadMovesFromFile("data/move.txt");
    loadYokaiFromFile("data/yokai.txt");
    loadItemsFromFile("data/items.txt");
    
    // 게임 설정 초기화
    initSettings();
    
    while (1) {
        initGame(); // 게임 루프 상태 초기화
        showMainMenu();
    }
    
    // 회복 시스템 정리
    cleanupHealSystem();
    
    // 아이템 시스템 정리
    cleanupItemSystem();
    
    return 0;
} 