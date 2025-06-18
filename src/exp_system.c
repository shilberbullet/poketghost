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
#include "../core/state.h"
#include "yokai.h"
#include "move_learning.h"

// 레벨업에 필요한 경험치 계산 함수
// 레벨에 따라 필요한 경험치를 계산
int calculateRequiredExp(int level) {
    // 기본 공식: 20 * (level^1.2)
    return (int)(15 * pow(level, 1.7));
}

// 전투 승리 시 획득 경험치 계산 함수
// 적 요괴의 레벨과 보스 여부에 따라 경험치를 계산
int calculateBattleExp(const Yokai* enemy) {
    // 기본 경험치: 적 레벨 * 80
    int baseExp = enemy->level * 80;
    
    // 보스 요괴일 경우 2배 경험치
    if (enemy->level >= 10) {
        baseExp *= 2;
    }
    
    // 레벨 차이 보정: 플레이어 요괴와 적 요괴의 레벨 차이에 따라 경험치 증감
    // 파티 정보는 state 모듈에서 접근
    int playerLevel = gPartyCount > 0 ? gParty[0].level : 1;
    int levelDiff = enemy->level - playerLevel;
    float ratio;
    
    if (levelDiff > 0) {
        // 플레이어 레벨이 낮을 때: 레벨 차이당 20% 증가 (최대 200%)
        ratio = 1.0f + (levelDiff * 0.2f);
        if (ratio > 3.0f) ratio = 3.0f;
    } else {
        // 플레이어 레벨이 높을 때: 기본 경험치 유지
        ratio = 1.0f;
    }
    
    // 랜덤 보너스: ±20%
    float randomBonus = 0.8f + (rand() % 41) / 100.0f;
    int finalExp = (int)(baseExp * ratio * randomBonus);
    if (finalExp < 1) finalExp = 1;
    // 경험치 2배로 증가
    return finalExp * 2;
}

// 개별 요괴의 경험치 계산 함수 (새로 추가)
// 각 요괴의 레벨에 따라 개별적으로 경험치를 계산
int calculateIndividualExp(const Yokai* enemy, const Yokai* player) {
    // 기본 경험치: 적 레벨 * 80
    int baseExp = enemy->level * 80;
    
    // 보스 요괴일 경우 2배 경험치
    if (enemy->level >= 10) {
        baseExp *= 2;
    }
    
    // 레벨 차이 보정: 개별 요괴와 적 요괴의 레벨 차이에 따라 경험치 증감
    int levelDiff = enemy->level - player->level;
    float ratio;
    
    if (levelDiff > 0) {
        // 요괴 레벨이 낮을 때: 레벨 차이당 20% 증가 (최대 200%)
        ratio = 1.0f + (levelDiff * 0.2f);
        if (ratio > 3.0f) ratio = 3.0f;
    } else {
        // 요괴 레벨이 높을 때: 기본 경험치 유지
        ratio = 1.0f;
    }
    
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
        int requiredExp = calculateRequiredExp(yokai->level);
        yokai->exp -= requiredExp;  // 필요한 경험치만 차감
        levelUp(yokai);
    }
}

// 레벨업 처리 함수
// 요괴의 레벨을 올리고 능력치를 재계산
void levelUp(Yokai* yokai) {
    char buffer[256];
    float oldMaxHP = calculateHP(yokai);  // 이전 최대 HP 저장
    yokai->level++;  // 레벨 증가
    float newMaxHP = calculateHP(yokai);  // 새로운 최대 HP 계산
    float hpIncrease = newMaxHP - oldMaxHP;  // HP 증가량 계산
    
    // 기절 상태가 아닐 때만 현재 HP 증가
    if (yokai->status != YOKAI_FAINTED) {
        yokai->currentHP += hpIncrease;  // 현재 HP에 증가량만큼 더하기
    }
    
    // 레벨업 메시지 출력
    sprintf(buffer, "\n%s가 레벨 %d로 상승했습니다!\n", yokai->name, yokai->level);
    printText(buffer);
    
    // 기술 학습 시도
    if (tryLearnNewMove(yokai)) {
        printText("\n새로운 기술을 배웠습니다!\n");
    }
} 