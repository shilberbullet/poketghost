// 표준 입출력 함수를 위한 헤더
#include <stdio.h>
// 메모리 할당, 랜덤 함수 등을 위한 헤더
#include <stdlib.h>
// 문자열 처리 함수를 위한 헤더
#include <string.h>
// 이동 관련 함수와 구조체 정의
#include "move.h"
// 텍스트 출력 관련 함수와 구조체 정의
#include "text.h"
// 로깅 기능을 위한 헤더
#include "logger.h"

// 외부에서 정의된 변수와 함수 선언
extern const char* typeNames[];           // 요괴 타입 이름 배열
extern YokaiType parseType(const char* typeStr);  // 문자열을 요괴 타입으로 변환하는 함수

// 기술 목록과 개수를 저장하는 전역 변수
Move moveList[MAX_MOVES_TOTAL];  // 기술 목록 배열
int moveListCount = 0;           // 현재 기술 수

// 현재 로드 중인 기술 등급을 추적하는 변수
static MoveGrade currentGrade = MOVE_BASIC;

// 기술 데이터를 파일에서 로드하는 함수
void loadMovesFromFile(const char* filename) {
    LOG_FUNCTION_EXECUTION("loadMovesFromFile");
    moveListCount = 0;  // 기술 목록 초기화
    FILE* file = fopen(filename, "r");
    if (!file) return;
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // 주석이나 빈 줄 처리
        if (line[0] == '#' || line[0] == '\n') {
            // 주석 라인에서 기술 등급 확인
            if (strstr(line, "초급 기술")) {
                currentGrade = MOVE_BASIC;
            } else if (strstr(line, "중급 기술")) {
                currentGrade = MOVE_MEDIUM;
            } else if (strstr(line, "고급 기술")) {
                currentGrade = MOVE_ADVANCED;
            }
            continue;
        }
        
        // CSV 형식 파싱
        char* name = strtok(line, ",");      // 기술 이름
        char* type = strtok(NULL, ",");      // 기술 타입
        char* power = strtok(NULL, ",");     // 기술 위력
        char* acc = strtok(NULL, ",");       // 기술 명중률
        char* pp = strtok(NULL, ",");        // 기술 PP
        char* desc = strtok(NULL, "\n");     // 기술 설명
        
        // 모든 필드가 존재하고 최대 기술 수를 초과하지 않는 경우
        if (name && type && power && acc && pp && desc && moveListCount < MAX_MOVES_TOTAL) {
            // 기술 정보 저장
            strncpy(moveList[moveListCount].name, name, YOKAI_NAME_MAX);  // 기술 이름 복사
            moveList[moveListCount].type = parseType(type);               // 기술 타입 변환
            moveList[moveListCount].power = atoi(power);                  // 기술 위력 저장
            moveList[moveListCount].accuracy = atoi(acc);                 // 기술 명중률 저장
            moveList[moveListCount].pp = atoi(pp);                        // 기술 PP 저장
            strncpy(moveList[moveListCount].description, desc, 255);      // 기술 설명 복사
            moveList[moveListCount].grade = currentGrade;                 // 기술 등급 저장
            moveListCount++;                                              // 기술 수 증가
        }
    }
    fclose(file);
}

// 이름으로 기술을 찾는 함수
Move* findMoveByName(const char* name) {
    LOG_FUNCTION_EXECUTION("findMoveByName");
    for (int i = 0; i < moveListCount; i++) {
        if (strcmp(moveList[i].name, name) == 0) return &moveList[i];
    }
    return NULL;
}

// 기술 정보를 출력하는 함수
void printMoveInfo(const Move* move) {
    LOG_FUNCTION_EXECUTION("printMoveInfo");
    printf("\n=== %s ===\n", move->name);                    // 기술 이름 출력
    printf("등급: %s\n", gradeToString(move->grade));        // 기술 등급 출력
    printf("공격력: %d\n", move->power);                     // 기술 위력 출력
    printf("명중률: %d%%\n", move->accuracy);                // 기술 명중률 출력
    printf("설명: %s\n", move->description);                 // 기술 설명 출력
} 