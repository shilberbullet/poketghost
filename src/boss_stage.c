#include <stdio.h>  // 표준 입출력 함수
#include <stdlib.h>  // 표준 라이브러리 함수
#include <string.h>  // 문자열 처리 함수
#include "boss_stage.h"  // 보스 스테이지 관련 헤더
#include "text.h"  // 텍스트 출력 관련 헤더
#include "yokai.h"  // 요괴 관련 헤더
#include "party.h"  // 파티 관련 헤더
#include "savefile.h"  // 저장 파일 관련 헤더
#include "hp_system.h"  // HP 시스템 관련 헤더
#include "region.h"  // 지역 관련 헤더
#include "../core/state.h"  // 게임 상태 관련 헤더
#include "logger.h"  // 로거 관련 헤더
#include <windows.h>  // Windows API 함수

// 보스 스테이지 초기화 함수
void initBossStage(StageInfo* stage, int stageNumber) {  // 보스 스테이지 초기화 함수 시작
    LOG_FUNCTION_EXECUTION("initBossStage");  // 함수 실행 로그 기록
    stage->stageNumber = stageNumber;  // 스테이지 번호 설정
    stage->isBossStage = true;  // 보스 스테이지 플래그 설정
    // stage->hour = 0;  // 시간 초기화 제거
    
    // 현재 지역 설정
    strcpy(stage->region, getCurrentRegion());  // 현재 지역명을 스테이지에 설정
    
    // 레벨 범위 계산
    calculateLevelRange(stageNumber, &stage->minLevel, &stage->maxLevel);  // 스테이지 번호에 따른 레벨 범위 계산
    
    // 보스 스테이지는 1마리
    stage->enemyCount = 1;  // 적 개수를 1로 설정 (보스는 1마리)
    
    // 적 요괴 생성
    generateBossStageEnemies(stage);  // 보스 스테이지 적 요괴 생성 함수 호출
}

// 보스 스테이지 적 요괴 생성 함수
void generateBossStageEnemies(StageInfo* stage) {  // 보스 스테이지 적 요괴 생성 함수 시작
    LOG_FUNCTION_EXECUTION("generateBossStageEnemies");  // 함수 실행 로그 기록
    int level = stage->minLevel + (rand() % (stage->maxLevel - stage->minLevel + 1));  // 랜덤 레벨 생성
    stage->enemies[0] = createRandomBossYokaiWithLevel(level);  // 랜덤 보스 요괴를 지정된 레벨로 생성
}

// 보스 스테이지 클리어 처리 함수
// 반환값: 1 = finalstage 진입 조건 충족, 0 = 일반 보스 클리어
int handleBossStageClear(void) {  // 보스 스테이지 클리어 처리 함수 시작
    LOG_FUNCTION_EXECUTION("handleBossStageClear");  // 함수 실행 로그 기록
    // 보스 스테이지 클리어 시 처리
    resetAllYokaiPP();  // 모든 요괴의 PP를 초기화
    printTextAndWait("\n모든 동료 요괴의 기술 PP가 초기화되었습니다!");  // PP 초기화 메시지 출력
    // 모든 동료 요괴의 HP와 상태 회복
    for (int i = 0; i < gPartyCount; i++) {  // 파티의 모든 요괴에 대해 반복
        float maxHP = calculateHP(&gParty[i]);  // 요괴의 최대 HP 계산
        gParty[i].currentHP = maxHP;  // 현재 HP를 최대 HP로 설정
        gParty[i].status = YOKAI_NORMAL;  // 상태를 정상으로 설정
    }
    printTextAndWait("\n모든 동료 요괴의 HP와 상태가 회복되었습니다!");  // 회복 완료 메시지 출력
    fastSleep(500);  // 0.5초 대기
    saveGame();  // 게임 저장
    // 60스테이지 클리어 시 파이널 스테이지 진입 조건 체크 제거
    return 0;  // 일반 보스 클리어 반환
} 