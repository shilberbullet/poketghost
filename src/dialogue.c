#include <stdio.h>                    // 표준 입출력 함수를 사용하기 위한 헤더 파일
#include <stdlib.h>                   // 메모리 관리 함수들을 사용하기 위한 헤더 파일
#include <string.h>                   // 문자열 처리 함수들을 사용하기 위한 헤더 파일
#include <windows.h>                  // Windows API 함수를 사용하기 위한 헤더 파일
#include "dialogue.h"                 // 대화 시스템 관련 함수 선언이 포함된 헤더 파일
#include "text.h"                     // 텍스트 출력 관련 함수들이 포함된 헤더 파일
#include "settings.h"                 // 게임 설정 관련 함수들이 포함된 헤더 파일
#include "input.h"                    // 사용자 입력 처리 함수들이 포함된 헤더 파일
#include "logger.h"                   // 로깅 시스템 관련 함수들이 포함된 헤더 파일

// 전역 대화 시스템 구조체 (모든 대화 데이터와 상태를 관리)
static DialogueSystem gDialogueSystem = {0};  // 모든 멤버를 0으로 초기화

// 대화 시스템 초기화 함수 (시스템 시작 시 호출됨)
void initDialogueSystem(void) {
    LOG_FUNCTION_EXECUTION("initDialogueSystem");    // 함수 실행을 로그에 기록
    
    gDialogueSystem.dialogues = NULL;                // 대화 배열 포인터를 NULL로 초기화
    gDialogueSystem.dialogueCount = 0;               // 대화 개수를 0으로 초기화
    gDialogueSystem.currentDialogueId = -1;          // 현재 대화 ID를 -1로 초기화 (대화 없음)
    gDialogueSystem.isDialogueActive = 0;            // 대화 활성화 상태를 0으로 초기화 (비활성화)
}

// 대화 시스템 정리 함수 (시스템 종료 시 호출됨, 메모리 해제)
void cleanupDialogueSystem(void) {
    LOG_FUNCTION_EXECUTION("cleanupDialogueSystem"); // 함수 실행을 로그에 기록
    
    if (gDialogueSystem.dialogues) {                 // 대화 배열이 존재하는 경우
        for (int i = 0; i < gDialogueSystem.dialogueCount; i++) { // 모든 대화에 대해 반복
            if (gDialogueSystem.dialogues[i].speaker) {           // 화자 문자열이 존재하면
                free(gDialogueSystem.dialogues[i].speaker);       // 화자 문자열 메모리 해제
            }
            if (gDialogueSystem.dialogues[i].message) {           // 메시지 문자열이 존재하면
                free(gDialogueSystem.dialogues[i].message);       // 메시지 문자열 메모리 해제
            }
        }
        free(gDialogueSystem.dialogues);             // 대화 배열 자체의 메모리 해제
        gDialogueSystem.dialogues = NULL;            // 포인터를 NULL로 설정
    }
    gDialogueSystem.dialogueCount = 0;               // 대화 개수를 0으로 초기화
    gDialogueSystem.currentDialogueId = -1;          // 현재 대화 ID를 -1로 초기화
    gDialogueSystem.isDialogueActive = 0;            // 대화 활성화 상태를 0으로 초기화
}

// 대화 타입별 색상 반환 함수 (대화 타입에 따라 다른 색상 코드 반환)
const char* getDialogueColor(DialogueType type) {
    switch (type) {                                   // 대화 타입에 따라 분기
        case DIALOGUE_NORMAL:                         // 일반 대화인 경우
            return "\033[37m";                        // 흰색 색상 코드 반환
        case DIALOGUE_BATTLE:                         // 전투 대화인 경우
            return "\033[31m";                        // 빨간색 색상 코드 반환
        case DIALOGUE_STORY:                          // 스토리 대화인 경우
            return "\033[33m";                        // 노란색 색상 코드 반환
        case DIALOGUE_TUTORIAL:                       // 튜토리얼 대화인 경우
            return "\033[36m";                        // 청록색 색상 코드 반환
        case DIALOGUE_SYSTEM:                         // 시스템 대화인 경우
            return "\033[32m";                        // 초록색 색상 코드 반환
        default:                                      // 기타 타입인 경우
            return "\033[0m";                         // 기본색(색상 리셋) 코드 반환
    }
}

// 대화 메시지 출력 함수 (색상 포함하여 대화를 화면에 표시)
void printDialogueMessage(const char* speaker, const char* message, DialogueType type) {
    LOG_FUNCTION_EXECUTION("printDialogueMessage");   // 함수 실행을 로그에 기록
    
    const char* color = getDialogueColor(type);       // 대화 타입에 맞는 색상 코드 가져오기
    
    // 대화 시작을 명확하게 표시
    printText("\n");                                  // 줄바꿈으로 대화 시작 표시
    if (speaker && strlen(speaker) > 0) {             // 화자가 존재하고 길이가 0보다 큰 경우
        char buffer[512];                             // 메시지 출력용 버퍼
        sprintf(buffer, "%s%s\033[0m: %s%s\033[0m\n", color, speaker, color, message); // 화자와 메시지를 포함한 대화 형식 생성
        printTextAndWait(buffer);                     // 대화 메시지 출력 및 대기
        clearInputBuffer();                           // 입력 버퍼 정리

    } else {                                          // 화자가 없는 경우 (내레이션 등)
        char buffer[512];                             // 메시지 출력용 버퍼
        sprintf(buffer, "%s%s\033[0m\n", color, message); // 메시지만 포함한 형식 생성
        printTextAndWait(buffer);                     // 메시지 출력 및 대기
    }
    
    // 대화 끝을 명확하게 표시 (현재는 추가 표시 없음)
}

// 대화 추가 함수 (새로운 대화를 시스템에 등록)
int addDialogue(int id, DialogueType type, const char* speaker, const char* message, int nextId, int condition) { // 대화를 시스템에 추가하는 함수
    LOG_FUNCTION_EXECUTION("addDialogue");            // 함수 실행 로그 기록
    Dialogue* newDialogues = realloc(gDialogueSystem.dialogues, (gDialogueSystem.dialogueCount + 1) * sizeof(Dialogue)); // 기존 대화 배열을 하나 더 늘려서 재할당
    if (!newDialogues) {                              // 메모리 할당 실패 시
        return 0;                                     // 실패를 나타내는 0 반환
    }
    gDialogueSystem.dialogues = newDialogues;         // 새로 할당된 배열로 교체
    Dialogue* newDialogue = &gDialogueSystem.dialogues[gDialogueSystem.dialogueCount]; // 새 대화의 포인터 가져오기
    newDialogue->id = id;                             // 대화 ID 저장
    newDialogue->type = type;                         // 대화 타입 저장
    newDialogue->nextDialogueId = nextId;             // 다음 대화 ID 저장
    newDialogue->condition = condition;               // 조건 저장
    if (speaker) {                                    // 화자 문자열이 있으면
        newDialogue->speaker = strdup(speaker);       // 화자 문자열 복사 (메모리 할당)
    } else {                                          // 화자 문자열이 없으면
        newDialogue->speaker = NULL;                  // NULL로 설정
    }
    if (message) {                                    // 메시지 문자열이 있으면
        newDialogue->message = strdup(message);       // 메시지 문자열 복사 (메모리 할당)
    } else {                                          // 메시지 문자열이 없으면
        newDialogue->message = NULL;                  // NULL로 설정
    }
    gDialogueSystem.dialogueCount++;                  // 대화 개수 증가
    return 1;                                         // 성공을 나타내는 1 반환
}

// 대화 찾기 함수 (ID로 특정 대화를 검색)
Dialogue* findDialogue(int id) {                      // ID로 대화를 찾는 함수
    for (int i = 0; i < gDialogueSystem.dialogueCount; i++) { // 모든 대화를 순회
        if (gDialogueSystem.dialogues[i].id == id) {  // ID가 일치하면
            return &gDialogueSystem.dialogues[i];     // 해당 대화의 포인터 반환
        }
    }
    return NULL;                                      // 찾지 못하면 NULL 반환
}

// 대화 조건 확인 함수 (대화 표시 여부를 결정하는 조건 검사)
int checkDialogueCondition(int condition) {           // 대화 조건을 확인하는 함수
    if (condition == 0) {                             // 조건이 0이면 항상 참
        return 1;                                     // 참 반환
    }
    // 여기에 다양한 조건들을 추가할 수 있습니다
    // 예: 특정 아이템 보유, 특정 요괴 보유, 특정 스테이지 등
    return 1;                                         // 기본적으로 참 반환
}

// 대화 시작 함수 (특정 ID의 대화 시퀀스를 시작)
void startDialogue(int dialogueId) {                  // 특정 ID의 대화를 시작하는 함수
    LOG_FUNCTION_EXECUTION("startDialogue");          // 함수 실행 로그 기록
    if (!gameSettings.dialogueEnabled) {              // 대화 시스템이 꺼져있으면 대화를 표시하지 않음
        return;                                       // 함수 종료
    }
    Dialogue* dialogue = findDialogue(dialogueId);    // 대화 찾기
    if (!dialogue) {                                  // 대화를 찾지 못하면
        return;                                       // 함수 종료
    }
    if (!checkDialogueCondition(dialogue->condition)) { // 조건 확인
        return;                                       // 조건을 만족하지 않으면 함수 종료
    }
    gDialogueSystem.currentDialogueId = dialogueId;   // 현재 대화 ID 설정
    gDialogueSystem.isDialogueActive = 1;             // 대화 활성화
    showDialogue();                                   // 대화 표시 함수 호출
}

// 대화 표시 함수 (현재 대화를 화면에 표시하고 다음 대화로 진행)
void showDialogue(void) {                             // 현재 대화를 표시하는 함수
    LOG_FUNCTION_EXECUTION("showDialogue");           // 함수 실행 로그 기록
    if (!gDialogueSystem.isDialogueActive) {          // 대화가 비활성화되어 있으면
        return;                                       // 함수 종료
    }
    Dialogue* dialogue = findDialogue(gDialogueSystem.currentDialogueId); // 현재 대화 찾기
    if (!dialogue) {                                  // 대화를 찾지 못하면
        endDialogue();                                // 대화 종료 함수 호출
        return;                                       // 함수 종료
    }
    printDialogueMessage(dialogue->speaker, dialogue->message, dialogue->type); // 대화 메시지 출력
    fastSleep(500);                                   // 잠시 대기 후 자동으로 다음 대화로 진행
    if (dialogue->nextDialogueId != -1) {             // 다음 대화가 있으면 자동으로 진행
        gDialogueSystem.currentDialogueId = dialogue->nextDialogueId; // 다음 대화로 이동
        showDialogue();                               // 재귀 호출로 다음 대화 표시
    } else {                                          // 다음 대화가 없으면
        endDialogue();                                // 대화 종료 함수 호출
    }
}

// 대화 종료 함수 (현재 진행 중인 대화를 종료)
void endDialogue(void) {                              // 대화를 종료하는 함수
    LOG_FUNCTION_EXECUTION("endDialogue");            // 함수 실행 로그 기록
    gDialogueSystem.currentDialogueId = -1;           // 현재 대화 ID를 -1로 초기화
    gDialogueSystem.isDialogueActive = 0;             // 대화 비활성화
}

// 대화 진행 함수 (다음 대화로 수동 진행)
void nextDialogue(void) {                             // 다음 대화로 진행하는 함수
    LOG_FUNCTION_EXECUTION("nextDialogue");           // 함수 실행 로그 기록
    if (!gDialogueSystem.isDialogueActive) {          // 대화가 비활성화되어 있으면
        return;                                       // 함수 종료
    }
    Dialogue* dialogue = findDialogue(gDialogueSystem.currentDialogueId); // 현재 대화 찾기
    if (!dialogue) {                                  // 대화를 찾지 못하면
        endDialogue();                                // 대화 종료 함수 호출
        return;                                       // 함수 종료
    }
    if (dialogue->nextDialogueId != -1) {             // 다음 대화가 있으면
        gDialogueSystem.currentDialogueId = dialogue->nextDialogueId; // 다음 대화로 이동
        showDialogue();                               // 재귀 호출로 다음 대화 표시
    } else {                                          // 다음 대화가 없으면
        endDialogue();                                // 대화 종료 함수 호출
    }
}

// 대화 파일에서 로드 함수 (텍스트 파일에서 대화 데이터를 읽어와 시스템에 등록)
int loadDialoguesFromFile(const char* filename) {     // 파일에서 대화 데이터를 로드하는 함수
    LOG_FUNCTION_EXECUTION("loadDialoguesFromFile");  // 함수 실행 로그 기록
    FILE* file = fopen(filename, "r");                // 파일을 읽기 모드로 열기
    if (!file) {                                      // 파일 열기 실패 시
        return 0;                                     // 실패를 나타내는 0 반환
    }
    char line[512];                                   // 한 줄을 저장할 버퍼
    int lineCount = 0;                                // 줄 수 카운트
    while (fgets(line, sizeof(line), file)) {         // 파일 끝까지 한 줄씩 읽기
        lineCount++;                                  // 줄 수 증가
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') { // 주석이나 빈 줄 무시
            continue;                                 // 다음 줄로 건너뛰기
        }
        line[strcspn(line, "\r\n")] = 0;             // 줄바꿈 문자 제거
        char* token = strtok(line, "|");             // 대화 데이터 파싱 (ID|타입|화자|메시지|다음ID|조건)
        if (!token) continue;                        // 토큰이 없으면 다음 줄로
        int id = atoi(token);                        // ID를 정수로 변환
        token = strtok(NULL, "|");                   // 다음 토큰 파싱
        if (!token) continue;                        // 토큰이 없으면 다음 줄로
        DialogueType type = (DialogueType)atoi(token); // 타입을 정수로 변환
        token = strtok(NULL, "|");                   // 다음 토큰 파싱
        if (!token) continue;                        // 토큰이 없으면 다음 줄로
        char* speaker = token;                       // 화자 문자열 저장
        token = strtok(NULL, "|");                   // 다음 토큰 파싱
        if (!token) continue;                        // 토큰이 없으면 다음 줄로
        char* message = token;                       // 메시지 문자열 저장
        token = strtok(NULL, "|");                   // 다음 토큰 파싱
        if (!token) continue;                        // 토큰이 없으면 다음 줄로
        int nextId = atoi(token);                    // 다음 ID를 정수로 변환
        token = strtok(NULL, "|");                   // 다음 토큰 파싱
        int condition = token ? atoi(token) : 0;     // 조건을 정수로 변환 (없으면 0)
        if (!addDialogue(id, type, speaker, message, nextId, condition)) { // 대화 추가 실패 시
            fclose(file);                            // 파일 닫기
            return 0;                                // 실패 반환
        }
    }
    fclose(file);                                    // 파일 닫기
    return 1;                                        // 성공 반환
} 