/**
 * @file game.c
 * @brief 포켓요괴 게임의 핵심 게임 로직 구현 파일
 * @details 게임 초기화, 전화 관리, 게임 상태 관리 등의 함수를 구현합니다.
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

// 표준 입출력 함수를 위한 헤더
#include <stdio.h> // 표준 입출력 함수
// 표준 라이브러리 함수를 위한 헤더
#include <stdlib.h> // 표준 라이브러리 함수
// 문자열 처리 함수를 위한 헤더
#include <string.h> // 문자열 처리 함수
// 시간 관련 함수를 위한 헤더
#include <time.h> // 시간 관련 함수
// 게임 관련 함수와 구조체 정의
#include "game.h" // 게임 관련 헤더
// 텍스트 출력 관련 함수
#include "text.h" // 텍스트 출력 헤더
// 파티 관리 관련 함수
#include "party.h" // 파티 관리 헤더
// 아이템 관련 함수
#include "item.h" // 아이템 관련 헤더
// 로그라이트 시스템 관련 함수
#include "roguelite.h" // 로그라이트 시스템 헤더
// 이벤트 시스템 관련 함수
#include "event_system.h" // 이벤트 시스템 헤더
// 로거 관련 함수
#include "logger.h" // 로거 헤더
// 게임 상태 관련 함수
#include "../core/state.h" // 게임 상태 헤더
// 입력 처리 관련 함수
#include "input.h" // 입력 처리 헤더
// 스테이지 관련 함수
#include "stage.h" // 스테이지 헤더
// 전투 관련 함수
#include "battle.h" // 전투 헤더
// 보상 관련 함수
#include "reward.h" // 보상 헤더
// 저장 파일 관련 함수
#include "savefile.h" // 저장 파일 헤더
// 대화 시스템 관련 함수
#include "dialogue.h" // 대화 시스템 헤더
// 윈도우 API 관련 함수
#include <windows.h> // 윈도우 API
// 수학 함수를 위한 헤더
#include <math.h> // 수학 함수

// 상수 정의
#define MAX_PARTY 6 // 파티의 최대 요괴 수
#define MAX_INVENTORY 50 // 인벤토리의 최대 아이템 수

// 전역 변수 선언
extern int turnCount; // 현재 턴 수
extern int participatedCount; // 전투에 참여한 요괴 수
extern int participatedIdx[]; // 전투에 참여한 요괴 인덱스 배열
extern unsigned long long participatedId[]; // 전투에 참여한 요괴 ID 배열
// extern int lastYokaiIdx; // 마지막 요괴 인덱스 (필요시 주석 해제)

// 전역 게임 상태와 플레이어 정보는 state 모듈에서 관리
 
/**
 * @brief 게임을 초기화하는 함수
 * @details 게임 시작 시 필요한 모든 시스템을 초기화합니다.
 */
void initGame() { // 게임 초기화 함수 시작
    // 로그 시스템 초기화
    initLogger(); // 로깅 시스템 초기화
    
    // 이벤트 시스템 초기화
    initEventSystem(); // 이벤트 시스템 초기화

    // 게임의 기본 상태를 초기화
    gGameState.isRunning = 1;        // 게임 실행 상태를 true로 설정
    gGameState.currentStage = 1;     // 현재 스테이지를 1로 설정
    gGameState.currentTime = 0;      // 게임 시간을 0으로 초기화
    
    // 이어하기가 아닌 새로운 게임인 경우에만 플레이어 정보 초기화
    if (!gGameState.isLoadedGame) { // 로드된 게임이 아닌 경우
        gGameState.isNewGame = 1;     // 새 게임 플래그를 true로 설정
        gPlayer.money = 0;            // 초기 전을 0으로 설정
        inventoryCount = 0;          // 인벤토리 아이템 수를 0으로 초기화
        
        // 아이템 데이터를 파일에서 로드
        loadItemsFromFile("data/items.txt"); // 아이템 데이터 로드
    }
}

/**
 * @brief 플레이어의 전화를 증가시키는 함수
 * @param amount 증가시킬 전화 양
 */
void addMoney(int amount) { // 전화 추가 함수 시작
    // 무당방울 아이템의 효과를 계산 (최대 5개까지 효과 적용)
    int mudangCount = 0; // 무당방울 개수 초기화
    for (int i = 0; i < inventoryCount; i++) { // 인벤토리 전체 검사
        if (strcmp(inventory[i].item.name, "무당방울") == 0) { // 무당방울 찾기
            mudangCount = inventory[i].count; // 무당방울 개수 저장
            if (mudangCount > 5) mudangCount = 5; // 최대 5개까지만 효과 적용
            break; // 찾았으면 루프 종료
        }
    }
    // 무당방울 효과로 추가 전 계산 (무당방울 1개당 10% 보너스)
    int bonus = (amount * 10 * mudangCount) / 100; // 보너스 금액 계산
    int total = amount + bonus; // 총 획득 금액 계산
    gPlayer.money += total; // 플레이어 전화에 추가
    
    // 획득한 전에 대한 메시지 출력
    char buffer[128]; // 메시지 버퍼
    if (mudangCount > 0) { // 무당방울 효과가 있는 경우
        sprintf(buffer, "\n%d전을 획득했습니다! (무당방울 보너스 +%d%%, 총 %d전, 현재 보유: %d전)\n",
                amount, mudangCount*10, total, gPlayer.money); // 보너스 정보 포함 메시지
    } else { // 무당방울 효과가 없는 경우
        sprintf(buffer, "\n%d전을 획득했습니다! (현재 보유: %d전)\n", amount, gPlayer.money); // 기본 메시지
    }
    printText(buffer); // 메시지 출력
}

/**
 * @brief 현재 보유 전화를 화면에 표시하는 함수
 */
void showMoney() { // 전화 표시 함수 시작
    char buffer[128]; // 메시지 버퍼
    sprintf(buffer, "\n현재 보유 전: %d전\n", gPlayer.money); // 현재 전화 정보
    printText(buffer); // 메시지 출력
}

/**
 * @brief 게임 상태를 완전히 초기화하는 함수
 * @details 게임 재시작 시 상태를 리셋합니다.
 */
void resetGameState() { // 게임 상태 리셋 함수 시작
    // 게임 상태를 기본값으로 초기화
    gGameState.isRunning = 1;        // 게임 실행 상태를 true로 설정
    gGameState.currentStage = 1;     // 현재 스테이지를 1로 설정
    gGameState.currentTime = 0;      // 게임 시간을 0으로 초기화
    gGameState.isNewGame = 1;        // 새 게임 플래그를 true로 설정
    gGameState.isLoadedGame = 0;     // 이어하기 플래그를 false로 설정
    gGameState.isManualSave = 0;     // 수동 저장 플래그를 false로 설정
    gGameState.skipCurrentStageAutoSave = 0; // 현재 스테이지 자동 저장 건너뛰기 플래그를 false로 설정
    gPlayer.money = 0;               // 전을 0으로 초기화
    inventoryCount = 0;             // 인벤토리 아이템 수를 0으로 초기화
    
    // 아이템 데이터를 다시 로드
    loadItemsFromFile("data/items.txt"); // 아이템 데이터 재로드
    
    // 낡은 부적 5개를 다시 추가
    Item* oldTalisman = NULL; // 낡은 부적 포인터 초기화
    for (int i = 0; i < itemListCount; i++) { // 아이템 목록 검사
        if (strcmp(itemList[i].name, "낡은부적") == 0) { // 낡은 부적 찾기
            oldTalisman = &itemList[i]; // 낡은 부적 포인터 설정
            break; // 찾았으면 루프 종료
        }
    }
    if (oldTalisman) { // 낡은 부적을 찾은 경우
        for (int i = 0; i < 5; i++) { // 5개 추가
            addItemToInventoryWithoutMessage(oldTalisman); // 메시지 없이 인벤토리에 추가
        }
    }
}

/**
 * @brief 게임 종료 시 정리 작업을 수행하는 함수
 * @details 메모리 해제 등의 정리 작업을 수행합니다.
 */
void cleanupGame() { // 게임 정리 함수 시작
    cleanupLogger(); // 로거 정리
}

/**
 * @brief 게임 클리어 후 데이터를 초기화하는 함수
 * @details 게임 클리어 후 다음 게임을 위한 초기화를 수행합니다.
 */
void resetGameAfterClear() { // 게임 클리어 후 리셋 함수 시작
    logMessage("게임 클리어 후 데이터 초기화 시작"); // 초기화 시작 로그
    
    // 게임 상태 초기화
    gGameState.isRunning = 0; // 게임 실행 상태를 false로 설정
    gGameState.currentStage = 1; // 현재 스테이지를 1로 설정
    gGameState.currentTime = 0; // 게임 시간을 0으로 초기화
    gGameState.isNewGame = 1; // 새 게임 플래그를 true로 설정
    gGameState.isLoadedGame = 0; // 이어하기 플래그를 false로 설정
    gGameState.isManualSave = 0; // 수동 저장 플래그를 false로 설정
    gGameState.skipCurrentStageAutoSave = 0; // 현재 스테이지 자동 저장 건너뛰기 플래그를 false로 설정
    
    // 플레이어 데이터 초기화
    gPlayer.money = 0; // 전화를 0으로 초기화
    
    // 스테이지 데이터 초기화
    gStage.stageNumber = 1; // 스테이지 번호를 1로 설정
    gStage.isBossStage = false; // 보스 스테이지 플래그를 false로 설정
    gStage.hour = 0; // 시간을 0으로 초기화
    strcpy(gStage.region, ""); // 지역명을 빈 문자열로 초기화
    strcpy(gStage.terrainName, ""); // 지형명을 빈 문자열로 초기화
    gStage.terrain = 0; // 지형을 0으로 초기화
    gStage.enemyCount = 0; // 적 수를 0으로 초기화
    
    // 파티 데이터 초기화
    gPartyCount = 0; // 파티 요괴 수를 0으로 초기화
    for (int i = 0; i < MAX_PARTY; i++) { // 파티 배열 초기화
        memset(&gParty[i], 0, sizeof(Yokai)); // 요괴 구조체를 0으로 초기화
    }
    
    // 인벤토리 초기화
    inventoryCount = 0; // 인벤토리 아이템 수를 0으로 초기화
    for (int i = 0; i < MAX_INVENTORY; i++) { // 인벤토리 배열 초기화
        memset(&inventory[i], 0, sizeof(InventoryItem)); // 인벤토리 아이템을 0으로 초기화
    }
    
    // 전투 관련 변수 초기화
    turnCount = 0; // 턴 수를 0으로 초기화
    participatedCount = 0; // 참여 요괴 수를 0으로 초기화
    for (int i = 0; i < MAX_PARTY; i++) { // 참여 요괴 배열 초기화
        participatedIdx[i] = 0; // 참여 요괴 인덱스를 0으로 초기화
        participatedId[i] = 0; // 참여 요괴 ID를 0으로 초기화
    }
    
    // 아이템 보상 시스템 초기화
    resetItemRewardSystem(); // 아이템 보상 시스템 리셋
    
    // 저장 파일 삭제
    removeSaveFile(); // 저장 파일 제거
    
    logMessage("게임 클리어 후 데이터 초기화 완료"); // 초기화 완료 로그
} 