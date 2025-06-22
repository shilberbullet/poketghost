// 표준 입출력 함수를 위한 헤더
#include <stdio.h> // 표준 입출력 함수
// 수학 함수를 위한 헤더
#include <math.h> // 수학 함수
// 문자열 처리 함수를 위한 헤더
#include <string.h> // 문자열 처리 함수
// HP 시스템 관련 함수와 구조체 정의
#include "hp_system.h" // HP 시스템 헤더
// 텍스트 출력 관련 함수
#include "text.h" // 텍스트 출력 헤더
// 로깅 함수를 위한 헤더
#include "logger.h" // 로거 헤더

// HP 계산 함수
// 요괴의 체력 종족값과 레벨을 기반으로 최대 HP를 계산
float calculateHP(const Yokai* yokai) { // HP 계산 함수 시작
    LOG_FUNCTION_EXECUTION("calculateHP"); // 함수 실행 로그 기록
    if (yokai == NULL) return 0.0f; // 요괴가 NULL인 경우 0 반환
    // HP = 체력 종족값 * (1 + 레벨^2/(100 - log10(레벨+1)*7)) * 0.06

    float hp = yokai->stamina * (1.0f + (yokai->level * yokai->level) / (100.0f - log10(yokai->level + 1) *7)) * 0.06f; // HP 공식 계산
    return (float)((int)hp); // 소수점 버림하여 반환
}

// HP 상태 확인 함수
// 현재 HP 비율에 따라 상태 메시지를 반환
const char* getHPStatus(const Yokai* yokai) { // HP 상태 확인 함수 시작
    LOG_FUNCTION_EXECUTION("getHPStatus"); // 함수 실행 로그 기록
    // 기절 상태일 때만 "기절" 반환, 그 외에는 빈 문자열 반환
    if (yokai->status == YOKAI_FAINTED) { // 기절 상태인 경우
        return "기절"; // 기절 메시지 반환
    }
    return ""; // 빈 문자열 반환
}

// HP 바 출력 함수
// 요괴의 현재 HP 상태를 시각적으로 표시
void printHPBar(const Yokai* yokai) { // HP 바 출력 함수 시작
    LOG_FUNCTION_EXECUTION("printHPBar"); // 함수 실행 로그 기록
    if (yokai == NULL) return; // 요괴가 NULL인 경우 함수 종료
    
    // 최대 HP와 현재 HP 비율 계산
    float maxHP = calculateHP(yokai); // 최대 HP 계산
    float currentHP = yokai->currentHP; // 현재 HP 가져오기
    float hpPercentage = (currentHP / maxHP) * 100.0f; // HP 비율 계산
    
    // HP 바 길이 계산
    int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH); // 채워진 길이 계산
    
    // HP 바 전체를 하나의 문자열로 구성
    char buffer[512]; // 출력 버퍼
    sprintf(buffer, "\n%s의 HP: %.0f/%.0f\n[", yokai->name, currentHP, maxHP); // HP 정보 메시지 생성
    
    // HP 비율에 따른 색상 설정
    if (hpPercentage <= 20.0f) { // HP가 20% 이하인 경우
        strcat(buffer, "\033[31m"); // 빨간색 (위험)
    } else if (hpPercentage <= 50.0f) { // HP가 50% 이하인 경우
        strcat(buffer, "\033[33m"); // 노란색 (주의)
    } else { // HP가 50% 초과인 경우
        strcat(buffer, "\033[1;32m"); // 초록색 (양호)
    }
    
    // HP 바 시각화
    for (int i = 0; i < HP_BAR_LENGTH; i++) { // HP 바 길이만큼 반복
        if (i < filledLength) { // 채워진 부분인 경우
            strcat(buffer, "█"); // 채워진 부분
        } else { // 빈 부분인 경우
            strcat(buffer, "░"); // 빈 부분
        }
    }
    
    // 색상 초기화 및 HP 바 종료
    strcat(buffer, "\033[0m]"); // 색상 초기화 및 바 종료
    
    // HP 상태 메시지 추가
    sprintf(buffer + strlen(buffer), " (%s)\n", getHPStatus(yokai)); // 상태 메시지 추가
    
    // HP 바 전체를 한 번에 출력
    printTextAndWait(buffer); // HP 바 출력 및 대기
} 