#ifndef TYPES_H
#define TYPES_H

#define YOKAI_NAME_MAX 32

typedef struct {
    char name[YOKAI_NAME_MAX];
    int power;          // 공격력
    int accuracy;       // 명중률 (0-100)
    int pp;             // 최대 사용 가능 횟수 (PP)
    char description[256];  // 기술 설명
} Move;

typedef struct {
    Move move;
    int currentPP; // 현재 남은 PP
} MoveInstance;

#endif // TYPES_H 