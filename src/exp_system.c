// 표준 입출력 함수를 위한 헤더
#include <stdio.h>
// 메모리 할당 및 랜덤 함수를 위한 헤더
#include <stdlib.h>
// 수학 함수를 위한 헤더
#include <math.h>
// 경험치 시스템 관련 함수와 구조체 정의
#include "exp_system.h"
// 텍스트 출력 관련 함수
#include "text.h"

// 레벨업에 필요한 경험치 계산 함수
// 레벨에 따라 필요한 경험치를 계산
int calculateRequiredExp(int level) {
    // 기본 공식: 100 * (level^1.5)
    return (int)(100 * pow(level, 1.5));
}

// 전투 승리 시 획득 경험치 계산 함수
// 적 요괴의 레벨과 보스 여부에 따라 경험치를 계산
int calculateBattleExp(const Yokai* enemy) {
    // 기본 경험치: 적 레벨 * 50
    int baseExp = enemy->level * 50;
    
    // 보스 요괴일 경우 2배 경험치
    if (enemy->level >= 10) {
        baseExp *= 2;
    }
    
    // 레벨 차이 보정: 플레이어 요괴와 적 요괴의 레벨 차이에 따라 경험치 증감
    // (플레이어 요괴 포인터를 추가로 받아야 하지만, 기존 인터페이스를 유지하기 위해 전역 party[0] 사용)
    extern Yokai party[];
    extern int partyCount;
    int playerLevel = partyCount > 0 ? party[0].level : 1;
    int levelDiff = enemy->level - playerLevel;
    float ratio = 1.0f + (levelDiff * 0.2f); // 레벨 차이 1당 20% 증감
    if (ratio < 0.1f) ratio = 0.1f; // 최소 10%
    
    // 랜덤 보너스: ±20%
    float randomBonus = 0.8f + (rand() % 41) / 100.0f;
    int finalExp = (int)(baseExp * ratio * randomBonus);
    if (finalExp < 1) finalExp = 1;
    // 경험치 2배로 증가
    return finalExp * 2;
}

// 경험치 획득 및 레벨업 처리 함수
// 요괴가 경험치를 획득하고 레벨업 여부를 체크
void gainExp(Yokai* yokai, int exp) {
    // 경험치 획득 메시지 출력
    char buffer[256];
    sprintf(buffer, "\n%s가 %d의 경험치를 획득했습니다!\n", yokai->name, exp);
    printText(buffer);
    
    // 경험치 추가
    yokai->exp += exp;
    
    // 레벨업 체크 및 처리
    while (yokai->exp >= calculateRequiredExp(yokai->level)) {
        levelUp(yokai);
    }
}

// 레벨업 처리 함수
// 요괴의 레벨을 올리고 능력치를 재계산
void levelUp(Yokai* yokai) {
    char buffer[256];
    yokai->level++;  // 레벨 증가
    yokai->exp = 0;  // 경험치 초기화
    
    // HP 재계산 (10%로 감소하고 소수점 버림)
    float hp = yokai->stamina * (1.0f + (yokai->level * yokai->level) / 100.0f) * 0.1f;
    yokai->currentHP = (float)((int)hp);
    
    // 레벨업 메시지 출력
    sprintf(buffer, "\n%s가 레벨 %d로 상승했습니다!\n", yokai->name, yokai->level);
    printText(buffer);
} 