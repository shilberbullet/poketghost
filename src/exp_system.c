// 표준 입출력 함수를 위한 헤더
#include <stdio.h> // 표준 입출력 함수
// 메모리 할당 및 랜덤 함수를 위한 헤더
#include <stdlib.h> // 표준 라이브러리 함수
// 수학 함수를 위한 헤더
#include <math.h> // 수학 함수
// 경험치 시스템 관련 함수와 구조체 정의
#include "exp_system.h" // 경험치 시스템 헤더
// 텍스트 출력 관련 함수
#include "text.h" // 텍스트 출력 헤더
#include "../core/state.h" // 게임 상태 헤더
#include "yokai.h" // 요괴 관련 헤더
#include "move_learning.h" // 기술 학습 헤더
#include "dialogue.h" // 대화 시스템 헤더
#include "logger.h" // 로거 헤더

// 레벨업에 필요한 경험치 계산 함수
// 레벨에 따라 필요한 경험치를 계산
int calculateRequiredExp(int level) { // 레벨업 필요 경험치 계산 함수 시작
    LOG_FUNCTION_EXECUTION("calculateRequiredExp"); // 함수 실행 로그 기록
    // 기본 공식: 20 * (level^1.2)
    return (int)(15 * pow(level, 1.7)); // 레벨의 1.7승에 15를 곱한 값 반환
}

// 전투 승리 시 획득 경험치 계산 함수
// 적 요괴의 레벨과 보스 여부에 따라 경험치를 계산
int calculateBattleExp(const Yokai* enemy) { // 전투 경험치 계산 함수 시작
    LOG_FUNCTION_EXECUTION("calculateBattleExp"); // 함수 실행 로그 기록
    // 기본 경험치: 적 레벨 * 80
    int baseExp = enemy->level * 80; // 기본 경험치 계산
    
    // 보스 요괴일 경우 2배 경험치
    if (enemy->level >= 10) { // 적 레벨이 10 이상인 경우
        baseExp *= 2; // 경험치 2배 증가
    }
    
    // 레벨 차이 보정: 플레이어 요괴와 적 요괴의 레벨 차이에 따라 경험치 증감
    // 파티 정보는 state 모듈에서 접근
    int playerLevel = gPartyCount > 0 ? gParty[0].level : 1; // 플레이어 레벨 가져오기
    int levelDiff = enemy->level - playerLevel; // 레벨 차이 계산
    float ratio; // 경험치 비율 변수
    
    if (levelDiff > 0) { // 적 레벨이 높은 경우
        // 플레이어 레벨이 낮을 때: 레벨 차이당 20% 증가 (최대 200%)
        ratio = 1.0f + (levelDiff * 0.2f); // 비율 계산
        if (ratio > 3.0f) ratio = 3.0f; // 최대 300%로 제한
    } else { // 적 레벨이 낮거나 같은 경우
        // 플레이어 레벨이 높을 때: 기본 경험치 유지
        ratio = 1.0f; // 기본 비율 유지
    }
    
    // 랜덤 보너스: ±20%
    float randomBonus = 0.8f + (rand() % 41) / 100.0f; // 0.8~1.2 범위의 랜덤 보너스
    int finalExp = (int)(baseExp * ratio * randomBonus); // 최종 경험치 계산
    if (finalExp < 1) finalExp = 1; // 최소 1 경험치 보장
    // 경험치 2배로 증가
    return finalExp * 2; // 최종 경험치 2배로 반환
}

// 개별 요괴의 경험치 계산 함수 (새로 추가)
// 각 요괴의 레벨에 따라 개별적으로 경험치를 계산
int calculateIndividualExp(const Yokai* enemy, const Yokai* player) { // 개별 경험치 계산 함수 시작
    LOG_FUNCTION_EXECUTION("calculateIndividualExp"); // 함수 실행 로그 기록
    // 기본 경험치: 적 레벨 * 80
    int baseExp = enemy->level * 80; // 기본 경험치 계산
    
    // 보스 요괴일 경우 2배 경험치
    if (enemy->level >= 10) { // 적 레벨이 10 이상인 경우
        baseExp *= 2; // 경험치 2배 증가
    }
    
    // 레벨 차이 보정: 개별 요괴와 적 요괴의 레벨 차이에 따라 경험치 증감
    int levelDiff = enemy->level - player->level; // 레벨 차이 계산
    float ratio; // 경험치 비율 변수
    
    if (levelDiff > 0) { // 적 레벨이 높은 경우
        // 요괴 레벨이 낮을 때: 레벨 차이당 20% 증가 (최대 200%)
        ratio = 1.0f + (levelDiff * 0.2f); // 비율 계산
        if (ratio > 3.0f) ratio = 3.0f; // 최대 300%로 제한
    } else { // 적 레벨이 낮거나 같은 경우
        // 요괴 레벨이 높을 때: 기본 경험치 유지
        ratio = 1.0f; // 기본 비율 유지
    }
    
    // 랜덤 보너스: ±20%
    float randomBonus = 0.8f + (rand() % 41) / 100.0f; // 0.8~1.2 범위의 랜덤 보너스
    int finalExp = (int)(baseExp * ratio * randomBonus); // 최종 경험치 계산
    if (finalExp < 1) finalExp = 1; // 최소 1 경험치 보장
    // 경험치 2배로 증가
    return finalExp * 2; // 최종 경험치 2배로 반환
}

// 경험치 획득 및 레벨업 처리 함수
// 요괴가 경험치를 획득하고 레벨업 여부를 체크
void gainExp(Yokai* yokai, int exp) { // 경험치 획득 함수 시작
    LOG_FUNCTION_EXECUTION("gainExp"); // 함수 실행 로그 기록
    // 경험치 획득 메시지 출력
    char buffer[256]; // 메시지 버퍼
    sprintf(buffer, "\n%s가 %d의 경험치를 획득했습니다!\n", yokai->name, exp); // 경험치 획득 메시지 생성
    printText(buffer); // 메시지 출력
    
    // 경험치 추가
    yokai->exp += exp; // 요괴의 경험치에 획득한 경험치 추가
    
    // 레벨업 체크 및 처리
    while (yokai->exp >= calculateRequiredExp(yokai->level)) { // 레벨업 조건을 만족하는 동안 반복
        int requiredExp = calculateRequiredExp(yokai->level); // 필요 경험치 계산
        yokai->exp -= requiredExp;  // 필요한 경험치만 차감
        levelUp(yokai); // 레벨업 함수 호출
    }
}

// 레벨업 처리 함수
// 요괴의 레벨을 올리고 능력치를 재계산
void levelUp(Yokai* yokai) { // 레벨업 함수 시작
    LOG_FUNCTION_EXECUTION("levelUp"); // 함수 실행 로그 기록
    char buffer[256]; // 메시지 버퍼
    float oldMaxHP = calculateHP(yokai);  // 이전 최대 HP 저장
    yokai->level++;  // 레벨 증가
    float newMaxHP = calculateHP(yokai);  // 새로운 최대 HP 계산
    float hpIncrease = newMaxHP - oldMaxHP;  // HP 증가량 계산
    
    // 기절 상태가 아닐 때만 현재 HP 증가
    if (yokai->status != YOKAI_FAINTED) { // 기절 상태가 아닌 경우
        yokai->currentHP += hpIncrease;  // 현재 HP에 증가량만큼 더하기
    }
    
    // 레벨업 메시지 출력
    sprintf(buffer, "\n%s가 레벨 %d로 상승했습니다!\n", yokai->name, yokai->level); // 레벨업 메시지 생성
    printText(buffer); // 메시지 출력
    
    // 기술 학습 시도
    bool learnedNewMove = tryLearnNewMove(yokai); // 새로운 기술 학습 시도
    if (learnedNewMove) { // 새로운 기술을 배운 경우
        // 고급기술 배움 여부 확인
        bool learnedAdvancedMove = false; // 고급기술 배움 여부 플래그
        for (int i = 0; i < yokai->moveCount; i++) { // 모든 기술 검사
            if (yokai->moves[i].move.grade == MOVE_ADVANCED) { // 고급기술인 경우
                learnedAdvancedMove = true; // 고급기술 배움 플래그 설정
                break; // 루프 종료
            }
        }
        
        if (learnedAdvancedMove) { // 고급기술을 배운 경우
            // 고급기술 배움 대화 시작
            startDialogue(55); // 고급기술 대화 시작
        } else { // 일반 기술을 배운 경우
            // 일반 기술 배움 대화 시작
            startDialogue(50); // 일반 기술 대화 시작
        }
    }
} 