// 표준 입출력 함수를 위한 헤더
#include <stdio.h> // 표준 입출력 함수
// 메모리 할당, 랜덤 함수 등을 위한 헤더
#include <stdlib.h> // 표준 라이브러리 함수
// 문자열 처리 함수를 위한 헤더
#include <string.h> // 문자열 처리 함수
// 이동 관련 함수와 구조체 정의
#include "move.h" // 기술 관련 헤더
// 텍스트 출력 관련 함수와 구조체 정의
#include "text.h" // 텍스트 출력 헤더
// 로깅 기능을 위한 헤더
#include "logger.h" // 로거 헤더

// 외부에서 정의된 변수와 함수 선언
extern const char* typeNames[];           // 요괴 타입 이름 배열
extern YokaiType parseType(const char* typeStr);  // 문자열을 요괴 타입으로 변환하는 함수

// 기술 목록과 개수를 저장하는 전역 변수
Move moveList[MAX_MOVES_TOTAL];  // 기술 목록 배열
int moveListCount = 0;           // 현재 기술 수

// 현재 로드 중인 기술 등급을 추적하는 변수
static MoveGrade currentGrade = MOVE_BASIC; // 현재 기술 등급

// 기술 데이터를 파일에서 로드하는 함수
void loadMovesFromFile(const char* filename) { // 기술 파일 로드 함수 시작
    printf("[DEBUG] loadMovesFromFile 함수 호출됨: %s\n", filename); // 디버그 메시지 출력
    LOG_FUNCTION_EXECUTION("loadMovesFromFile"); // 함수 실행 로그 기록
    
    // 디버그 정보 추가
    char debugBuffer[256]; // 디버그 메시지 버퍼
    sprintf(debugBuffer, "[DEBUG] 기술 파일 로딩 시작: %s", filename); // 디버그 메시지 생성
    LOG_FUNCTION_EXECUTION(debugBuffer); // 디버그 로그 기록
    printf("%s\n", debugBuffer); // 디버그 메시지 출력
    
    moveListCount = 0;  // 기술 목록 초기화
    int basicCount = 0, mediumCount = 0, advancedCount = 0;  // 섹션별 기술 수 추적
    
    FILE* file = fopen(filename, "r"); // 파일 열기
    if (!file) { // 파일 열기 실패시
        sprintf(debugBuffer, "[DEBUG] 기술 파일을 열 수 없음: %s", filename); // 오류 메시지 생성
        LOG_FUNCTION_EXECUTION(debugBuffer); // 오류 로그 기록
        printf("%s\n", debugBuffer); // 오류 메시지 출력
        return; // 함수 종료
    }
    
    sprintf(debugBuffer, "[DEBUG] 기술 파일 열기 성공: %s", filename); // 성공 메시지 생성
    LOG_FUNCTION_EXECUTION(debugBuffer); // 성공 로그 기록
    printf("%s\n", debugBuffer); // 성공 메시지 출력
    
    char line[256]; // 라인 버퍼
    while (fgets(line, sizeof(line), file)) { // 파일의 각 라인을 읽기
        // 주석이나 빈 줄 처리
        if (line[0] == '#' || line[0] == '\n') { // 주석이나 빈 라인인 경우
            // 주석 라인에서 기술 등급 확인
            if (strstr(line, "초급 기술")) { // 초급 기술 섹션 확인
                currentGrade = MOVE_BASIC; // 초급 등급 설정
                printf("[DEBUG] 초급 기술 섹션 시작\n"); // 디버그 메시지 출력
            } else if (strstr(line, "중급 기술")) { // 중급 기술 섹션 확인
                currentGrade = MOVE_MEDIUM; // 중급 등급 설정
                printf("[DEBUG] 중급 기술 섹션 시작\n"); // 디버그 메시지 출력
            } else if (strstr(line, "고급 기술")) { // 고급 기술 섹션 확인
                currentGrade = MOVE_ADVANCED; // 고급 등급 설정
                printf("[DEBUG] 고급 기술 섹션 시작\n"); // 디버그 메시지 출력
            }
            continue; // 다음 라인으로
        }
        
        // CSV 형식 파싱
        char* name = strtok(line, ",");      // 기술 이름 파싱
        char* type = strtok(NULL, ",");      // 기술 타입 파싱
        char* power = strtok(NULL, ",");     // 기술 위력 파싱
        char* acc = strtok(NULL, ",");       // 기술 명중률 파싱
        char* pp = strtok(NULL, ",");        // 기술 PP 파싱
        char* desc = strtok(NULL, "\n");     // 기술 설명 파싱
        
        // 모든 필드가 존재하고 최대 기술 수를 초과하지 않는 경우
        if (name && type && power && acc && pp && desc && moveListCount < MAX_MOVES_TOTAL) { // 유효성 검사
            // 기술 정보 저장
            strncpy(moveList[moveListCount].name, name, YOKAI_NAME_MAX);  // 기술 이름 복사
            moveList[moveListCount].type = parseType(type);               // 기술 타입 변환
            moveList[moveListCount].power = atoi(power);                  // 기술 위력 저장
            moveList[moveListCount].accuracy = atoi(acc);                 // 기술 명중률 저장
            moveList[moveListCount].pp = atoi(pp);                        // 기술 PP 저장
            strncpy(moveList[moveListCount].description, desc, 255);      // 기술 설명 복사
            moveList[moveListCount].grade = currentGrade;                 // 기술 등급 저장
            
            // 섹션별 기술 수 추적
            switch (currentGrade) { // 현재 등급에 따라 카운터 증가
                case MOVE_BASIC: basicCount++; break; // 초급 기술 카운터 증가
                case MOVE_MEDIUM: mediumCount++; break; // 중급 기술 카운터 증가
                case MOVE_ADVANCED: advancedCount++; break; // 고급 기술 카운터 증가
            }
            
            // 디버그 정보 추가
            char debugBuffer[256]; // 디버그 메시지 버퍼
            sprintf(debugBuffer, "[DEBUG] 로드된 기술: %s, 공격력: %d, 등급: %d", 
                    moveList[moveListCount].name, moveList[moveListCount].power, moveList[moveListCount].grade); // 디버그 메시지 생성
            LOG_FUNCTION_EXECUTION(debugBuffer); // 디버그 로그 기록
            printf("%s\n", debugBuffer); // 디버그 메시지 출력
            
            moveListCount++;                                              // 기술 수 증가
        }
    }
    fclose(file); // 파일 닫기
    
    // 최종 로드된 기술 수 출력
    sprintf(debugBuffer, "[DEBUG] 총 로드된 기술 수: %d (초급: %d, 중급: %d, 고급: %d)", 
            moveListCount, basicCount, mediumCount, advancedCount); // 최종 통계 메시지 생성
    LOG_FUNCTION_EXECUTION(debugBuffer); // 최종 통계 로그 기록
    printf("%s\n", debugBuffer); // 최종 통계 메시지 출력
}

// 이름으로 기술을 찾는 함수
Move* findMoveByName(const char* name) { // 기술 이름으로 찾기 함수 시작
    LOG_FUNCTION_EXECUTION("findMoveByName"); // 함수 실행 로그 기록
    
    // 디버그 정보 추가
    char debugBuffer[256]; // 디버그 메시지 버퍼
    sprintf(debugBuffer, "[DEBUG] 찾는 기술: '%s'", name); // 검색 메시지 생성
    LOG_FUNCTION_EXECUTION(debugBuffer); // 검색 로그 기록
    
    for (int i = 0; i < moveListCount; i++) { // 모든 기술 검사
        if (strcmp(moveList[i].name, name) == 0) { // 이름이 일치하는 경우
            sprintf(debugBuffer, "[DEBUG] 기술을 찾았음: %s, 공격력: %d, 등급: %d", 
                    moveList[i].name, moveList[i].power, moveList[i].grade); // 찾음 메시지 생성
            LOG_FUNCTION_EXECUTION(debugBuffer); // 찾음 로그 기록
            return &moveList[i]; // 기술 포인터 반환
        }
    }
    
    sprintf(debugBuffer, "[DEBUG] 기술을 찾지 못함: '%s'", name); // 못찾음 메시지 생성
    LOG_FUNCTION_EXECUTION(debugBuffer); // 못찾음 로그 기록
    return NULL; // NULL 반환
}

// 기술 정보를 출력하는 함수
void printMoveInfo(const Move* move) { // 기술 정보 출력 함수 시작
    LOG_FUNCTION_EXECUTION("printMoveInfo"); // 함수 실행 로그 기록
    printf("\n=== %s ===\n", move->name);                    // 기술 이름 출력
    printf("등급: %s\n", gradeToString(move->grade));        // 기술 등급 출력
    printf("공격력: %d\n", move->power);                     // 기술 위력 출력
    printf("명중률: %d%%\n", move->accuracy);                // 기술 명중률 출력
    printf("설명: %s\n", move->description);                 // 기술 설명 출력
} 