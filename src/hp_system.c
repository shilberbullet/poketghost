// 표준 입출력 함수를 위한 헤더
#include <stdio.h>
// 수학 함수를 위한 헤더
#include <math.h>
// HP 시스템 관련 함수와 구조체 정의
#include "hp_system.h"
// 텍스트 출력 관련 함수
#include "text.h"

// HP 계산 함수
// 요괴의 체력 종족값과 레벨을 기반으로 최대 HP를 계산
float calculateHP(const Yokai* yokai) {
    if (yokai == NULL) return 0.0f;
    // HP = 체력 종족값 * (1 + 레벨^2/(100 - log10(레벨+1)*10)) * 0.1
    float hp = yokai->stamina * (1.0f + (yokai->level * yokai->level) / (100.0f - log10(yokai->level + 1) * 10)) * 0.15f;
    return (float)((int)hp); // 소수점 버림
}

// HP 상태 확인 함수
// 현재 HP 비율에 따라 상태 메시지를 반환
const char* getHPStatus(const Yokai* yokai) {
    if (yokai == NULL) return "알 수 없음";
    
    // 최대 HP와 현재 HP 비율 계산
    float maxHP = calculateHP(yokai);
    float currentHP = yokai->currentHP;
    float hpPercentage = (currentHP / maxHP) * 100.0f;
    
    // HP 비율에 따른 상태 메시지 반환
    if (hpPercentage >= 80.0f) return "상태 양호";
    if (hpPercentage >= 50.0f) return "주의 필요";
    if (hpPercentage >= 20.0f) return "위험";
    return "매우 위험";
}

// HP 바 출력 함수
// 요괴의 현재 HP 상태를 시각적으로 표시
void printHPBar(const Yokai* yokai) {
    if (yokai == NULL) return;
    
    // 최대 HP와 현재 HP 비율 계산
    float maxHP = calculateHP(yokai);
    float currentHP = yokai->currentHP;
    float hpPercentage = (currentHP / maxHP) * 100.0f;
    
    // HP 바 길이 계산
    int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
    
    // HP 수치 출력
    char buffer[256];
    sprintf(buffer, "\n%s의 HP: %.0f/%.0f\n", yokai->name, currentHP, maxHP);
    printText(buffer);
    
    // HP 바 시작
    printText("[");
    
    // HP 비율에 따른 색상 설정
    if (hpPercentage <= 20.0f) {
        printText("\033[31m"); // 빨간색 (위험)
    } else if (hpPercentage <= 50.0f) {
        printText("\033[33m"); // 노란색 (주의)
    } else {
        printText("\033[1;32m"); // 초록색 (양호)
    }
    
    // HP 바 시각화
    for (int i = 0; i < HP_BAR_LENGTH; i++) {
        if (i < filledLength) {
            printText("█"); // 채워진 부분
        } else {
            printText("░"); // 빈 부분
        }
    }
    
    // 색상 초기화 및 HP 바 종료
    printText("\033[0m");
    printText("]");
    
    // HP 상태 메시지 출력
    sprintf(buffer, " (%s)\n", getHPStatus(yokai));
    printText(buffer);
} 