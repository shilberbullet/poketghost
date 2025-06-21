/**
 * @file boss_stage.h
 * @brief 포켓요괴 게임의 보스 스테이지 헤더 파일
 * @details 보스 스테이지 초기화, 적 생성, 클리어 처리 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef BOSS_STAGE_H
#define BOSS_STAGE_H

#include "stage_types.h" // 스테이지 타입 정의

/**
 * @brief 보스 스테이지를 초기화하는 함수
 * @param stage 초기화할 스테이지 정보
 * @param stageNumber 스테이지 번호
 */
void initBossStage(StageInfo* stage, int stageNumber);

/**
 * @brief 보스 스테이지에 적을 생성하는 함수
 * @param stage 적을 생성할 스테이지
 */
void generateBossStageEnemies(StageInfo* stage);

/**
 * @brief 보스 스테이지 클리어를 처리하는 함수
 * @return 1 = finalstage 진입 조건 충족, 0 = 일반 보스 클리어
 */
int handleBossStageClear(void);

#endif // BOSS_STAGE_H 