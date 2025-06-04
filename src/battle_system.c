#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "battle_system.h"
#include "text.h"
#include "hp_system.h"
#include "battle.h"  // currentEnemy 변수를 위해 추가

// 전투 시스템 초기화 함수
void initBattleSystem() {
    // 향후 전투 시스템 초기화 코드가 들어갈 자리
}

// 상성 시스템 구현 함수
// 공격 기술의 타입과 방어자의 타입에 따른 데미지 배율을 반환
float getTypeEffectiveness(YokaiType moveType, YokaiType defenderType) {
    // 상성 관계 정의
    switch (moveType) {
        case TYPE_EVIL_SPIRIT:
            if (defenderType == TYPE_GHOST) return 2.0f;  // 악령 기술은 유령에게 효과적
            if (defenderType == TYPE_HUMAN) return 0.5f;  // 악령 기술은 인간에게 약함
            break;
        case TYPE_GHOST:
            if (defenderType == TYPE_HUMAN) return 2.0f;  // 유령 기술은 인간에게 효과적
            if (defenderType == TYPE_EVIL_SPIRIT) return 0.5f;  // 유령 기술은 악령에게 약함
            break;
        case TYPE_MONSTER:
            if (defenderType == TYPE_ANIMAL) return 2.0f;  // 괴물 기술은 동물에게 효과적
            if (defenderType == TYPE_HUMAN) return 0.5f;  // 괴물 기술은 인간에게 약함
            break;
        case TYPE_HUMAN:
            if (defenderType == TYPE_MONSTER) return 2.0f;  // 인간 기술은 괴물에게 효과적
            if (defenderType == TYPE_GHOST) return 0.5f;  // 인간 기술은 유령에게 약함
            break;
        case TYPE_ANIMAL:
            if (defenderType == TYPE_EVIL_SPIRIT) return 2.0f;  // 동물 기술은 악령에게 효과적
            if (defenderType == TYPE_MONSTER) return 0.5f;  // 동물 기술은 괴물에게 약함
            break;
    }
    return 1.0f;  // 기본 상성
}

// 데미지 계산 함수
// 공격자, 방어자, 사용 기술에 따른 데미지를 계산
float calculateDamage(const Yokai* attacker, const Yokai* defender, const Move* move) {
    // 기본 데미지 계산: 공격력 * 기술 위력 * 레벨 보정
    float baseDamage = (float)attacker->attack * move->power * (attacker->level / 5.0f + 1);
    // 방어력 보정
    float defenseFactor = defender->defense + 100.0f;
    // 타입 상성 적용
    float typeEffectiveness = getTypeEffectiveness(move->type, defender->type);
    
    // 최종 데미지 계산
    float damage = (baseDamage / defenseFactor) * typeEffectiveness;
    
    // 랜덤 요소 추가 (0.85 ~ 1.0)
    float randomFactor = 0.85f + (float)(rand() % 16) / 100.0f;
    damage *= randomFactor;
    
    // 데미지를 5%로 감소
    damage *= 0.05f;
    
    // 소수점 올림
    return (float)ceil(damage);
}

// 전투 실행 함수
// 공격자와 방어자 간의 전투를 실행하고 결과를 반환
int executeBattle(Yokai* attacker, Yokai* defender, int moveIndex) {
    // 사용할 기술 정보 가져오기
    const Move* move = &attacker->moves[moveIndex].move;
    
    // 기술의 상성에 따른 색상 설정
    const char* colorCode;
    switch (move->type) {
        case TYPE_EVIL_SPIRIT:
            colorCode = "\033[31m";  // 빨간색
            break;
        case TYPE_GHOST:
            colorCode = "\033[35m";  // 보라색
            break;
        case TYPE_MONSTER:
            colorCode = "\033[33m";  // 노란색
            break;
        case TYPE_HUMAN:
            colorCode = "\033[36m";  // 청록색
            break;
        case TYPE_ANIMAL:
            colorCode = "\033[32m";  // 초록색
            break;
        default:
            colorCode = "\033[0m";   // 기본색
    }
    
    // 기술 사용 메시지 출력 (상성 색상 적용)
    char buffer[256];
    sprintf(buffer, "\n%s의 ", attacker->name);
    printText(buffer);
    printText(colorCode);
    printText(move->name);
    printText("\033[0m");
    printText("!");
    
    // 명중률 체크
    if ((rand() % 100) >= move->accuracy) {
        printText("\n하지만 빗나갔다!");
        return 0;
    }
    
    // 데미지 계산 및 적용
    float damage = calculateDamage(attacker, defender, move);
    
    // 데미지 적용 및 상태 업데이트
    if (defender == attacker) {
        attacker->currentHP -= damage;
        if (attacker->currentHP < 0) {
            attacker->currentHP = 0;
            attacker->status = YOKAI_FAINTED;  // 기절 상태로 변경
        }
    } else {
        defender->currentHP -= damage;
        if (defender->currentHP < 0) {
            defender->currentHP = 0;
            defender->status = YOKAI_FAINTED;  // 기절 상태로 변경
        }
        // 상대 요괴의 HP 업데이트
        if (defender == &currentEnemy) {
            currentEnemy.currentHP = defender->currentHP;
            currentEnemy.status = defender->status;
        }
    }
    
    // 데미지 메시지 출력
    sprintf(buffer, "\n%s에게 %.0f의 데미지를 입혔다!", defender->name, damage);
    printText(buffer);
    
    // 상성 메시지 출력
    float typeEffectiveness = getTypeEffectiveness(move->type, defender->type);
    if (typeEffectiveness > 1.0f) {
        printText("\n효과가 굉장했다!");
    } else if (typeEffectiveness < 1.0f) {
        printText("\n효과가 별로인 것 같다...");
    }
    
    // HP 바 업데이트 및 출력
    float maxHP = calculateHP(defender);
    float hpPercentage = (defender->currentHP / maxHP) * 100.0f;
    int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
    
    printText("\nHP[");
    // HP 상태에 따른 색상 설정
    if (hpPercentage <= 20.0f) {
        printText("\033[31m"); // 빨간색
    } else if (hpPercentage <= 50.0f) {
        printText("\033[33m"); // 노란색
    } else {
        printText("\033[1;32m"); // 초록색
    }
    
    // HP 바 시각화
    for (int i = 0; i < HP_BAR_LENGTH; i++) {
        if (i < filledLength) {
            printText("█");
        } else {
            printText("░");
        }
    }
    printText("\033[0m");
    sprintf(buffer, "] %.0f/%.0f\n", defender->currentHP, maxHP);
    printText(buffer);
    
    // 전투 결과 체크
    if (defender->currentHP <= 0) {
        defender->currentHP = 0;
        defender->status = YOKAI_FAINTED;  // 기절 상태로 변경
        if (defender == &currentEnemy) {
            currentEnemy.currentHP = 0;
            currentEnemy.status = YOKAI_FAINTED;
        }
        return 1;  // 승리
    }
    
    return 0;  // 전투 계속
}

// 턴제 전투 실행 함수
// 플레이어와 적 요괴 간의 턴제 전투를 실행
int executeTurnBattle(Yokai* playerYokai, Yokai* enemyYokai, int playerMoveIndex) {
    // 스피드에 따른 선공 결정
    int playerFirst = playerYokai->speed >= enemyYokai->speed;
    
    // 선공 요괴의 공격
    Yokai* firstAttacker = playerFirst ? playerYokai : enemyYokai;
    Yokai* firstDefender = playerFirst ? enemyYokai : playerYokai;
    int firstMoveIndex = playerFirst ? playerMoveIndex : (rand() % enemyYokai->moveCount);
    
    // 첫 번째 공격 실행
    int result = executeBattle(firstAttacker, firstDefender, firstMoveIndex);
    if (result == 1) {
        return playerFirst ? 1 : -1;  // 선공자가 승리
    }
    
    // 후공 요괴의 공격
    Yokai* secondAttacker = playerFirst ? enemyYokai : playerYokai;
    Yokai* secondDefender = playerFirst ? playerYokai : enemyYokai;
    int secondMoveIndex = playerFirst ? (rand() % enemyYokai->moveCount) : playerMoveIndex;
    
    // 두 번째 공격 실행
    result = executeBattle(secondAttacker, secondDefender, secondMoveIndex);
    if (result == 1) {
        return playerFirst ? -1 : 1;  // 후공자가 승리
    }
    
    return 0;  // 전투 계속
}

// 전투 결과 처리 함수
// 전투 결과에 따른 메시지 출력
void handleBattleResult(Yokai* attacker, Yokai* defender, int result) {
    if (result == 1) {
        char buffer[256];
        if (defender->status == YOKAI_FAINTED) {
            sprintf(buffer, "\n%s이(가) 쓰러졌다!", defender->name);
            printTextAndWait(buffer);
        }
    } else if (result == -1) {
        char buffer[256];
        if (attacker->status == YOKAI_FAINTED) {
            sprintf(buffer, "\n%s이(가) 쓰러졌다!", attacker->name);
            printTextAndWait(buffer);
        }
    }
} 