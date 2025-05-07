#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "game_db.h"

#define MAX_MONSTERS 5
#define MONSTER_SKILLS 4  // 몬스터당 기술 수

// 요괴 정보를 저장하는 구조체
typedef struct {
    char name[50];
    int attack;
    int defense;
    int hp;
    char skills[MONSTER_SKILLS][50];
} Monster;

// 기술 정보를 저장하는 구조체
typedef struct {
    char name[50];
    int power;
    int accuracy;
    char description[100];
} Skill;

// 전역 변수
Monster monsters[MAX_MONSTERS];
Skill skills[MAX_MONSTERS];
int monsterCount = 0;
int skillCount = 0;
Monster* currentMonster = NULL;

// 외부 함수 선언
extern void printCharByChar(const char* str);
extern GameSpeed gameSpeeds[];
extern int currentSpeedIndex;
extern void startCompanionBattle();  // 동료 요괴 전투 시스템 함수 선언

// 요괴 정보 파일에서 읽어오기
void loadMonsters() {
    FILE* file = fopen("monsters.txt", "r");
    if (file == NULL) {
        printf("요괴 정보 파일을 열 수 없습니다.\n");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file) && monsterCount < MAX_MONSTERS) {
        char* token = strtok(line, ",");
        strcpy(monsters[monsterCount].name, token);
        
        token = strtok(NULL, ",");
        monsters[monsterCount].attack = atoi(token);
        
        token = strtok(NULL, ",");
        monsters[monsterCount].defense = atoi(token);
        
        token = strtok(NULL, ",");
        monsters[monsterCount].hp = atoi(token);
        
        for (int i = 0; i < MONSTER_SKILLS; i++) {
            token = strtok(NULL, ",");
            if (token != NULL) {
                token[strcspn(token, "\n")] = 0;  // 개행문자 제거
                strcpy(monsters[monsterCount].skills[i], token);
            }
        }
        monsterCount++;
    }
    fclose(file);
}

// 기술 정보 파일에서 읽어오기
void loadSkills() {
    FILE* file = fopen("skills.txt", "r");
    if (file == NULL) {
        printf("기술 정보 파일을 열 수 없습니다.\n");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file) && skillCount < MAX_MONSTERS) {
        char* token = strtok(line, ",");
        strcpy(skills[skillCount].name, token);
        
        token = strtok(NULL, ",");
        skills[skillCount].power = atoi(token);
        
        token = strtok(NULL, ",");
        skills[skillCount].accuracy = atoi(token);
        
        token = strtok(NULL, ",");
        if (token != NULL) {
            token[strcspn(token, "\n")] = 0;  // 개행문자 제거
            strcpy(skills[skillCount].description, token);
        }
        skillCount++;
    }
    fclose(file);
}

// 랜덤 요괴 선택
Monster* getRandomMonster() {
    return &monsters[rand() % monsterCount];
}

// 기술 정보 찾기
Skill* findSkill(const char* skillName) {
    for (int i = 0; i < skillCount; i++) {
        if (strcmp(skills[i].name, skillName) == 0) {
            return &skills[i];
        }
    }
    return NULL;
}

// 전투 시스템
void startBattle() {
    currentMonster = getRandomMonster();
    printCharByChar("\n=== 전투 시작 ===");
    
    char buffer[100];
    sprintf(buffer, "등장한 요괴: %s", currentMonster->name);
    printCharByChar(buffer);
    
    sprintf(buffer, "체력: %d", currentMonster->hp);
    printCharByChar(buffer);
    
    sprintf(buffer, "공격력: %d", currentMonster->attack);
    printCharByChar(buffer);
    
    sprintf(buffer, "방어력: %d", currentMonster->defense);
    printCharByChar(buffer);
    
    printCharByChar("\n행동을 선택하세요:");
    
    sprintf(buffer, "1. %s와 싸운다", currentMonster->name);
    printCharByChar(buffer);
    
    sprintf(buffer, "2. %s에게 부적을 던진다", currentMonster->name);
    printCharByChar(buffer);
    
    printCharByChar("3. 동료요괴를 본다");
    printCharByChar("4. 도망간다");
    printCharByChar("5. 게임을 저장하고 종료한다");
    printf("선택: ");
}

// 현재 스테이지 정보를 가져오기 위한 외부 함수 선언
extern Stage getCurrentStageInfo();

// 초기화 함수
void initializeTeamCode() {
    static int isInitialized = 0;
    if (!isInitialized) {
        srand(time(NULL));
        loadMonsters();
        loadSkills();
        isInitialized = 1;
    }
    startBattle();
}

int isTeamCodeCompleted() {
    static int choice = 0;
    
    // 사용자 입력 받기
    if (choice == 0) {
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("\n%s와 싸우기 시작합니다...\n", currentMonster->name);
                startCompanionBattle();  // 동료 요괴 전투 시스템 호출
                choice = 0;  // 다음 스테이지를 위해 초기화
                return 1;
                
            case 2:
                printf("\n%s에게 부적을 던집니다...\n", currentMonster->name);
                // 여기에 부적 던지기 관련 팀원 코드 호출
                choice = 0;  // 다음 스테이지를 위해 초기화
                return 1;
                
            case 3:
                printf("\n동료요괴를 확인합니다...\n");
                // 여기에 동료요괴 확인 관련 팀원 코드 호출
                choice = 0;  // 다음 스테이지를 위해 초기화
                return 1;
                
            case 4:
                if (rand() % 2 == 0) {  // 50% 확률
                    printf("\n무사히 도망쳤다!\n");
                    Sleep(1000);  // 1초 대기
                    printf("\n다음 스테이지로 이동합니다...\n");
                    Sleep(1000);  // 1초 대기
                    choice = 0;  // 다음 스테이지를 위해 초기화
                    return 1;
                } else {
                    printf("\n도망치는데 실패했습니다!\n");
                    printf("\n다시 행동을 선택하세요:\n");
                    printf("1. %s와 싸운다\n", currentMonster->name);
                    printf("2. %s에게 부적을 던진다\n", currentMonster->name);
                    printf("3. 동료요괴를 본다\n");
                    printf("4. 도망간다\n");
                    printf("5. 게임을 저장하고 종료한다\n");
                    printf("선택: ");
                    choice = 0;  // 다시 선택하도록 함
                    return 0;
                }

            case 5: {
                // 현재 스테이지 정보 가져오기
                Stage currentStage = getCurrentStageInfo();
                // 게임 저장
                saveGame(currentStage.stageNumber, currentStage.time, 
                        currentStage.location, currentStage.terrain, currentSpeedIndex);
                printCharByChar("\n게임이 저장되었습니다. 게임을 종료합니다...");
                Sleep(1000);
                exit(0);  // 게임 종료
                break;
            }
            
            default:
                printf("\n잘못된 선택입니다. 다시 선택해주세요.\n");
                printf("\n행동을 선택하세요:\n");
                printf("1. %s와 싸운다\n", currentMonster->name);
                printf("2. %s에게 부적을 던진다\n", currentMonster->name);
                printf("3. 동료요괴를 본다\n");
                printf("4. 도망간다\n");
                printf("5. 게임을 저장하고 종료한다\n");
                printf("선택: ");
                choice = 0;
                return 0;
        }
    }
    
    return 0;
} 