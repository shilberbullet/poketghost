#include <stdio.h>
#include <string.h>
#include "party.h"
#include "yokai.h"
#include "move.h"
#include "text.h"
#include "input.h"

#ifndef YOKAI_DESC_MAX
#define YOKAI_DESC_MAX 256
#endif

// assignRandomMoves 함수 선언
void assignRandomMoves(Yokai* y);

Yokai party[MAX_PARTY_SIZE];
int partyCount = 0;

void initParty() {
    partyCount = 0;
    // 도깨비 요괴를 파일에서 불러와 추가
    Yokai* dokkaebi = findYokaiByName("도깨비");
    if (dokkaebi) {
        memset(&party[0], 0, sizeof(Yokai)); // 구조체 전체를 0으로 초기화
        party[0] = *dokkaebi;  // 기본 정보 복사
        party[0].level = 1;    // 초기 레벨 1로 설정
        
        // 도감 설명 명시적 복사
        strncpy(party[0].desc, dokkaebi->desc, YOKAI_DESC_MAX - 1);
        party[0].desc[YOKAI_DESC_MAX - 1] = '\0';
        
        // learnableMoves 복사
        party[0].learnableMoveCount = dokkaebi->learnableMoveCount;
        for (int i = 0; i < dokkaebi->learnableMoveCount; i++) {
            party[0].learnableMoves[i] = dokkaebi->learnableMoves[i];
        }
        
        adjustStatsByLevel(&party[0]);  // 레벨에 따른 능력치 보정
        assignRandomMoves(&party[0]);   // 랜덤 기술 할당
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
    memset(&party[partyCount], 0, sizeof(Yokai));  // 구조체 전체를 0으로 초기화
    party[partyCount] = *yokai;  // 기본 정보 복사
    party[partyCount].level = 1;  // 새로 잡은 요괴도 레벨 1로 시작
    
    // 도감 설명 명시적 복사
    strncpy(party[partyCount].desc, yokai->desc, YOKAI_DESC_MAX - 1);
    party[partyCount].desc[YOKAI_DESC_MAX - 1] = '\0';
    
    // learnableMoves 복사
    party[partyCount].learnableMoveCount = yokai->learnableMoveCount;
    for (int i = 0; i < yokai->learnableMoveCount; i++) {
        party[partyCount].learnableMoves[i] = yokai->learnableMoves[i];
    }
    
    adjustStatsByLevel(&party[partyCount]);  // 레벨에 따른 능력치 보정
    assignRandomMoves(&party[partyCount]);   // 랜덤 기술 할당
    partyCount++;
    return 1;
}

void printParty() {
    printText("\n=== 동료 요괴 목록 ===\n");
    for (int i = 0; i < partyCount; i++) {
        char buffer[128];
        sprintf(buffer, "[%d] %s Lv.%d\n", i+1, party[i].name, party[i].level);
        printText(buffer);
    }
    printText("[0] 뒤로 가기\n");
    printText("\n선택하세요: ");
    
    int choice = getIntInput();
    if (choice == 0) {
        return;
    }
    
    if (choice > 0 && choice <= partyCount) {
        int idx = choice - 1;
        char buffer[2048];  // 버퍼 크기를 2048로 증가
        
        // 기본 정보 출력
        sprintf(buffer, "\n=== %s Lv.%d의 정보 ===\n", party[idx].name, party[idx].level);
        printText(buffer);
        sprintf(buffer, "체력: %d\n", party[idx].hp);
        printText(buffer);
        sprintf(buffer, "공격력: %d\n", party[idx].attack);
        printText(buffer);
        sprintf(buffer, "방어력: %d\n", party[idx].defense);
        printText(buffer);
        sprintf(buffer, "상성: %s\n", typeNames[party[idx].type]);
        printText(buffer);
        
        // 도감 설명을 별도로 출력
        printText("\n도감설명:\n");
        char* desc = party[idx].desc;
        while (*desc) {
            char temp[256];
            int i = 0;
            while (*desc && i < 255) {
                temp[i++] = *desc++;
            }
            temp[i] = '\0';
            printText(temp);
            printText("\n");
        }
        
        // 기술 목록 출력
        printText("\n기술 목록:\n");
        for (int j = 0; j < party[idx].moveCount; j++) {
            sprintf(buffer, "%d. %s (공격력: %d, 명중률: %d%%)\n", 
                j+1, party[idx].moves[j].name, 
                party[idx].moves[j].power,
                party[idx].moves[j].accuracy);
            printText(buffer);
            printText("   설명: ");
            char* moveDesc = party[idx].moves[j].description;
            while (*moveDesc) {
                char temp[256];
                int i = 0;
                while (*moveDesc && i < 255) {
                    temp[i++] = *moveDesc++;
                }
                temp[i] = '\0';
                printText(temp);
                printText("\n");
            }
        }
        printText("\n");
        printParty(); // 다시 목록으로 돌아가기
    } else {
        printTextAndWait("\n잘못된 선택입니다. 다시 시도하세요.");
        printParty();
    }
} 