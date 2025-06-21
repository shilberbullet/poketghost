/**
 * @file savefile.h
 * @brief 포켓요괴 게임의 저장 파일 관리 헤더 파일
 * @details 게임 저장, 로드, 파일 삭제 등의 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef SAVEFILE_H
#define SAVEFILE_H

#include "game.h" // 게임 관련 헤더
#include "region.h" // 지역 관련 헤더

/**
 * @brief 현재 게임 상태를 파일에 저장하는 함수
 */
void saveGame();

/**
 * @brief 저장된 게임 데이터를 파일에서 로드하는 함수
 * @return 로드 성공 여부
 */
int loadGameData();

/**
 * @brief 저장 파일을 삭제하는 함수
 */
void removeSaveFile();

/**
 * @brief 지역 데이터를 파일에 저장하는 함수
 * @param file 저장할 파일 포인터
 */
void saveRegionData(FILE* file);

/**
 * @brief 지역 데이터를 파일에서 로드하는 함수
 * @param file 로드할 파일 포인터
 */
void loadRegionData(FILE* file);

#endif // SAVEFILE_H 