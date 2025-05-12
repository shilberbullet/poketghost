#include <stdio.h>
#include <string.h>
#include "party.h"
#include "yokai.h"
#include "move.h"

Yokai party[MAX_PARTY_SIZE];
int partyCount = 0;

void initParty() {
    partyCount = 0;
    // 도깨비 요괴를 파일에서 불러와 추가
    Yokai* dokkaebi = findYokaiByName("도깨비");
    if (dokkaebi) {
        party[0] = *dokkaebi;
        partyCount = 1;
    }
}

int addYokaiToParty(const Yokai* yokai) {
    if (partyCount >= MAX_PARTY_SIZE) return 0;
    party[partyCount++] = *yokai;
    return 1;
}

void printParty() {
    printf("\n=== 동료 요괴 목록 ===\n");
    for (int i = 0; i < partyCount; i++) {
        printf("[%d] %s (체력: %d, 공격력: %d, 방어력: %d, 상성: %s)\n", i+1, party[i].name, party[i].hp, party[i].attack, party[i].defense, typeNames[party[i].type]);
        printf("  기술: ");
        for (int j = 0; j < party[i].moveCount; j++) {
            printf("%s%s", party[i].moves[j].name, (j < party[i].moveCount-1) ? ", " : "");
        }
        printf("\n");
    }
    printf("\n");
} 