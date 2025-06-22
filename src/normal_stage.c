// 표준 입출력 함수를 위한 헤더
#include <stdio.h>  // 표준 입출력 함수
// 표준 라이브러리 함수를 위한 헤더
#include <stdlib.h>  // 표준 라이브러리 함수
// 문자열 처리 함수를 위한 헤더
#include <string.h>  // 문자열 처리 함수
// 일반 스테이지 관련 함수와 구조체 정의
#include "normal_stage.h"  // 일반 스테이지 헤더
// 텍스트 출력 관련 함수
#include "text.h"  // 텍스트 출력 헤더
// 요괴 관련 함수
#include "yokai.h"  // 요괴 헤더
// 파티 관리 관련 함수
#include "party.h"  // 파티 관리 헤더
// 저장 파일 관련 함수
#include "savefile.h"  // 저장 파일 헤더
// 지역 관련 함수
#include "region.h"  // 지역 헤더
// 스테이지 관련 함수
#include "stage.h"  // 스테이지 헤더
// 스테이지 타입 관련 함수
#include "stage_types.h"  // 스테이지 타입 헤더
// 이벤트 시스템 관련 함수
#include "event_system.h"  // 이벤트 시스템 헤더
// 로거 관련 함수
#include "logger.h"  // 로거 헤더

// 일반 스테이지 초기화 함수
void initNormalStage(StageInfo* stage, int stageNumber) {  // 일반 스테이지 초기화 함수 시작
    LOG_FUNCTION_EXECUTION("initNormalStage");  // 함수 실행 로그 기록
    stage->stageNumber = stageNumber;  // 스테이지 번호 설정
    stage->isBossStage = false;  // 일반 스테이지 플래그 설정
    
    // 현재 지역 설정
    strcpy(stage->region, getCurrentRegion());  // 현재 지역을 스테이지에 설정
    
    // 랜덤 지형 설정 (차원의 균열 제외)
    stage->terrain = rand() % (TERRAIN_COUNT - 1);  // 랜덤 지형 선택 (차원의 균열 제외)
    strcpy(stage->terrainName, terrainNames[stage->terrain]);  // 지형 이름 설정
    
    // 레벨 범위 계산
    calculateLevelRange(stageNumber, &stage->minLevel, &stage->maxLevel);  // 레벨 범위 계산
    
    // 이벤트 발생 확률 체크
    if (shouldTriggerEvent()) {  // 이벤트 발생 조건 확인
        // 이벤트 발생 시 요괴 없이 이벤트만 실행
        stage->enemyCount = 0;  // 적 수를 0으로 설정
        Event* event = generateRandomEvent();  // 랜덤 이벤트 생성
        if (event) {  // 이벤트가 생성된 경우
            handleEvent(event);  // 이벤트 처리
        }
    } else {  // 이벤트가 발생하지 않는 경우
        // 일반 스테이지는 3-5마리
        stage->enemyCount = rand() % 3 + 3;  // 3-5마리 랜덤 적 수 설정
        // 적 요괴 생성
        generateNormalStageEnemies(stage);  // 일반 스테이지 적 요괴 생성
    }
}

// 일반 스테이지 적 요괴 생성 함수
void generateNormalStageEnemies(StageInfo* stage) {  // 일반 스테이지 적 요괴 생성 함수 시작
    LOG_FUNCTION_EXECUTION("generateNormalStageEnemies");  // 함수 실행 로그 기록
    for (int i = 0; i < stage->enemyCount; i++) {  // 모든 적 요괴 생성
        int level = stage->minLevel + (rand() % (stage->maxLevel - stage->minLevel + 1));  // 랜덤 레벨 생성
        stage->enemies[i] = createRandomYokaiWithLevel(level);  // 랜덤 요괴 생성
    }
}

// 일반 스테이지 클리어 처리 함수
void handleNormalStageClear(void) {  // 일반 스테이지 클리어 처리 함수 시작
    LOG_FUNCTION_EXECUTION("handleNormalStageClear");  // 함수 실행 로그 기록
    
    // 활성 이벤트가 있는지 확인하고 완료 조건 체크
    Event* currentEvent = getCurrentEvent();  // 현재 이벤트 가져오기
    if (currentEvent && checkEventCompletion(currentEvent)) {  // 이벤트가 있고 완료 조건을 만족하는 경우
        completeEvent(currentEvent);  // 이벤트 완료 처리
    }
    
    // 일반 스테이지 클리어 시 처리
} 