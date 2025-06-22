// 표준 입출력 함수 사용을 위한 헤더 파일 포함
#include <stdio.h> // printf, scanf 등 입출력 함수 사용
// 동적 메모리 할당 및 기타 유틸리티 함수 사용을 위한 헤더 파일 포함
#include <stdlib.h> // malloc, free, rand 등 사용
// 수학 함수 사용을 위한 헤더 파일 포함
#include <math.h> // pow, ceil 등 수학 함수 사용
// 전투 시스템 관련 함수와 타입 정의 헤더 파일 포함
#include "battle_system.h" // 전투 시스템 함수 및 타입 정의
// 로깅 기능을 위한 헤더 파일 포함
#include "logger.h" // 함수 실행, 메시지 기록 등 로깅 함수 사용
// 텍스트 출력 관련 함수 헤더 파일 포함
#include "text.h" // printText, printTextAndWait 등 텍스트 출력 함수 사용
// HP(체력) 시스템 관련 함수 헤더 파일 포함
#include "hp_system.h" // calculateHP 등 HP 관련 함수 사용
// 전투 관련 전역 변수 및 함수 사용을 위한 헤더 파일 포함
#include "battle.h"  // currentEnemy 등 전투 관련 전역 변수 사용
// 아이템 시스템 관련 함수 및 타입 정의 헤더 파일 포함
#include "item.h" // 작두 개수 확인 등 아이템 관련 함수 사용
// 게임의 전역 상태 관리 헤더 파일 포함
#include "../core/state.h" // gPartyCount, gParty 등 전역 상태 변수 사용
// Windows API 함수 사용을 위한 헤더 파일 포함
#include <windows.h> // Sleep 등 윈도우 전용 함수 사용
// 상대 요괴 이름 색상 반환 함수 외부 선언
extern const char* getEnemyNameColorExport(); // 전투 UI에서 상대 요괴 이름 색상 적용 함수

// 전투 시스템 초기화 함수 정의 시작
void initBattleSystem() { // 전투 시스템을 초기화하는 함수 선언
    LOG_FUNCTION_EXECUTION("initBattleSystem"); // 함수 실행 시 로깅(함수명 기록)
    // 향후 전투 시스템 초기화 코드가 들어갈 자리(현재는 비어 있음)
}

// 상성 시스템 구현 함수 정의 시작
// 공격 기술의 타입과 방어자의 타입에 따른 데미지 배율을 반환하는 함수 선언
float getTypeEffectiveness(YokaiType moveType, YokaiType defenderType) {
    LOG_FUNCTION_EXECUTION("getTypeEffectiveness"); // 함수 실행 시 로깅(함수명 기록)
    // 상성 관계를 switch문으로 분기하여 처리
    switch (moveType) { // 공격 기술의 타입에 따라 분기
        case TYPE_EVIL_SPIRIT: // 공격 기술이 악귀 타입인 경우
            if (defenderType == TYPE_GHOST) return 2.0f;  // 악귀 기술은 원귀에게 효과적(2배)
            if (defenderType == TYPE_HUMAN || defenderType == TYPE_MONSTER) return 0.5f;  // 악귀 기술은 인간형, 괴수형에게 약함(0.5배)
            break; // 분기 종료
        case TYPE_GHOST: // 공격 기술이 원귀 타입인 경우
            if (defenderType == TYPE_HUMAN || defenderType == TYPE_MONSTER) return 2.0f;  // 원귀 기술은 인간형, 괴수형에게 효과적(2배)
            if (defenderType == TYPE_EVIL_SPIRIT) return 0.5f;  // 원귀 기술은 악귀에게 약함(0.5배)
            break; // 분기 종료
        case TYPE_HUMAN: // 공격 기술이 인간형 타입인 경우
            if (defenderType == TYPE_EVIL_SPIRIT) return 2.0f;  // 인간형 기술은 악귀에게 효과적(2배)
            if (defenderType == TYPE_GHOST || defenderType == TYPE_ANIMAL) return 0.5f;  // 인간형 기술은 원귀, 동물형에게 약함(0.5배)
            break; // 분기 종료
        case TYPE_ANIMAL: // 공격 기술이 동물형 타입인 경우
            if (defenderType == TYPE_HUMAN) return 2.0f;  // 동물형 기술은 인간형에게 효과적(2배)
            if (defenderType == TYPE_MONSTER) return 0.5f;  // 동물형 기술은 괴수형에게 약함(0.5배)
            break; // 분기 종료
        case TYPE_MONSTER: // 공격 기술이 괴수형 타입인 경우
            if (defenderType == TYPE_EVIL_SPIRIT || defenderType == TYPE_ANIMAL) return 2.0f;  // 괴수형 기술은 악귀, 동물형에게 효과적(2배)
            if (defenderType == TYPE_GHOST) return 0.5f;  // 괴수형 기술은 원귀에게 약함(0.5배)
            break; // 분기 종료
    }
    return 1.0f;  // 기본 상성(효과 없음, 1배)
}

// 데미지 계산 함수 정의 시작
// 공격자, 방어자, 사용 기술에 따른 데미지를 계산하여 반환하는 함수 선언
float calculateDamage(const Yokai* attacker, const Yokai* defender, const Move* move) {
    LOG_FUNCTION_EXECUTION("calculateDamage"); // 함수 실행 시 로깅(함수명 기록)
    // 기본 데미지 계산: 공격력 * 기술 위력 * 레벨 보정
    float baseDamage = (float)attacker->attack * move->power * (1.0f + (attacker->level * 1.2f)); // 공격력, 기술 위력, 레벨에 따른 기본 데미지 산출
    // 방어력 보정값 계산(방어력 + 100)
    float defenseFactor = defender->defense + 100.0f; // 방어력 보정값 계산
    // 타입 상성 효과 계산
    float typeEffectiveness = getTypeEffectiveness(move->type, defender->type); // 상성 효과 배율 계산
    // 타입 일치(STAB) 보너스 적용: 공격자와 기술 타입이 같으면 1.5배
    if (move->type == attacker->type) {
        typeEffectiveness *= 1.5f; // 타입 일치 보너스 적용
    }
    // 최종 데미지 계산: (기본 데미지 / 방어력 보정) * 상성 * 0.7
    float damage = (baseDamage / defenseFactor) * typeEffectiveness * 0.7f; // 최종 데미지 산출
    // 랜덤 요소 추가(0.85~1.0)
    float randomFactor = 0.85f + (float)(rand() % 16) / 100.0f; // 0.85~1.0 사이의 랜덤값
    damage *= randomFactor; // 랜덤 요소 적용
    // 데미지를 5%로 감소(게임 밸런스 조정)
    damage *= 0.05f; // 데미지 5%로 축소
    // 소수점 올림하여 반환
    return (float)ceil(damage); // 최종 데미지 반환(올림)
}

// 전투 실행 함수 정의 시작
// 공격자와 방어자 간의 전투를 실행하고 결과(1=승리, 0=계속)를 반환하는 함수 선언
int executeBattle(Yokai* attacker, Yokai* defender, int moveIndex) {
    LOG_FUNCTION_EXECUTION("executeBattle"); // 함수 실행 시 로깅(함수명 기록)
    // 사용할 기술 정보 가져오기
    const Move* move = &attacker->moves[moveIndex].move;
    
    // PP 감소
    attacker->moves[moveIndex].currentPP--;
    
    // 기술 타입에 따른 색상 코드 설정
    const char* colorCode; // 기술 타입별 색상 코드를 저장할 포인터 변수
    if (move->type >= 0 && move->type < 5) { // 기술 타입이 유효한 범위인지 확인
        switch (move->type) { // 기술 타입에 따라 색상 분기
        case TYPE_EVIL_SPIRIT: // 악귀 타입인 경우
            colorCode = "\033[31m";  // 빨간색 코드 설정
            break; // 분기 종료
        case TYPE_GHOST: // 원귀 타입인 경우
            colorCode = "\033[35m";  // 보라색 코드 설정
            break; // 분기 종료
        case TYPE_MONSTER: // 괴수 타입인 경우
            colorCode = "\033[33m";  // 노란색 코드 설정
            break; // 분기 종료
        case TYPE_HUMAN: // 인간형 타입인 경우
            colorCode = "\033[36m";  // 청록색 코드 설정
            break; // 분기 종료
        case TYPE_ANIMAL: // 동물형 타입인 경우
            colorCode = "\033[32m";  // 초록색 코드 설정
            break; // 분기 종료
        default: // 기본값 처리
            colorCode = "\033[0m";   // 기본색(흰색) 코드 설정
        }
    } else { // 기술 타입이 유효하지 않은 경우
        colorCode = "\033[0m";   // 잘못된 타입일 경우 기본색으로 설정
    }
    
    // 기술 사용 메시지 출력을 위한 버퍼 선언
    char buffer[512]; // 기술 사용 메시지를 저장할 문자열 버퍼
    if (attacker == &currentEnemy) { // 공격자가 상대 요괴인 경우
        // 상대 요괴의 경우 색상 적용하여 메시지 구성
        sprintf(buffer, "\n%s%s\033[0m의 %s%s\033[0m!", getEnemyNameColorExport(), attacker->name, colorCode, move->name);
    } else { // 공격자가 플레이어 요괴인 경우
        // 동료 요괴의 경우 요괴 타입에 따른 색상 적용
        const char* yokaiColorCode; // 요괴 타입별 색상 코드를 저장할 포인터 변수
        switch (attacker->type) { // 공격자 요괴의 타입에 따라 색상 분기
            case TYPE_EVIL_SPIRIT: // 악귀 타입인 경우
                yokaiColorCode = "\033[31m";  // 빨간색 코드 설정
                break; // 분기 종료
            case TYPE_GHOST: // 원귀 타입인 경우
                yokaiColorCode = "\033[35m";  // 보라색 코드 설정
                break; // 분기 종료
            case TYPE_MONSTER: // 괴수 타입인 경우
                yokaiColorCode = "\033[33m";  // 노란색 코드 설정
                break; // 분기 종료
            case TYPE_HUMAN: // 인간형 타입인 경우
                yokaiColorCode = "\033[36m";  // 청록색 코드 설정
                break; // 분기 종료
            case TYPE_ANIMAL: // 동물형 타입인 경우
                yokaiColorCode = "\033[32m";  // 초록색 코드 설정
                break; // 분기 종료
            default: // 기본값 처리
                yokaiColorCode = "\033[0m";   // 기본색(흰색) 코드 설정
        }
        // 동료 요괴의 기술 사용 메시지 구성
        sprintf(buffer, "\n%s%s\033[0m의 %s%s\033[0m!", yokaiColorCode, attacker->name, colorCode, move->name);
    }
    // 기술 사용 메시지 출력 및 대기
    printTextAndWait(buffer); // 구성된 메시지를 화면에 출력하고 사용자 입력 대기
    fastSleep(500); // 0.5초간 대기

    // 명중률 계산 시작
    int baseAccuracy = move->accuracy; // 기술의 기본 명중률 가져오기
    int magnifierBonus = attacker->magnifierCount * 3; // 돋보기 개수에 따른 명중률 보너스 계산(개당 3% 증가)
    int finalAccuracy = baseAccuracy + magnifierBonus; // 최종 명중률 계산(기본 + 보너스)
    if (finalAccuracy > 100) finalAccuracy = 100; // 최종 명중률이 100%를 초과하면 100%로 제한

    // 돋보기 효과 메시지 출력 (돋보기가 있을 때만)
    if (magnifierBonus > 0) { // 돋보기 보너스가 있는 경우
        char magnifierBuffer[256]; // 돋보기 효과 메시지를 저장할 버퍼
    
        // 돋보기 효과 메시지 구성
        sprintf(magnifierBuffer, "\n돋보기 %d개의 효과로 명중률이 %d%% 증가! (%d%% → %d%%)", 
            attacker->magnifierCount, magnifierBonus, baseAccuracy, finalAccuracy);
        printTextAndWait(magnifierBuffer); // 돋보기 효과 메시지 출력 및 대기
        fastSleep(500); // 0.5초간 대기
    }

    // 명중 판정 (랜덤 값이 명중률보다 크면 빗나감)
    if ((rand() % 100) > finalAccuracy) { // 랜덤 값(0-99)이 명중률보다 큰 경우
        printTextAndWait("\n하지만 빗나갔다!"); // 빗나감 메시지 출력 및 대기
        fastSleep(500); // 0.5초간 대기
        return 0; // 빗나감을 나타내는 0 반환
    }
    
    // 데미지 계산 및 적용 시작
    float damage = calculateDamage(attacker, defender, move); // 공격자, 방어자, 기술에 따른 기본 데미지 계산
    float actualDamage = damage; // 실제 적용될 데미지를 저장할 변수(초기값은 계산된 데미지)
    
    // 작두 효과 적용 (플레이어 동료 요괴가 공격할 때만)
    bool isPlayerYokai = false; // 공격자가 플레이어 요괴인지 확인하는 플래그 변수
    for (int i = 0; i < gPartyCount; i++) { // 파티의 모든 요괴를 순회
        if (attacker == &gParty[i]) { // 공격자가 파티의 i번째 요괴와 같은 경우
            isPlayerYokai = true; // 플레이어 요괴임을 표시
            break; // 찾았으므로 반복문 종료
        }
    }
    
    if (isPlayerYokai) { // 공격자가 플레이어 요괴인 경우
        int jakduCount = getJakduCount(); // 현재 보유한 작두 개수 가져오기
        if (jakduCount > 0) { // 작두가 1개 이상 있는 경우
            if (jakduCount > 5) jakduCount = 5; // 작두 개수가 5개를 초과하면 5개로 제한
            float damageMultiplier = 1.0f + 0.1f * jakduCount; // 작두 개수에 따른 데미지 배율 계산(개당 10% 증가)
            actualDamage *= damageMultiplier; // 실제 데미지에 배율 적용
            char buffer[256]; // 작두 효과 메시지를 저장할 버퍼
            sprintf(buffer, "\n작두의 힘으로 데미지가 %.0f%% 증가했습니다!", (damageMultiplier - 1.0f) * 100); // 작두 효과 메시지 구성
            printTextAndWait(buffer); // 작두 효과 메시지 출력 및 대기
            fastSleep(500); // 0.5초간 대기
        }
    }
    
    // 실제 데미지가 현재 HP보다 크면 현재 HP만큼만 데미지를 입힘
    if (actualDamage > defender->currentHP) { // 계산된 데미지가 방어자의 현재 HP보다 큰 경우
        actualDamage = defender->currentHP; // 데미지를 현재 HP로 제한(과다 처치 방지)
    }
    
    // 데미지 적용 및 상태 업데이트 시작
    if (defender == attacker) { // 자기 자신을 공격한 경우(자해)
        attacker->currentHP -= actualDamage; // 공격자의 현재 HP에서 데미지 차감
        if (attacker->currentHP < 0) { // HP가 0보다 작아진 경우
            attacker->currentHP = 0; // HP를 0으로 설정
            attacker->status = YOKAI_FAINTED;  // 기절 상태로 변경
        }
    } else { // 다른 요괴를 공격한 경우
        defender->currentHP -= actualDamage; // 방어자의 현재 HP에서 데미지 차감
        if (defender->currentHP <= 0) { // HP가 0 이하가 된 경우
            defender->currentHP = 0; // HP를 0으로 설정
            defender->status = YOKAI_FAINTED;  // 기절 상태로 변경
        }
        // 상대 요괴의 HP 업데이트 (전역 변수와 동기화)
        if (defender == &currentEnemy) { // 방어자가 현재 상대 요괴인 경우
            currentEnemy.currentHP = defender->currentHP; // 전역 변수의 HP 업데이트
            currentEnemy.status = defender->status; // 전역 변수의 상태 업데이트
        }
    }
    
    // 찹살경단 자동 발동 (기절하지 않았고 HP 20% 이하)
    extern bool tryActivateChapsalgyungdan(Yokai* yokai); // 찹살경단 발동 함수 외부 선언
    if (defender->status != YOKAI_FAINTED) { // 방어자가 기절 상태가 아닌 경우
        tryActivateChapsalgyungdan(defender); // 찹살경단 자동 발동 시도
    }
    
    // 데미지 메시지 출력 시작
    if (defender == &currentEnemy) { // 방어자가 상대 요괴인 경우
        // 상대 요괴의 경우 색상 적용하여 데미지 메시지 구성
        sprintf(buffer, "\n%s%s\033[0m에게 %.0f의 데미지를 입혔다!", getEnemyNameColorExport(), defender->name, actualDamage);
    } else { // 방어자가 플레이어 요괴인 경우
        // 동료 요괴의 경우 기본 색상으로 데미지 메시지 구성
        sprintf(buffer, "\n%s에게 %.0f의 데미지를 입혔다!", defender->name, actualDamage);
    }
    printTextAndWait(buffer); // 데미지 메시지 출력 및 대기
    
    
    // 상성 메시지 출력 시작
    float typeEffectiveness = getTypeEffectiveness(move->type, defender->type); // 기술과 방어자 타입 간의 상성 효과 계산
    if (typeEffectiveness > 1.0f) { // 상성 효과가 1배보다 큰 경우(효과적)
        printTextAndWait("\n효과가 굉장했다!"); // 효과적임을 나타내는 메시지 출력 및 대기
    } else if (typeEffectiveness < 1.0f) { // 상성 효과가 1배보다 작은 경우(비효과적)
        printTextAndWait("\n효과가 별로인 것 같다..."); // 비효과적임을 나타내는 메시지 출력 및 대기
    }
    
    // HP 바 업데이트 및 출력 시작
    float maxHP = calculateHP(defender); // 방어자의 최대 HP 계산
    float hpPercentage = (defender->currentHP / maxHP) * 100.0f; // 현재 HP 비율 계산(백분율)
    int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH); // HP 바에서 채워질 길이 계산
    
    // HP 바 전체를 하나의 문자열로 구성 시작
    char hpBuffer[512]; // HP 바 메시지를 저장할 버퍼
    if (defender == &currentEnemy) { // 방어자가 상대 요괴인 경우
        // 상대 요괴의 경우 색상 적용하여 HP 바 시작 부분 구성
        sprintf(hpBuffer, "\n%s%s\033[0m HP[", getEnemyNameColorExport(), defender->name);
    } else { // 방어자가 플레이어 요괴인 경우
        // 동료 요괴의 경우 기본 색상으로 HP 바 시작 부분 구성
        sprintf(hpBuffer, "\n%s HP[", defender->name);
    }
    
    // HP 비율에 따른 색상 설정 시작
    if (hpPercentage <= 20.0f) { // HP가 20% 이하인 경우
        strcat(hpBuffer, "\033[31m"); // 빨간색 코드 추가(위험 상태)
    } else if (hpPercentage <= 50.0f) { // HP가 50% 이하인 경우
        strcat(hpBuffer, "\033[33m"); // 노란색 코드 추가(주의 상태)
    } else { // HP가 50% 초과인 경우
        strcat(hpBuffer, "\033[1;32m"); // 밝은 초록색 코드 추가(안전 상태)
    }
    
    // HP 바 시각화 시작
    for (int i = 0; i < HP_BAR_LENGTH; i++) { // HP 바의 각 위치를 순회
        if (i < filledLength) { // 현재 위치가 채워진 길이보다 작은 경우
            strcat(hpBuffer, "█"); // 채워진 블록 문자 추가
        } else { // 현재 위치가 채워진 길이보다 크거나 같은 경우
            strcat(hpBuffer, "░"); // 빈 블록 문자 추가
        }
    }
    
    // 색상 초기화 및 HP 바 종료
    strcat(hpBuffer, "\033[0m"); // 색상 코드 초기화
    strcat(hpBuffer, "]"); // HP 바 종료 괄호 추가
    sprintf(hpBuffer + strlen(hpBuffer), " %.0f/%.0f\n", defender->currentHP, maxHP); // 현재 HP와 최대 HP 정보 추가
    
    // HP 바 전체를 한 번에 출력
    printTextAndWait(hpBuffer); // 완성된 HP 바 메시지 출력 및 대기
    fastSleep(500); // 0.5초간 대기
    
    // 전투 결과 체크 시작
    if (defender->currentHP <= 0) { // 방어자의 HP가 0 이하인 경우
        defender->currentHP = 0; // HP를 0으로 설정
        defender->status = YOKAI_FAINTED;  // 기절 상태로 변경
        if (defender == &currentEnemy) { // 방어자가 현재 상대 요괴인 경우
            currentEnemy.currentHP = 0; // 전역 변수의 HP를 0으로 설정
            currentEnemy.status = YOKAI_FAINTED; // 전역 변수의 상태를 기절로 설정
            return 1; // 전투 승리를 나타내는 1 반환
        } else { // 방어자가 플레이어 요괴인 경우
            printTextAndWait("\n동료 요괴가 쓰러졌다!"); // 동료 요괴 기절 메시지 출력 및 대기
            fastSleep(500); // 0.5초간 대기
            return 0; // 전투 계속을 나타내는 0 반환
        }
    }
    
    return 0; // 전투가 계속됨을 나타내는 0 반환
}

// 턴제 전투 실행 함수 정의 시작
// 플레이어와 적 요괴 간의 턴제 전투를 실행하는 함수 선언
int executeTurnBattle(Yokai* playerYokai, Yokai* enemyYokai, int playerMoveIndex) {
    LOG_FUNCTION_EXECUTION("executeTurnBattle"); // 함수 실행 시 로깅(함수명 기록)
    // 레벨 차이에 따른 스피드 보정 계산 시작
    int levelDiff = playerYokai->level - enemyYokai->level; // 플레이어와 적 요괴의 레벨 차이 계산
    float speedMultiplier = 1.0f + (levelDiff * 0.05f);  // 레벨 차이당 5% 보정 배율 계산
    
    // 보정된 스피드 계산 시작
    float playerAdjustedSpeed = playerYokai->speed * speedMultiplier; // 플레이어 요괴의 보정된 스피드 계산
    float enemyAdjustedSpeed = enemyYokai->speed * (1.0f / speedMultiplier); // 적 요괴의 보정된 스피드 계산
    
    // 스피드에 따른 선공 결정 (보정된 스피드 사용)
    int playerFirst = playerAdjustedSpeed >= enemyAdjustedSpeed; // 플레이어가 먼저 공격할지 결정하는 플래그
    
    // 선공 요괴의 공격 준비 시작
    Yokai* firstAttacker = playerFirst ? playerYokai : enemyYokai; // 먼저 공격할 요괴 포인터 설정
    Yokai* firstDefender = playerFirst ? enemyYokai : playerYokai; // 먼저 공격받을 요괴 포인터 설정
    int firstMoveIndex = playerFirst ? playerMoveIndex : (rand() % enemyYokai->moveCount); // 첫 번째 공격에 사용할 기술 인덱스 결정
    
    // 첫 번째 공격 실행
    int result = executeBattle(firstAttacker, firstDefender, firstMoveIndex); // 첫 번째 공격 실행 및 결과 저장
    if (result == 1) { // 첫 번째 공격으로 승리가 결정된 경우
        return playerFirst ? 1 : -1;  // 선공자가 승리한 경우 플레이어 승리(1) 또는 적 승리(-1) 반환
    }
    
    // 후공 요괴의 공격 준비 시작
    Yokai* secondAttacker = playerFirst ? enemyYokai : playerYokai; // 나중에 공격할 요괴 포인터 설정
    Yokai* secondDefender = playerFirst ? playerYokai : enemyYokai; // 나중에 공격받을 요괴 포인터 설정
    int secondMoveIndex = playerFirst ? (rand() % enemyYokai->moveCount) : playerMoveIndex; // 두 번째 공격에 사용할 기술 인덱스 결정
    
    // 두 번째 공격 실행
    result = executeBattle(secondAttacker, secondDefender, secondMoveIndex); // 두 번째 공격 실행 및 결과 저장
    if (result == 1) { // 두 번째 공격으로 승리가 결정된 경우
        return playerFirst ? -1 : 1;  // 후공자가 승리한 경우 적 승리(-1) 또는 플레이어 승리(1) 반환
    }
    
    return 0;  // 전투가 계속됨을 나타내는 0 반환
}

// 전투 결과 처리 함수 정의 시작
// 전투 결과에 따른 메시지를 출력하는 함수 선언
void handleBattleResult(Yokai* attacker, Yokai* defender, int result) {
    LOG_FUNCTION_EXECUTION("handleBattleResult"); // 함수 실행 시 로깅(함수명 기록)
    if (result == 1) { // 공격자가 승리한 경우
        char buffer[256]; // 결과 메시지를 저장할 버퍼
        if (defender->status == YOKAI_FAINTED) { // 방어자가 기절 상태인 경우
            sprintf(buffer, "\n%s(이)가 쓰러졌다!", defender->name); // 방어자 기절 메시지 구성
            printTextAndWait(buffer); // 기절 메시지 출력 및 대기
            fastSleep(500); // 0.5초간 대기
        }
    } else if (result == -1) { // 방어자가 승리한 경우
        char buffer[256]; // 결과 메시지를 저장할 버퍼
        if (attacker->status == YOKAI_FAINTED) { // 공격자가 기절 상태인 경우
            sprintf(buffer, "\n%s(이)가 쓰러졌다!", attacker->name); // 공격자 기절 메시지 구성
            printTextAndWait(buffer); // 기절 메시지 출력 및 대기
            fastSleep(500); // 0.5초간 대기
        }
    }
}

// 발버둥(스트러글) 전투 실행 함수 정의 시작
// PP가 부족한 경우 사용하는 기본 공격을 실행하는 함수 선언
int struggleBattle(Yokai* attacker, Yokai* defender) {
    LOG_FUNCTION_EXECUTION("struggleBattle"); // 함수 실행 시 로깅(함수명 기록)
    // 발버둥 공격 메시지 출력
    printTextAndWait("\n발버둥을 시도했다!"); // 발버둥 시도 메시지 출력 및 대기
    fastSleep(500); // 0.5초간 대기
    
    // 발버둥 데미지 계산 (기본 공격력의 50%)
    float damage = attacker->attack * 0.5f; // 기본 공격력의 50%로 데미지 계산
    if (damage > defender->currentHP) { // 계산된 데미지가 방어자의 현재 HP보다 큰 경우
        damage = defender->currentHP; // 데미지를 현재 HP로 제한(과다 처치 방지)
    }
    
    // 데미지 적용
    defender->currentHP -= damage; // 방어자의 현재 HP에서 데미지 차감
    if (defender->currentHP <= 0) { // HP가 0 이하가 된 경우
        defender->currentHP = 0; // HP를 0으로 설정
        defender->status = YOKAI_FAINTED; // 기절 상태로 변경
    }
    
    // 발버둥 결과 메시지 출력
    char buffer[256]; // 결과 메시지를 저장할 버퍼
    sprintf(buffer, "\n%.0f의 데미지를 입혔다!", damage); // 데미지 결과 메시지 구성
    printTextAndWait(buffer); // 데미지 결과 메시지 출력 및 대기
    fastSleep(500); // 0.5초간 대기
    
    // 전투 결과 반환
    if (defender->currentHP <= 0) { // 방어자가 기절한 경우
        return 1; // 승리를 나타내는 1 반환
    }
    return 0; // 전투가 계속됨을 나타내는 0 반환
} 