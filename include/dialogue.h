/**
 * @file dialogue.h
 * @brief 포켓요괴 게임의 대화 시스템 헤더 파일
 * @details 대화 구조체, 대화 시스템, 대화 관련 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <stdio.h> // 표준 입출력 함수
#include <stdlib.h> // 표준 라이브러리 함수
#include <string.h> // 문자열 처리 함수

// 대화 타입 정의
typedef enum {
    DIALOGUE_NORMAL,      // 일반 대화
    DIALOGUE_BATTLE,      // 전투 대화
    DIALOGUE_STORY,       // 스토리 대화
    DIALOGUE_TUTORIAL,    // 튜토리얼 대화
    DIALOGUE_SYSTEM       // 시스템 메시지
} DialogueType; // 대화의 종류를 나타내는 열거형

// 대화 구조체
typedef struct {
    int id;                     // 대화 ID
    DialogueType type;          // 대화 타입
    char* speaker;              // 화자 (NULL이면 시스템 메시지)
    char* message;              // 대화 내용
    int nextDialogueId;         // 다음 대화 ID (-1이면 대화 종료)
    int condition;              // 조건 (0이면 항상 표시)
} Dialogue; // 대화 한 줄의 정보를 담는 구조체

// 대화 시스템 구조체
typedef struct {
    Dialogue* dialogues;        // 대화 배열
    int dialogueCount;          // 대화 개수
    int currentDialogueId;      // 현재 대화 ID
    int isDialogueActive;       // 대화 활성화 상태
} DialogueSystem; // 전체 대화 시스템 상태를 관리하는 구조체

/** @brief 대화 시스템을 초기화하는 함수 */
void initDialogueSystem(void);

/** @brief 대화 시스템을 정리하는 함수 */
void cleanupDialogueSystem(void);

/** @brief 대화를 시작하는 함수 */
void startDialogue(int dialogueId);

/** @brief 현재 대화를 표시하는 함수 */
void showDialogue(void);

/** @brief 대화를 종료하는 함수 */
void endDialogue(void);

/** @brief 다음 대화로 진행하는 함수 */
void nextDialogue(void);

/**
 * @brief 대화 파일에서 대화 데이터를 로드하는 함수
 * @param filename 로드할 파일명
 * @return 성공 여부
 */
int loadDialoguesFromFile(const char* filename);

/**
 * @brief 대화 데이터를 추가하는 함수
 * @param id 대화 ID
 * @param type 대화 타입
 * @param speaker 화자
 * @param message 대화 내용
 * @param nextId 다음 대화 ID
 * @param condition 표시 조건
 * @return 성공 여부
 */
int addDialogue(int id, DialogueType type, const char* speaker, const char* message, int nextId, int condition);

/**
 * @brief ID로 대화 데이터를 찾는 함수
 * @param id 찾을 대화 ID
 * @return 대화 구조체 포인터
 */
Dialogue* findDialogue(int id);

/**
 * @brief 대화 조건을 확인하는 함수
 * @param condition 조건 값
 * @return 조건 충족 여부
 */
int checkDialogueCondition(int condition);

/**
 * @brief 대화 타입별 색상 문자열 반환 함수
 * @param type 대화 타입
 * @return 색상 문자열
 */
const char* getDialogueColor(DialogueType type);

/**
 * @brief 대화 메시지를 색상과 함께 출력하는 함수
 * @param speaker 화자
 * @param message 메시지
 * @param type 대화 타입
 */
void printDialogueMessage(const char* speaker, const char* message, DialogueType type);

#endif // DIALOGUE_H 