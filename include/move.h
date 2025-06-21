/**
 * @file move.h
 * @brief 포켓요괴 게임의 기술(이동) 시스템 헤더 파일
 * @details 기술 데이터 로드, 검색, 출력 등의 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef MOVE_H
#define MOVE_H

#include "types.h" // 기본 타입 정의

#define MAX_MOVES_TOTAL 256 // 전체 기술의 최대 개수

// 기술 구조체 (yokai.h에 이미 정의됨)
// typedef struct { ... } Move;

extern Move moveList[MAX_MOVES_TOTAL]; // 모든 기술 목록
extern int moveListCount; // 기술 목록 개수

/**
 * @brief 기술 정보를 파일에서 로드하는 함수
 * @param filename 로드할 파일명
 */
void loadMovesFromFile(const char* filename);

/**
 * @brief 이름으로 기술을 찾는 함수
 * @param name 찾을 기술 이름
 * @return 찾은 기술 포인터, 없으면 NULL
 */
Move* findMoveByName(const char* name);

/**
 * @brief 기술 정보를 출력하는 함수
 * @param move 출력할 기술
 */
void printMoveInfo(const Move* move);

#endif // MOVE_H 