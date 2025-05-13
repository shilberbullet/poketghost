#include <stdio.h>
#include <string.h>
#include "party.h"
#include "yokai.h"
#include "move.h"
#include "text.h"
#include "input.h"

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

// 요괴 성불 처리 함수
void releaseYokai(int index) {
    if (index < 0 || index >= partyCount) return;
    
    // 선택된 요괴를 배열에서 제거하고 나머지 요괴들을 앞으로 이동
    for (int i = index; i < partyCount - 1; i++) {
        party[i] = party[i + 1];
    }
    partyCount--;
}

// 새로운 요괴를 잡았을 때 파티가 가득 찼을 경우의 처리
int handleFullParty(const Yokai* newYokai) {
    if (partyCount < MAX_PARTY_SIZE) {
        return addYokaiToParty(newYokai);
    }

    printText("\n동료 요괴가 가득 찼습니다!\n");
    printText("1. 잡은 요괴를 성불시킨다\n");
    printText("2. 동료 요괴를 성불시킨다\n");
    printText("선택하세요 (1-2): ");

    int choice = getIntInput();
    if (choice == 1) {
        printTextAndWait("\n잡은 요괴를 성불시켰습니다.");
        return 0;
    } else if (choice == 2) {
        while (1) {
            printText("\n성불시킬 동료 요괴를 선택하세요:\n");
            for (int i = 0; i < partyCount; i++) {
                char buffer[128];
                sprintf(buffer, "%d. %s (체력: %d, 공격력: %d, 방어력: %d)\n", 
                    i+1, party[i].name, party[i].hp, party[i].attack, party[i].defense);
                printText(buffer);
            }
            printText("0. 뒤로 돌아간다\n");
            printText("선택하세요: ");

            int yokaiChoice = getIntInput();
            if (yokaiChoice == 0) {
                return handleFullParty(newYokai); // 다시 선택 메뉴로
            }
            yokaiChoice--; // 0-based index로 변환

            if (yokaiChoice >= 0 && yokaiChoice < partyCount) {
                char buffer[128];
                sprintf(buffer, "\n%s를 성불시켰습니다.", party[yokaiChoice].name);
                printTextAndWait(buffer);
                
                // 선택된 요괴를 성불시키고 새로운 요괴 추가
                releaseYokai(yokaiChoice);
                return addYokaiToParty(newYokai);
            } else {
                printTextAndWait("\n잘못된 선택입니다. 다시 시도하세요.");
            }
        }
    }
    return 0;
}

int addYokaiToParty(const Yokai* yokai) {
    if (partyCount >= MAX_PARTY_SIZE) {
        return handleFullParty(yokai);
    }
    party[partyCount++] = *yokai;
    return 1;
}

void printParty() {
    printf("\n=== 동료 요괴 목록 ===\n");
    for (int i = 0; i < partyCount; i++) {
        printf("[%d] %s (체력: %d, 공격력: %d, 방어력: %d, 상성: %s)\n", 
            i+1, party[i].name, party[i].hp, party[i].attack, party[i].defense, 
            typeNames[party[i].type]);
        printf("  기술: ");
        for (int j = 0; j < party[i].moveCount; j++) {
            printf("%s%s", party[i].moves[j].name, 
                (j < party[i].moveCount-1) ? ", " : "");
        }
        printf("\n");
    }
    printf("\n");
} 