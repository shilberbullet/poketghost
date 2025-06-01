#include <stdio.h>
#include <math.h>
#include "hp_system.h"
#include "text.h"

// HP 계산 함수
float calculateHP(const Yokai* yokai) {
    if (yokai == NULL) return 0.0f;
    return yokai->stamina * (1.0f + (yokai->level * yokai->level) / 100.0f);
}

// HP 상태 확인 함수
const char* getHPStatus(const Yokai* yokai) {
    if (yokai == NULL) return "알 수 없음";
    
    float maxHP = calculateHP(yokai);
    float currentHP = yokai->currentHP;
    float hpPercentage = (currentHP / maxHP) * 100.0f;
    
    if (hpPercentage >= 80.0f) return "상태 양호";
    if (hpPercentage >= 50.0f) return "주의 필요";
    if (hpPercentage >= 20.0f) return "위험";
    return "매우 위험";
}

// HP 바 출력 함수
void printHPBar(const Yokai* yokai) {
    if (yokai == NULL) return;
    
    float maxHP = calculateHP(yokai);
    float currentHP = yokai->currentHP;
    float hpPercentage = (currentHP / maxHP) * 100.0f;
    
    // HP 바 계산
    int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
    
    // HP 바 출력
    char buffer[256];
    sprintf(buffer, "\n%s의 HP: %.1f/%.1f\n", yokai->name, currentHP, maxHP);
    printText(buffer);
    
    printText("[");
    if (hpPercentage <= 20.0f) {
        printText("\033[31m"); // 빨간색
    } else if (hpPercentage <= 50.0f) {
        printText("\033[33m"); // 노란색
    } else {
        printText("\033[1;32m"); // 초록색
    }
    for (int i = 0; i < HP_BAR_LENGTH; i++) {
        if (i < filledLength) {
            printText("█");
        } else {
            printText("░");
        }
    }
    printText("\033[0m");
    
    // HP 상태 출력
    sprintf(buffer, "(%s)\n", getHPStatus(yokai));
    printText(buffer);
} 