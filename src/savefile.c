#include <stdio.h> // 표준 입출력 함수 사용을 위한 헤더 파일
#include <stdlib.h> // 동적 메모리 할당, 난수 생성 등 표준 라이브러리 함수 사용 헤더 파일
#include <string.h> // 문자열 처리 함수 사용 헤더 파일
#include "savefile.h" // 저장 파일 관련 함수 및 구조체 정의 헤더 파일
#include "stage.h" // 스테이지 정보 관련 함수 및 구조체 정의 헤더 파일
#include "party.h" // 파티(동료 요괴) 관련 함수 및 구조체 정의 헤더 파일
#include "yokai.h" // 요괴 정보 관련 함수 및 구조체 정의 헤더 파일
#include "game.h" // 게임 전역 상태 및 관련 함수 정의 헤더 파일
#include "settings.h" // 게임 설정 관련 함수 및 구조체 정의 헤더 파일
#include "item.h" // 아이템 정보 및 관련 함수 정의 헤더 파일
#include "battle.h" // 전투 시스템 관련 함수 및 구조체 정의 헤더 파일
#include "text.h" // 텍스트 출력 함수 정의 헤더 파일
#include "region.h" // 지역 시스템 관련 함수 및 구조체 정의 헤더 파일
#include "event_system.h" // 이벤트 시스템 관련 함수 및 구조체 정의 헤더 파일
#include "../core/state.h" // 게임 상태 구조체 및 전역 변수 정의 헤더 파일
#include "logger.h" // 함수 실행 로그 기록 함수 정의 헤더 파일
#include "statistics.h" // 통계 시스템 관련 함수 및 구조체 정의 헤더 파일
#include "encyclopedia.h" // 도감 시스템 관련 함수 및 구조체 정의 헤더 파일
#include <windows.h> // Windows API 함수 사용을 위한 헤더 파일
#define SAVE_FILE "data/save.dat" // 저장 파일 경로를 상수로 정의

// battle.c에서 선언된 변수들을 외부에서 사용하기 위해 extern으로 선언
extern int turnCount; // 현재 턴 수(전역 변수)
extern int participatedCount; // 전투에 참여한 요괴 수(전역 변수)
extern int participatedIdx[]; // 전투에 참여한 요괴 인덱스 배열(전역 변수)
extern int lastYokaiIdx; // 마지막으로 사용한 요괴 인덱스(전역 변수)

char currentRegion[32]; // 현재 지역명을 저장할 변수(글로벌)

// 게임 저장 함수 정의 시작
void saveGame() {
    LOG_FUNCTION_EXECUTION("saveGame"); // 함수 실행 시 로그 기록
    
    // 통계 업데이트(저장 시점의 누적 통계 반영)
    update_total_statistics_on_save(); // 통계 시스템 함수 호출

    FILE* file = fopen(SAVE_FILE, "wb");  // 바이너리 쓰기 모드로 저장 파일 열기
    if (!file) { // 파일 열기에 실패한 경우
        printText("저장 실패!\n"); // 실패 메시지 출력
        return; // 함수 종료
    }
    
    if (gameSettings.debugMode) { // 디버그 모드일 때 저장 정보 출력
        char buffer[256]; // 디버그 메시지 버퍼
        sprintf(buffer, "[DEBUG] 저장 시 turnCount: %d\n지역: %s\n지형: %s\n", turnCount, gStage.region, gStage.terrainName); // 디버그 메시지 생성
        printText(buffer); // 메시지 출력
        fastSleep(500); // 0.5초 대기
    }
    
    // 지역 데이터 먼저 저장(현재 지역명, 방문 기록 등)
    saveRegionData(file); // 지역 데이터 저장 함수 호출
    
    // 스테이지 정보 저장(현재 진행 중인 스테이지 정보)
    fwrite(&gStage, sizeof(StageInfo), 1, file); // 스테이지 구조체를 파일에 저장
    
    // 동료 요괴 수 및 총 포획 요괴 수 저장
    fwrite(&gPartyCount, sizeof(int), 1, file); // 파티 요괴 수 저장
    fwrite(&gTotalCaughtCount, sizeof(int), 1, file); // 총 포획 요괴 수 저장
    
    // 동료 요괴 정보 저장(각 요괴의 상세 정보)
    for (int i = 0; i < gPartyCount; i++) { // 파티 내 모든 요괴에 대해 반복
        fwrite(&gParty[i].id, sizeof(unsigned long long), 1, file); // 요괴 고유 ID 저장
        fwrite(&gParty[i].name, sizeof(char), YOKAI_NAME_MAX, file); // 요괴 이름 저장
        fwrite(&gParty[i].level, sizeof(int), 1, file); // 요괴 레벨 저장
        fwrite(&gParty[i].exp, sizeof(int), 1, file); // 요괴 경험치 저장
        fwrite(&gParty[i].attack, sizeof(int), 1, file); // 요괴 공격력 저장
        fwrite(&gParty[i].defense, sizeof(int), 1, file); // 요괴 방어력 저장
        fwrite(&gParty[i].stamina, sizeof(int), 1, file); // 요괴 체력(스태미나) 저장
        fwrite(&gParty[i].speed, sizeof(int), 1, file); // 요괴 스피드 저장
        fwrite(&gParty[i].currentHP, sizeof(float), 1, file); // 요괴 현재 HP 저장
        fwrite(&gParty[i].type, sizeof(YokaiType), 1, file); // 요괴 타입 저장
        fwrite(&gParty[i].status, sizeof(YokaiStatus), 1, file); // 요괴 상태 저장
        fwrite(&gParty[i].desc, sizeof(char), YOKAI_DESC_MAX, file); // 요괴 설명 저장
        fwrite(&gParty[i].moveCount, sizeof(int), 1, file); // 요괴가 보유한 기술 개수 저장
        
        // 기술 정보 저장(각 기술의 상세 정보)
        for (int j = 0; j < gParty[i].moveCount; j++) {
            fwrite(&gParty[i].moves[j].move, sizeof(Move), 1, file); // 기술 구조체 저장
            fwrite(&gParty[i].moves[j].currentPP, sizeof(int), 1, file); // 기술의 현재 PP 저장
        }
        
        // 배울 수 있는 기술 목록 저장
        fwrite(&gParty[i].learnableMoveCount, sizeof(int), 1, file); // 배울 수 있는 기술 개수 저장
        for (int j = 0; j < gParty[i].learnableMoveCount; j++) {
            fwrite(&gParty[i].learnableMoves[j], sizeof(Move), 1, file); // 배울 수 있는 기술 구조체 저장
        }
        
        // 잊은 기술 목록 저장
        fwrite(&gParty[i].forgottenMoveCount, sizeof(int), 1, file); // 잊은 기술 개수 저장
        for (int j = 0; j < gParty[i].forgottenMoveCount; j++) {
            fwrite(&gParty[i].forgottenMoves[j], sizeof(Move), 1, file); // 잊은 기술 구조체 저장
        }
        
        // 요괴 인벤토리 저장(요괴가 보유한 아이템 정보)
        fwrite(&gParty[i].yokaiInventoryCount, sizeof(int), 1, file); // 인벤토리 아이템 개수 저장
        for (int j = 0; j < gParty[i].yokaiInventoryCount; j++) {
            fwrite(&gParty[i].yokaiInventory[j], sizeof(InventoryItem), 1, file); // 인벤토리 아이템 구조체 저장
        }
        
        // 돋보기 개수 저장(요괴별 보유 돋보기)
        fwrite(&gParty[i].magnifierCount, sizeof(int), 1, file); // 돋보기 개수 저장
    }
    
    // 플레이어 정보 저장(플레이어 구조체 전체)
    fwrite(&gPlayer, sizeof(Player), 1, file); // 플레이어 정보 구조체를 파일에 저장
    
    // 인벤토리 개수 저장(플레이어가 보유한 전체 아이템 개수)
    fwrite(&inventoryCount, sizeof(int), 1, file); // 인벤토리 아이템 개수 저장
    
    // 인벤토리 아이템 정보 저장(각 아이템의 상세 정보)
    for (int i = 0; i < inventoryCount; i++) { // 인벤토리 내 모든 아이템에 대해 반복
        fwrite(&inventory[i], sizeof(InventoryItem), 1, file); // 인벤토리 아이템 구조체 저장
    }
    
    // 수동 저장이 아닌 경우에만 다음 스테이지의 상대 요괴 정보 저장
    if (!gGameState.isManualSave) { // 자동 저장(일반 진행)일 때
        Yokai nextEnemy; // 다음 스테이지의 적 요괴 정보 임시 변수
        if (gStage.isBossStage) { // 보스 스테이지인 경우
            nextEnemy = gStage.enemies[0];  // 첫 번째 적 요괴 사용
        } else {
            nextEnemy = gStage.enemies[rand() % gStage.enemyCount];  // 랜덤 적 요괴 선택
        }
        
        fwrite(&nextEnemy.name, sizeof(char), YOKAI_NAME_MAX, file); // 적 요괴 이름 저장
        fwrite(&nextEnemy.level, sizeof(int), 1, file); // 적 요괴 레벨 저장
        fwrite(&nextEnemy.exp, sizeof(int), 1, file); // 적 요괴 경험치 저장
        fwrite(&nextEnemy.attack, sizeof(int), 1, file); // 적 요괴 공격력 저장
        fwrite(&nextEnemy.defense, sizeof(int), 1, file); // 적 요괴 방어력 저장
        fwrite(&nextEnemy.stamina, sizeof(int), 1, file); // 적 요괴 체력 저장
        fwrite(&nextEnemy.speed, sizeof(int), 1, file); // 적 요괴 스피드 저장
        fwrite(&nextEnemy.currentHP, sizeof(float), 1, file); // 적 요괴 현재 HP 저장
        fwrite(&nextEnemy.type, sizeof(YokaiType), 1, file); // 적 요괴 타입 저장
        fwrite(&nextEnemy.status, sizeof(YokaiStatus), 1, file); // 적 요괴 상태 저장
        fwrite(&nextEnemy.desc, sizeof(char), 128, file); // 적 요괴 설명 저장
        fwrite(&nextEnemy.moveCount, sizeof(int), 1, file); // 적 요괴 기술 개수 저장
        
        // 적 요괴의 기술 정보 저장
        for (int j = 0; j < nextEnemy.moveCount; j++) {
            fwrite(&nextEnemy.moves[j].move, sizeof(Move), 1, file); // 기술 구조체 저장
            fwrite(&nextEnemy.moves[j].currentPP, sizeof(int), 1, file); // 기술의 현재 PP 저장
        }
    } else { // 수동 저장(전투 중 저장)일 때
        // 현재 전투 중인 상대 요괴 정보 저장
        fwrite(&currentEnemy.name, sizeof(char), YOKAI_NAME_MAX, file); // 적 요괴 이름 저장
        fwrite(&currentEnemy.level, sizeof(int), 1, file); // 적 요괴 레벨 저장
        fwrite(&currentEnemy.exp, sizeof(int), 1, file); // 적 요괴 경험치 저장
        fwrite(&currentEnemy.attack, sizeof(int), 1, file); // 적 요괴 공격력 저장
        fwrite(&currentEnemy.defense, sizeof(int), 1, file); // 적 요괴 방어력 저장
        fwrite(&currentEnemy.stamina, sizeof(int), 1, file); // 적 요괴 체력 저장
        fwrite(&currentEnemy.speed, sizeof(int), 1, file); // 적 요괴 스피드 저장
        fwrite(&currentEnemy.currentHP, sizeof(float), 1, file); // 적 요괴 현재 HP 저장
        fwrite(&currentEnemy.type, sizeof(YokaiType), 1, file); // 적 요괴 타입 저장
        fwrite(&currentEnemy.status, sizeof(YokaiStatus), 1, file); // 적 요괴 상태 저장
        fwrite(&currentEnemy.desc, sizeof(char), 128, file); // 적 요괴 설명 저장
        fwrite(&currentEnemy.moveCount, sizeof(int), 1, file); // 적 요괴 기술 개수 저장
        
        // 적 요괴의 기술 정보 저장
        for (int j = 0; j < currentEnemy.moveCount; j++) {
            fwrite(&currentEnemy.moves[j].move, sizeof(Move), 1, file); // 기술 구조체 저장
            fwrite(&currentEnemy.moves[j].currentPP, sizeof(int), 1, file); // 기술의 현재 PP 저장
        }
    }
    
    // 턴 카운트 저장(현재 전투의 턴 수)
    fwrite(&turnCount, sizeof(int), 1, file); // 턴 카운트 저장
    
    // 전투에 참여한 요괴 수 저장
    fwrite(&participatedCount, sizeof(int), 1, file); // 전투 참여 요괴 수 저장
    
    // 전투에 참여한 요괴 목록 저장(인덱스 배열)
    fwrite(participatedIdx, sizeof(int), participatedCount, file); // 참여 요괴 인덱스 배열 저장
    
    // 마지막으로 사용한 요괴 인덱스 저장
    fwrite(&lastYokaiIdx, sizeof(int), 1, file); // 마지막 사용 요괴 인덱스 저장
    
    // 이벤트 데이터 저장(진행 중인 이벤트 정보)
    Event* currentEvent = getCurrentEvent(); // 현재 이벤트 포인터 가져오기
    if (currentEvent) { // 이벤트가 존재하면
        int hasActiveEvent = 1; // 이벤트 활성화 플래그
        fwrite(&hasActiveEvent, sizeof(int), 1, file); // 플래그 저장
        
        // 이벤트 데이터 저장(상세 정보)
        fwrite(&currentEvent->type, sizeof(EventType), 1, file); // 이벤트 타입 저장
        fwrite(&currentEvent->description, sizeof(char), 256, file); // 이벤트 설명 저장
        fwrite(&currentEvent->target_region, sizeof(char), 32, file); // 이벤트 대상 지역명 저장
        fwrite(&currentEvent->reward_money, sizeof(int), 1, file); // 이벤트 보상 금액 저장
        fwrite(&currentEvent->is_completed, sizeof(bool), 1, file); // 이벤트 완료 여부 저장
    } else { // 이벤트가 없으면
        int hasActiveEvent = 0; // 이벤트 비활성화 플래그
        fwrite(&hasActiveEvent, sizeof(int), 1, file); // 플래그 저장
    }
    
    fclose(file); // 파일 닫기
    
    // 잡은 요괴 데이터 저장(별도 파일 또는 시스템)
    saveCaughtYokaiData(); // 잡은 요괴 데이터 저장 함수 호출
    
    printText("게임이 저장되었습니다.\n"); // 저장 완료 메시지 출력
}

// 게임 데이터 불러오기 함수 정의 시작
int loadGameData() {
    LOG_FUNCTION_EXECUTION("loadGameData"); // 함수 실행 시 로그 기록
    FILE* file = fopen(SAVE_FILE, "rb");  // 바이너리 읽기 모드로 저장 파일 열기
    if (!file) { // 파일 열기에 실패한 경우
        printText("저장 파일이 없습니다.\n"); // 실패 메시지 출력
        return 0; // 실패 반환
    }
    
    // 지역 데이터 먼저 불러오기(현재 지역명, 방문 기록 등)
    loadRegionData(file); // 지역 데이터 불러오기 함수 호출
    
    // 스테이지 정보 불러오기(현재 진행 중인 스테이지 정보)
    fread(&gStage, sizeof(StageInfo), 1, file); // 스테이지 구조체를 파일에서 읽어옴
    
    // 이어하기 시 지역명이 바뀌는 문제 해결: currentRegion을 gStage.region으로 덮어씀
    strcpy(currentRegion, gStage.region); // 스테이지의 지역명을 현재 지역명으로 복사
    
    // 파이널 스테이지일 경우 지형 이름 복원
    if (gStage.stageNumber >= 81) { // 파이널 스테이지(81 이상)인 경우
        int terrainIndex = gStage.stageNumber - 81; // 파이널 스테이지 인덱스 계산
        if (terrainIndex >= 0 && terrainIndex < FINAL_TERRAIN_SEQUENCE_LENGTH) {
            strcpy(gStage.terrainName, finalTerrainNames[terrainSequence[terrainIndex]]); // 지형 이름 복원
        } else {
            strcpy(gStage.terrainName, finalTerrainNames[FINAL_TERRAIN_COUNT - 1]); // 마지막 지형 이름으로 복원
        }
    }
    
    // 동료 요괴 수 및 총 포획 요괴 수 불러오기
    fread(&gPartyCount, sizeof(int), 1, file); // 파티 요괴 수 불러오기
    fread(&gTotalCaughtCount, sizeof(int), 1, file); // 총 포획 요괴 수 불러오기
    
    // 동료 요괴 정보 불러오기(각 요괴의 상세 정보)
    for (int i = 0; i < gPartyCount; i++) { // 파티 내 모든 요괴에 대해 반복
        fread(&gParty[i].id, sizeof(unsigned long long), 1, file); // 요괴 고유 ID 불러오기
        fread(&gParty[i].name, sizeof(char), YOKAI_NAME_MAX, file); // 요괴 이름 불러오기
        fread(&gParty[i].level, sizeof(int), 1, file); // 요괴 레벨 불러오기
        fread(&gParty[i].exp, sizeof(int), 1, file); // 요괴 경험치 불러오기
        fread(&gParty[i].attack, sizeof(int), 1, file); // 요괴 공격력 불러오기
        fread(&gParty[i].defense, sizeof(int), 1, file); // 요괴 방어력 불러오기
        fread(&gParty[i].stamina, sizeof(int), 1, file); // 요괴 체력(스태미나) 불러오기
        fread(&gParty[i].speed, sizeof(int), 1, file); // 요괴 스피드 불러오기
        fread(&gParty[i].currentHP, sizeof(float), 1, file); // 요괴 현재 HP 불러오기
        fread(&gParty[i].type, sizeof(YokaiType), 1, file); // 요괴 타입 불러오기
        fread(&gParty[i].status, sizeof(YokaiStatus), 1, file); // 요괴 상태 불러오기
        fread(&gParty[i].desc, sizeof(char), YOKAI_DESC_MAX, file); // 요괴 설명 불러오기
        fread(&gParty[i].moveCount, sizeof(int), 1, file); // 요괴가 보유한 기술 개수 불러오기
        
        // 기술 정보 불러오기(각 기술의 상세 정보)
        for (int j = 0; j < gParty[i].moveCount; j++) {
            fread(&gParty[i].moves[j].move, sizeof(Move), 1, file); // 기술 구조체 불러오기
            fread(&gParty[i].moves[j].currentPP, sizeof(int), 1, file); // 기술의 현재 PP 불러오기
        }
        
        // 배울 수 있는 기술 목록 불러오기
        fread(&gParty[i].learnableMoveCount, sizeof(int), 1, file); // 배울 수 있는 기술 개수 불러오기
        for (int j = 0; j < gParty[i].learnableMoveCount; j++) {
            fread(&gParty[i].learnableMoves[j], sizeof(Move), 1, file); // 배울 수 있는 기술 구조체 불러오기
        }
        
        // 잊은 기술 목록 불러오기
        fread(&gParty[i].forgottenMoveCount, sizeof(int), 1, file); // 잊은 기술 개수 불러오기
        for (int j = 0; j < gParty[i].forgottenMoveCount; j++) {
            fread(&gParty[i].forgottenMoves[j], sizeof(Move), 1, file); // 잊은 기술 구조체 불러오기
        }
        
        // 요괴 인벤토리 불러오기(요괴가 보유한 아이템 정보)
        fread(&gParty[i].yokaiInventoryCount, sizeof(int), 1, file); // 인벤토리 아이템 개수 불러오기
        for (int j = 0; j < gParty[i].yokaiInventoryCount; j++) {
            fread(&gParty[i].yokaiInventory[j], sizeof(InventoryItem), 1, file); // 인벤토리 아이템 구조체 불러오기
        }
        
        // 돋보기 개수 불러오기(요괴별 보유 돋보기)
        fread(&gParty[i].magnifierCount, sizeof(int), 1, file); // 돋보기 개수 불러오기
        
        // 복숭아 개수 제한 확인(5개 초과 시 5개로 제한)
        for (int j = 0; j < gParty[i].yokaiInventoryCount; j++) {
            if (strcmp(gParty[i].yokaiInventory[j].item.name, "복숭아") == 0) {
                if (gParty[i].yokaiInventory[j].count > 5) {
                    gParty[i].yokaiInventory[j].count = 5;
                }
                break;
            }
        }
    }
    
    // 플레이어 정보 불러오기(플레이어 구조체 전체)
    fread(&gPlayer, sizeof(Player), 1, file); // 플레이어 정보 구조체를 파일에서 읽어옴
    
    // 인벤토리 개수 불러오기(플레이어가 보유한 전체 아이템 개수)
    fread(&inventoryCount, sizeof(int), 1, file); // 인벤토리 아이템 개수 불러오기
    
    // 인벤토리 아이템 정보 불러오기(각 아이템의 상세 정보)
    for (int i = 0; i < inventoryCount; i++) { // 인벤토리 내 모든 아이템에 대해 반복
        fread(&inventory[i], sizeof(InventoryItem), 1, file); // 인벤토리 아이템 구조체 불러오기
    }
    
    // 현재 전투 중인 상대 요괴 정보 불러오기(이름, 레벨, 능력치, 기술 등)
    fread(&currentEnemy.name, sizeof(char), YOKAI_NAME_MAX, file); // 적 요괴 이름 불러오기
    fread(&currentEnemy.level, sizeof(int), 1, file); // 적 요괴 레벨 불러오기
    fread(&currentEnemy.exp, sizeof(int), 1, file); // 적 요괴 경험치 불러오기
    fread(&currentEnemy.attack, sizeof(int), 1, file); // 적 요괴 공격력 불러오기
    fread(&currentEnemy.defense, sizeof(int), 1, file); // 적 요괴 방어력 불러오기
    fread(&currentEnemy.stamina, sizeof(int), 1, file); // 적 요괴 체력 불러오기
    fread(&currentEnemy.speed, sizeof(int), 1, file); // 적 요괴 스피드 불러오기
    fread(&currentEnemy.currentHP, sizeof(float), 1, file); // 적 요괴 현재 HP 불러오기
    fread(&currentEnemy.type, sizeof(YokaiType), 1, file); // 적 요괴 타입 불러오기
    fread(&currentEnemy.status, sizeof(YokaiStatus), 1, file); // 적 요괴 상태 불러오기
    fread(&currentEnemy.desc, sizeof(char), 128, file); // 적 요괴 설명 불러오기
    fread(&currentEnemy.moveCount, sizeof(int), 1, file); // 적 요괴 기술 개수 불러오기
    
    // 적 요괴의 기술 정보 불러오기(각 기술의 상세 정보)
    for (int j = 0; j < currentEnemy.moveCount; j++) {
        fread(&currentEnemy.moves[j].move, sizeof(Move), 1, file); // 기술 구조체 불러오기
        fread(&currentEnemy.moves[j].currentPP, sizeof(int), 1, file); // 기술의 현재 PP 불러오기
    }
    
    // 턴 카운트 불러오기(현재 전투의 턴 수)
    fread(&turnCount, sizeof(int), 1, file); // 턴 카운트 불러오기
    
    if (gameSettings.debugMode) { // 디버그 모드일 때 불러온 정보 출력
        char buffer[256]; // 디버그 메시지 버퍼
        sprintf(buffer, "[DEBUG] 로드 시 turnCount: %d\n", turnCount); // 턴 카운트 출력
        printText(buffer); // 메시지 출력
        sprintf(buffer, "[DEBUG] 로드 시 스테이지 번호: %d\n", gStage.stageNumber); // 스테이지 번호 출력
        printText(buffer); // 메시지 출력
        sprintf(buffer, "[DEBUG] 로드 시 지역: %s\n", getCurrentRegion()); // 지역명 출력
        printText(buffer); // 메시지 출력
        sprintf(buffer, "[DEBUG] 로드 시 지형: %s\n", getCurrentTerrain()); // 지형명 출력
        printText(buffer); // 메시지 출력
        fastSleep(500); // 0.5초 대기
    }
    
    // 전투에 참여한 요괴 수 불러오기
    fread(&participatedCount, sizeof(int), 1, file); // 전투 참여 요괴 수 불러오기
    
    // 전투에 참여한 요괴 목록 불러오기(인덱스 배열)
    fread(participatedIdx, sizeof(int), participatedCount, file); // 참여 요괴 인덱스 배열 불러오기
    
    // 마지막으로 사용한 요괴 인덱스 불러오기
    fread(&lastYokaiIdx, sizeof(int), 1, file); // 마지막 사용 요괴 인덱스 불러오기
    
    // 이벤트 데이터 불러오기(진행 중인 이벤트 정보)
    int hasActiveEvent; // 이벤트 활성화 플래그
    fread(&hasActiveEvent, sizeof(int), 1, file); // 플래그 불러오기
    if (hasActiveEvent) { // 이벤트가 존재하면
        Event* loadedEvent = loadEventFromFile(file); // 파일에서 이벤트 정보 불러오기
        if (loadedEvent) {
            setCurrentEvent(loadedEvent); // 현재 이벤트로 설정
        }
    }
    
    fclose(file); // 파일 닫기
    gGameState.isLoadedGame = 1; // 이어하기 플래그 설정
    return 1; // 성공 반환
}

// 저장 파일 삭제 함수
void removeSaveFile() {
    LOG_FUNCTION_EXECUTION("removeSaveFile");
    if (remove(SAVE_FILE) == 0) {
        printText("저장 파일이 삭제되었습니다.\n");
    } else {
        printText("저장 파일을 삭제하지 못했습니다.\n");
    }
} 