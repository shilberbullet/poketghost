#include <stdio.h>
#include "battle_system.h"
#include "text.h"

void initBattleSystem() {
    // 향후 전투 시스템 초기화 코드가 들어갈 자리
}

int executeBattle(Yokai* attacker, Yokai* defender, int moveIndex) {
    // 기술 사용 메시지 출력
    char buffer[256];
    sprintf(buffer, "\n%s가 %s 기술을 사용했다!\n", 
        attacker->name, 
        attacker->moves[moveIndex].move.name);
    printTextAndWait(buffer);
    
    // 향후 실제 전투 로직이 들어갈 자리
    printTextAndWait("\n전투 시스템은 추후 개발 예정입니다.");
    
    return 1; // 전투 승리로 가정
}

void handleBattleResult(Yokai* attacker, Yokai* defender, int result) {
    // 향후 전투 결과 처리 코드가 들어갈 자리
} 