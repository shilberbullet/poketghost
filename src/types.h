#ifndef TYPES_H
#define TYPES_H

#define YOKAI_NAME_MAX 32

typedef struct {
    char name[YOKAI_NAME_MAX];
    int power;          // 공격력
    int accuracy;       // 명중률 (0-100)
    char description[256];  // 기술 설명
} Move;

#endif // TYPES_H 