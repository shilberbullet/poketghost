#ifndef PARTY_H
#define PARTY_H

#include "yokai.h"

#define MAX_PARTY_SIZE 5

// 동료 요괴 배열
extern Yokai party[MAX_PARTY_SIZE];
extern int partyCount;

// 동료 요괴 초기화
void initParty();

// 동료 요괴 추가
int addYokaiToParty(const Yokai* yokai);

// 동료 요괴 목록 출력
void printParty();

#endif // PARTY_H 