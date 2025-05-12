#ifndef MOVE_H
#define MOVE_H

#include "types.h"

#define MAX_MOVES_TOTAL 32

// 기술 구조체 (yokai.h에 이미 정의됨)
// typedef struct { ... } Move;

extern Move moveList[MAX_MOVES_TOTAL];
extern int moveListCount;

// 기술 정보 파일에서 로드
void loadMovesFromFile(const char* filename);

// 이름으로 기술 찾기
Move* findMoveByName(const char* name);

// 기술 정보 출력 함수
void printMoveInfo(const Move* move);

#endif // MOVE_H 