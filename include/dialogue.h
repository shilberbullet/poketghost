#ifndef DIALOGUE_H           // 중복 포함 방지를 위한 전처리기 지시문 시작
#define DIALOGUE_H           // 중복 포함 방지를 위한 매크로 정의

#include <stdio.h>   // 표준 입출력 함수 (printf 등) 사용을 위한 헤더
#include <stdlib.h>  // 표준 라이브러리 함수 (malloc, free 등) 사용을 위한 헤더
#include <string.h>  // 문자열 처리 함수 (strcpy, strcmp 등) 사용을 위한 헤더

// 대화 타입 정의 (각 대화의 성격을 구분하는 열거형)
typedef enum {
    DIALOGUE_NORMAL,      // 일반 대화 (기본 텍스트)
    DIALOGUE_BATTLE,      // 전투 대화 (전투 중 발생하는 대사)
    DIALOGUE_STORY,       // 스토리 대화 (주요 스토리 진행 대사)
    DIALOGUE_TUTORIAL,    // 튜토리얼 대화 (게임 설명 및 안내)
    DIALOGUE_SYSTEM       // 시스템 메시지 (알림, 경고 등)
} DialogueType; // 대화의 종류를 나타내는 열거형 (int형으로 취급)

// 대화 구조체 (한 줄의 대화 정보를 저장)
typedef struct {
    int id;                     // 대화 ID (고유 식별자)
    DialogueType type;          // 대화 타입 (위의 열거형)
    char* speaker;              // 화자 (NULL이면 시스템 메시지)
    char* message;              // 대화 내용 (실제 출력될 텍스트)
    int nextDialogueId;         // 다음 대화 ID (-1이면 대화 종료)
    int condition;              // 조건 (0이면 항상 표시, 그 외는 특정 조건 필요)
} Dialogue; // 대화 한 줄의 정보를 담는 구조체

// 대화 시스템 구조체 (전체 대화 상태 및 배열 관리)
typedef struct {
    Dialogue* dialogues;        // 대화 배열 (동적 할당)
    int dialogueCount;          // 대화 개수 (배열의 크기)
    int currentDialogueId;      // 현재 대화 ID (진행 중인 대화)
    int isDialogueActive;       // 대화 활성화 상태 (1: 활성, 0: 비활성)
} DialogueSystem; // 전체 대화 시스템 상태를 관리하는 구조체

/** @brief 대화 시스템을 초기화하는 함수 */
void initDialogueSystem(void); // 대화 시스템의 내부 상태 및 리소스를 초기화하는 함수 선언

/** @brief 대화 시스템을 정리하는 함수 */
void cleanupDialogueSystem(void); // 대화 시스템에서 사용한 리소스를 해제하는 함수 선언

/** @brief 대화를 시작하는 함수 */
void startDialogue(int dialogueId); // 지정한 ID의 대화 시퀀스를 시작하는 함수 선언

/** @brief 현재 대화를 표시하는 함수 */
void showDialogue(void); // 현재 진행 중인 대화를 화면에 출력하는 함수 선언

/** @brief 대화를 종료하는 함수 */
void endDialogue(void); // 현재 진행 중인 대화를 종료하는 함수 선언

/** @brief 다음 대화로 진행하는 함수 */
void nextDialogue(void); // 다음 대화로 수동 진행하는 함수 선언

/**
 * @brief 대화 파일에서 대화 데이터를 로드하는 함수
 * @param filename 로드할 파일명 (텍스트 파일 경로)
 * @return 성공 여부 (1: 성공, 0: 실패)
 */
int loadDialoguesFromFile(const char* filename); // 파일에서 대화 데이터를 읽어와 시스템에 등록하는 함수 선언

/**
 * @brief 대화 데이터를 추가하는 함수
 * @param id 대화 ID
 * @param type 대화 타입
 * @param speaker 화자
 * @param message 대화 내용
 * @param nextId 다음 대화 ID
 * @param condition 표시 조건
 * @return 성공 여부 (1: 성공, 0: 실패)
 */
int addDialogue(int id, DialogueType type, const char* speaker, const char* message, int nextId, int condition); // 새로운 대화 데이터를 시스템에 추가하는 함수 선언

/**
 * @brief ID로 대화 데이터를 찾는 함수
 * @param id 찾을 대화 ID
 * @return 대화 구조체 포인터 (NULL이면 없음)
 */
Dialogue* findDialogue(int id); // 지정한 ID의 대화 데이터를 검색하는 함수 선언

/**
 * @brief 대화 조건을 확인하는 함수
 * @param condition 조건 값 (0: 항상 참, 그 외: 특정 조건)
 * @return 조건 충족 여부 (1: 충족, 0: 불충족)
 */
int checkDialogueCondition(int condition); // 대화 표시 조건을 검사하는 함수 선언

/**
 * @brief 대화 타입별 색상 문자열 반환 함수
 * @param type 대화 타입 (열거형)
 * @return 색상 문자열 (ANSI 코드)
 */
const char* getDialogueColor(DialogueType type); // 대화 타입에 따라 출력 색상을 반환하는 함수 선언

/**
 * @brief 대화 메시지를 색상과 함께 출력하는 함수
 * @param speaker 화자 (NULL이면 시스템 메시지)
 * @param message 메시지 (출력할 텍스트)
 * @param type 대화 타입 (열거형)
 */
void printDialogueMessage(const char* speaker, const char* message, DialogueType type); // 대화 메시지를 색상과 함께 출력하는 함수 선언

#endif // DIALOGUE_H           // 중복 포함 방지를 위한 전처리기 지시문 종료 