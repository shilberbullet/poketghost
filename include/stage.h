/**
 * @file stage.h
 * @brief 포켓요괴 게임의 스테이지 관리 헤더 파일
 * @details 스테이지 초기화, 적 생성, 정보 출력 등의 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef STAGE_H
#define STAGE_H

#include "stage_types.h" // 스테이지 타입 정의 - 스테이지 정보 구조체 사용

// 현재 스테이지 정보는 state 모듈에서 관리 - 전역 상태로 현재 스테이지 데이터 저장

/**
 * @brief 스테이지를 초기화하는 함수
 * @param stage 초기화할 스테이지 정보 - 스테이지 데이터 구조체 포인터
 * @param stageNumber 스테이지 번호 - 초기화할 스테이지의 순서 번호
 * @details 새로운 스테이지 시작 시 기본 정보와 적 배치를 초기화
 */
void initStage(StageInfo* stage, int stageNumber);

/**
 * @brief 스테이지에 적을 생성하는 함수
 * @param stage 적을 생성할 스테이지 - 스테이지 정보 구조체 포인터
 * @details 스테이지 난이도에 따라 적 요괴들을 랜덤하게 생성하고 배치
 */
void generateStageEnemies(StageInfo* stage);

/**
 * @brief 스테이지 정보를 출력하는 함수
 * @param stage 출력할 스테이지 정보 - 스테이지 데이터 구조체 포인터
 * @details 스테이지의 이름, 난이도, 적 정보 등을 화면에 표시
 */
void printStageInfo(const StageInfo* stage);

/**
 * @brief 현재 스테이지 정보를 화면에 표시하는 함수
 * @details 플레이어가 현재 위치한 스테이지의 상세 정보를 출력
 */
void showStageInfo();

/**
 * @brief 전투 인터페이스를 표시하는 함수
 * @details 전투 시작 시 플레이어에게 전투 옵션과 적 정보를 보여주는 UI 표시
 */
void showBattleInterface();

/**
 * @brief 다음 스테이지로 진행하는 함수
 * @details 현재 스테이지를 완료하고 다음 스테이지로 이동
 */
void nextStage();

/**
 * @brief 현재 지형 이름을 반환하는 함수
 * @return 현재 지형 이름 문자열 - 플레이어가 위치한 지형의 한글 이름
 * @details 현재 스테이지의 지형 타입에 해당하는 이름을 반환
 */
const char* getCurrentTerrain(void);

#define FINAL_TERRAIN_COUNT 5 // 최종 지형 개수 - 게임 클리어까지의 총 지형 수
#define FINAL_TERRAIN_SEQUENCE_LENGTH 10 // 최종 지형 시퀀스 길이 - 지형 순서 배열 크기
extern const char* finalTerrainNames[]; // 최종 지형 이름 배열 - 각 지형의 한글 이름 저장
extern const int terrainSequence[]; // 지형 시퀀스 배열 - 게임 진행 순서대로 지형 번호 저장

#endif // STAGE_H 