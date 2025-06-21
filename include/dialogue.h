#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 대화 타입 정의
typedef enum {
    DIALOGUE_NORMAL,      // 일반 대화
    DIALOGUE_BATTLE,      // 전투 대화
    DIALOGUE_STORY,       // 스토리 대화
    DIALOGUE_TUTORIAL,    // 튜토리얼 대화
    DIALOGUE_SYSTEM       // 시스템 메시지
} DialogueType;

// 대화 구조체
typedef struct {
    int id;                     // 대화 ID
    DialogueType type;          // 대화 타입
    char* speaker;              // 화자 (NULL이면 시스템 메시지)
    char* message;              // 대화 내용
    int nextDialogueId;         // 다음 대화 ID (-1이면 대화 종료)
    int condition;              // 조건 (0이면 항상 표시)
} Dialogue;

// 대화 시스템 구조체
typedef struct {
    Dialogue* dialogues;        // 대화 배열
    int dialogueCount;          // 대화 개수
    int currentDialogueId;      // 현재 대화 ID
    int isDialogueActive;       // 대화 활성화 상태
} DialogueSystem;

// 대화 시스템 초기화
void initDialogueSystem(void);

// 대화 시스템 정리
void cleanupDialogueSystem(void);

// 대화 시작
void startDialogue(int dialogueId);

// 대화 표시
void showDialogue(void);

// 대화 종료
void endDialogue(void);

// 대화 진행 (다음 대화로)
void nextDialogue(void);

// 대화 파일에서 로드
int loadDialoguesFromFile(const char* filename);

// 대화 추가
int addDialogue(int id, DialogueType type, const char* speaker, const char* message, int nextId, int condition);

// 대화 찾기
Dialogue* findDialogue(int id);

// 대화 조건 확인
int checkDialogueCondition(int condition);

// 대화 타입별 색상 설정
const char* getDialogueColor(DialogueType type);

// 대화 출력 (색상 포함)
void printDialogueMessage(const char* speaker, const char* message, DialogueType type);

#endif // DIALOGUE_H 