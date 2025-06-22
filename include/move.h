/**
 * @file move.h
 * @brief 포켓요괴 게임의 기술(이동) 시스템 헤더 파일
 * @details 기술 데이터 로드, 검색, 출력 등의 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef MOVE_H           // 중복 포함 방지를 위한 전처리기 지시문 시작
#define MOVE_H           // 중복 포함 방지를 위한 매크로 정의

#include "types.h"       // 기본 타입 정의 (Move 구조체 등이 정의됨)

#define MAX_MOVES_TOTAL 256 // 전체 기술의 최대 개수 (게임 내 모든 기술의 총 개수)

// 기술 구조체 (yokai.h에 이미 정의됨)
// typedef struct { ... } Move; // Move 구조체는 types.h에서 정의됨

extern Move moveList[MAX_MOVES_TOTAL]; // 모든 기술 목록 (최대 MAX_MOVES_TOTAL개)
extern int moveListCount; // 기술 목록 개수 (현재 등록된 기술의 수)

/**
 * @brief 기술 정보를 파일에서 로드하는 함수
 * @param filename 로드할 파일명 (텍스트 파일 경로)
 */
void loadMovesFromFile(const char* filename); // 파일에서 기술 데이터를 읽어오는 함수 선언

/**
 * @brief 이름으로 기술을 찾는 함수
 * @param name 찾을 기술 이름 (문자열)
 * @return 찾은 기술 포인터, 없으면 NULL
 */
Move* findMoveByName(const char* name); // 이름으로 기술을 검색하는 함수 선언

/**
 * @brief 기술 정보를 출력하는 함수
 * @param move 출력할 기술 (포인터)
 */
void printMoveInfo(const Move* move); // 기술의 상세 정보를 출력하는 함수 선언

#endif // MOVE_H           // 중복 포함 방지를 위한 전처리기 지시문 종료 