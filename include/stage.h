/**
 * @file stage.h
 * @brief 포켓요괴 게임의 스테이지 관리 헤더 파일
 * @details 스테이지 초기화, 적 생성, 정보 출력 등의 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef STAGE_H
#define STAGE_H

#include "stage_types.h" // 스테이지 타입 정의

// 현재 스테이지 정보는 state 모듈에서 관리

/**
 * @brief 스테이지를 초기화하는 함수
 * @param stage 초기화할 스테이지 정보
 * @param stageNumber 스테이지 번호
 */
void initStage(StageInfo* stage, int stageNumber);

/**
 * @brief 스테이지에 적을 생성하는 함수
 * @param stage 적을 생성할 스테이지
 */
void generateStageEnemies(StageInfo* stage);

/**
 * @brief 스테이지 정보를 출력하는 함수
 * @param stage 출력할 스테이지 정보
 */
void printStageInfo(const StageInfo* stage);

/**
 * @brief 현재 스테이지 정보를 화면에 표시하는 함수
 */
void showStageInfo();

/**
 * @brief 전투 인터페이스를 표시하는 함수
 */
void showBattleInterface();

/**
 * @brief 현재 지형 이름을 반환하는 함수
 * @return 현재 지형 이름 문자열
 */
const char* getCurrentTerrain(void);

#define FINAL_TERRAIN_COUNT 5 // 최종 지형 개수
#define FINAL_TERRAIN_SEQUENCE_LENGTH 10 // 최종 지형 시퀀스 길이
extern const char* finalTerrainNames[]; // 최종 지형 이름 배열
extern const int terrainSequence[]; // 지형 시퀀스 배열

#endif // STAGE_H 