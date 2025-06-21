#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "dialogue.h"
#include "text.h"
#include "settings.h"
#include "logger.h"

// 전역 대화 시스템
static DialogueSystem gDialogueSystem = {0};

// 대화 시스템 초기화
void initDialogueSystem(void) {
    LOG_FUNCTION_EXECUTION("initDialogueSystem");
    
    gDialogueSystem.dialogues = NULL;
    gDialogueSystem.dialogueCount = 0;
    gDialogueSystem.currentDialogueId = -1;
    gDialogueSystem.isDialogueActive = 0;
}

// 대화 시스템 정리
void cleanupDialogueSystem(void) {
    LOG_FUNCTION_EXECUTION("cleanupDialogueSystem");
    
    if (gDialogueSystem.dialogues) {
        for (int i = 0; i < gDialogueSystem.dialogueCount; i++) {
            if (gDialogueSystem.dialogues[i].speaker) {
                free(gDialogueSystem.dialogues[i].speaker);
            }
            if (gDialogueSystem.dialogues[i].message) {
                free(gDialogueSystem.dialogues[i].message);
            }
        }
        free(gDialogueSystem.dialogues);
        gDialogueSystem.dialogues = NULL;
    }
    gDialogueSystem.dialogueCount = 0;
    gDialogueSystem.currentDialogueId = -1;
    gDialogueSystem.isDialogueActive = 0;
}

// 대화 타입별 색상 설정
const char* getDialogueColor(DialogueType type) {
    switch (type) {
        case DIALOGUE_NORMAL:
            return "\033[37m";  // 흰색
        case DIALOGUE_BATTLE:
            return "\033[31m";  // 빨간색
        case DIALOGUE_STORY:
            return "\033[33m";  // 노란색
        case DIALOGUE_TUTORIAL:
            return "\033[36m";  // 청록색
        case DIALOGUE_SYSTEM:
            return "\033[32m";  // 초록색
        default:
            return "\033[0m";   // 기본색
    }
}

// 대화 출력 (색상 포함)
void printDialogueMessage(const char* speaker, const char* message, DialogueType type) {
    LOG_FUNCTION_EXECUTION("printDialogueMessage");
    
    const char* color = getDialogueColor(type);
    
    // 대화 시작을 명확하게 표시
    printText("\n");
    printText("==================================================\n");
    
    if (speaker && strlen(speaker) > 0) {
        char buffer[512];
        sprintf(buffer, "%s%s\033[0m: %s%s\033[0m\n", color, speaker, color, message);
        printTextAndWait(buffer);
    } else {
        char buffer[512];
        sprintf(buffer, "%s%s\033[0m\n", color, message);
        printTextAndWait(buffer);
    }
    
    // 대화 끝을 명확하게 표시
    printText("==================================================\n");
}

// 대화 추가
int addDialogue(int id, DialogueType type, const char* speaker, const char* message, int nextId, int condition) {
    LOG_FUNCTION_EXECUTION("addDialogue");
    
    // 기존 대화 배열 확장
    Dialogue* newDialogues = realloc(gDialogueSystem.dialogues, (gDialogueSystem.dialogueCount + 1) * sizeof(Dialogue));
    if (!newDialogues) {
        return 0; // 메모리 할당 실패
    }
    gDialogueSystem.dialogues = newDialogues;
    
    // 새 대화 추가
    Dialogue* newDialogue = &gDialogueSystem.dialogues[gDialogueSystem.dialogueCount];
    newDialogue->id = id;
    newDialogue->type = type;
    newDialogue->nextDialogueId = nextId;
    newDialogue->condition = condition;
    
    // 화자 복사
    if (speaker) {
        newDialogue->speaker = strdup(speaker);
    } else {
        newDialogue->speaker = NULL;
    }
    
    // 메시지 복사
    if (message) {
        newDialogue->message = strdup(message);
    } else {
        newDialogue->message = NULL;
    }
    
    gDialogueSystem.dialogueCount++;
    return 1;
}

// 대화 찾기
Dialogue* findDialogue(int id) {
    for (int i = 0; i < gDialogueSystem.dialogueCount; i++) {
        if (gDialogueSystem.dialogues[i].id == id) {
            return &gDialogueSystem.dialogues[i];
        }
    }
    return NULL;
}

// 대화 조건 확인
int checkDialogueCondition(int condition) {
    // 조건이 0이면 항상 참
    if (condition == 0) {
        return 1;
    }
    
    // 여기에 다양한 조건들을 추가할 수 있습니다
    // 예: 특정 아이템 보유, 특정 요괴 보유, 특정 스테이지 등
    
    return 1; // 기본적으로 참 반환
}

// 대화 시작
void startDialogue(int dialogueId) {
    LOG_FUNCTION_EXECUTION("startDialogue");
    
    // 대화 시스템이 꺼져있으면 대화를 표시하지 않음
    if (!gameSettings.dialogueEnabled) {
        return;
    }
    
    Dialogue* dialogue = findDialogue(dialogueId);
    if (!dialogue) {
        return;
    }
    
    // 조건 확인
    if (!checkDialogueCondition(dialogue->condition)) {
        return;
    }
    
    gDialogueSystem.currentDialogueId = dialogueId;
    gDialogueSystem.isDialogueActive = 1;
    
    showDialogue();
}

// 대화 표시
void showDialogue(void) {
    LOG_FUNCTION_EXECUTION("showDialogue");
    
    if (!gDialogueSystem.isDialogueActive) {
        return;
    }
    
    Dialogue* dialogue = findDialogue(gDialogueSystem.currentDialogueId);
    if (!dialogue) {
        endDialogue();
        return;
    }
    
    // 대화 메시지 출력
    printDialogueMessage(dialogue->speaker, dialogue->message, dialogue->type);
    
    // 사용자가 엔터키를 누를 때까지 대기
    printText("계속하려면 엔터키를 누르세요...\n");
    getchar();
    
    // 다음 대화가 있으면 자동으로 진행
    if (dialogue->nextDialogueId != -1) {
        gDialogueSystem.currentDialogueId = dialogue->nextDialogueId;
        showDialogue();
    } else {
        endDialogue();
    }
}

// 대화 종료
void endDialogue(void) {
    LOG_FUNCTION_EXECUTION("endDialogue");
    
    gDialogueSystem.currentDialogueId = -1;
    gDialogueSystem.isDialogueActive = 0;
}

// 대화 진행 (다음 대화로)
void nextDialogue(void) {
    LOG_FUNCTION_EXECUTION("nextDialogue");
    
    if (!gDialogueSystem.isDialogueActive) {
        return;
    }
    
    Dialogue* dialogue = findDialogue(gDialogueSystem.currentDialogueId);
    if (!dialogue) {
        endDialogue();
        return;
    }
    
    if (dialogue->nextDialogueId != -1) {
        gDialogueSystem.currentDialogueId = dialogue->nextDialogueId;
        showDialogue();
    } else {
        endDialogue();
    }
}

// 대화 파일에서 로드
int loadDialoguesFromFile(const char* filename) {
    LOG_FUNCTION_EXECUTION("loadDialoguesFromFile");
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        return 0;
    }
    
    char line[512];
    int lineCount = 0;
    
    while (fgets(line, sizeof(line), file)) {
        lineCount++;
        
        // 주석이나 빈 줄 무시
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }
        
        // 줄바꿈 문자 제거
        line[strcspn(line, "\r\n")] = 0;
        
        // 대화 데이터 파싱 (ID|타입|화자|메시지|다음ID|조건)
        char* token = strtok(line, "|");
        if (!token) continue;
        
        int id = atoi(token);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        DialogueType type = (DialogueType)atoi(token);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        char* speaker = token;
        
        token = strtok(NULL, "|");
        if (!token) continue;
        char* message = token;
        
        token = strtok(NULL, "|");
        if (!token) continue;
        int nextId = atoi(token);
        
        token = strtok(NULL, "|");
        int condition = token ? atoi(token) : 0;
        
        // 대화 추가
        if (!addDialogue(id, type, speaker, message, nextId, condition)) {
            fclose(file);
            return 0;
        }
    }
    
    fclose(file);
    return 1;
} 