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

// 요괴 성불 처리
void releaseYokai(int index);

// 파티가 가득 찼을 때의 처리
int handleFullParty(const Yokai* newYokai);

// 모든 요괴의 PP 초기화
void resetAllYokaiPP();

#endif // PARTY_H 