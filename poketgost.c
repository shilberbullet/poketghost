#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include "game_db.h"
#include "boss_monsters.h"

#define MAX_LOCATIONS 10
#define MAX_TERRAINS 5
#define STAGES_PER_LOCATION 10
#define MAX_SPEEDS 10

// 전역 변수로 지역 목록 저장
char locations[MAX_LOCATIONS][50];
const char* terrains[MAX_TERRAINS] = {"산", "강", "바다", "논", "마을"};

// 전역 변수로 게임 속도 설정 저장
GameSpeed gameSpeeds[MAX_SPEEDS];
int speedCount = 0;
int currentSpeedIndex = 1;  // 기본 속도로 시작

// 전역 변수로 현재 스테이지 정보 저장
int currentStage = 1;
int currentTime = 0;

// 전역 변수로 현재 게임 상태 저장
SaveData currentGameData;

// 외부 함수 선언
extern void printCharByChar(const char* str);

// 지역 정보 파일에서 읽어오기
void loadLocations() {
    FILE* file = fopen("locations.txt", "r");
    if (file == NULL) {
        printf("지역 정보 파일을 열 수 없습니다.\n");
        exit(1);
    }

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        if (fgets(locations[i], sizeof(locations[i]), file) == NULL) break;
        locations[i][strcspn(locations[i], "\n")] = 0;
    }
    fclose(file);
}

// 게임 속도 설정 파일에서 읽어오기
void loadGameSpeeds() {
    FILE* file = fopen("game_speed.txt", "r");
    if (file == NULL) {
        printf("게임 속도 설정 파일을 열 수 없습니다.\n");
        exit(1);
    }

    char line[256];
    // 주석 라인 건너뛰기
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file) && speedCount < MAX_SPEEDS) {
        line[strcspn(line, "\n")] = 0;  // 개행문자 제거
        char* token = strtok(line, ",");
        strcpy(gameSpeeds[speedCount].name, token);
        
        token = strtok(NULL, ",");
        gameSpeeds[speedCount].charDelay = atoi(token);
        
        token = strtok(NULL, ",");
        gameSpeeds[speedCount].lineDelay = atoi(token);
        
        speedCount++;
    }
    fclose(file);
}

// 랜덤 지형 선택
const char* getRandomTerrain() {
    return terrains[rand() % MAX_TERRAINS];
}

// 스테이지 정보 출력 함수
void displayStageInfo(Stage stage) {
    printCharByChar("\n=== 스테이지 정보 ===");
    
    char buffer[100];
    sprintf(buffer, "스테이지 번호: %d", stage.stageNumber);
    printCharByChar(buffer);
    
    sprintf(buffer, "지역: %s", stage.location);
    printCharByChar(buffer);
    
    sprintf(buffer, "지형: %s", stage.terrain);
    printCharByChar(buffer);
    
    sprintf(buffer, "시간: %d시", stage.time);
    printCharByChar(buffer);
    
    printCharByChar("==================");
    printf("\n");
}

// 현재 스테이지 정보 생성
Stage createStageInfo(int stageNumber, int time) {
    Stage stage;
    stage.stageNumber = stageNumber;
    stage.time = time;
    
    int locationIndex = ((stageNumber - 1) / STAGES_PER_LOCATION) % MAX_LOCATIONS;
    strcpy(stage.location, locations[locationIndex]);
    strcpy(stage.terrain, getRandomTerrain());
    
    return stage;
}

// 팀원 코드 완료 여부 확인 함수
extern int isTeamCodeCompleted();
extern void initializeTeamCode();  // 초기화 함수 선언

// 현재 스테이지 정보를 가져오는 함수
Stage getCurrentStageInfo() {
    return createStageInfo(currentStage, currentTime);
}

// 스테이지 진행 함수
void progressStage() {
    initializeTeamCode();  // 새로운 전투 시작
    
    while (!isTeamCodeCompleted()) {
        Sleep(1000);
    }
    
    currentStage++;
    currentTime = (currentTime + 1) % 24;
    
    // 보스 스테이지 확인
    if (isBossStage(currentStage)) {
        printCharByChar("\n보스 요괴가 나타났다!");
        Sleep(2000);
    }
    
    printCharByChar("다음 스테이지로 이동합니다!");
    
    // 현재 스테이지 정보 생성 및 저장
    Stage currentStageInfo = createStageInfo(currentStage, currentTime);
    saveGame(currentStage, currentTime, currentStageInfo.location, currentStageInfo.terrain, currentSpeedIndex);
}

// 메인 게임 시작 함수
void startGame() {
    srand(time(NULL));
    loadLocations();
    
    currentStage = 1;
    currentTime = 0;
    
    // 게임 데이터 초기화
    memset(&currentGameData, 0, sizeof(SaveData));
    currentGameData.stageNumber = currentStage;
    currentGameData.time = currentTime;
    currentGameData.companionCount = 0;
    
    // 기본 동료 요괴(도깨비) 추가
    Companion* dokkaebi = &currentGameData.companions[currentGameData.companionCount];
    strcpy(dokkaebi->name, "도깨비");
    dokkaebi->level = 1;
    dokkaebi->hp = 100;
    dokkaebi->maxHp = 100;
    dokkaebi->attack = 15;
    dokkaebi->defense = 10;
    dokkaebi->evasion = 5;
    
    // 도깨비의 기본 기술 설정
    dokkaebi->currentSkillCount = 2;
    
    // 기술 1: 불꽃 발사
    strcpy(dokkaebi->currentSkills[0].name, "불꽃 발사");
    dokkaebi->currentSkills[0].power = 20;
    dokkaebi->currentSkills[0].accuracy = 90;
    strcpy(dokkaebi->currentSkills[0].description, "강력한 불꽃을 발사하여 적을 공격합니다.");
    
    // 기술 2: 도깨비 불
    strcpy(dokkaebi->currentSkills[1].name, "도깨비 불");
    dokkaebi->currentSkills[1].power = 15;
    dokkaebi->currentSkills[1].accuracy = 95;
    strcpy(dokkaebi->currentSkills[1].description, "도깨비의 특기인 불을 사용하여 적을 공격합니다.");
    
    // 사용 가능한 기술 목록 설정
    dokkaebi->availableSkillCount = 2;
    memcpy(dokkaebi->availableSkills, dokkaebi->currentSkills, 
           sizeof(CompanionSkill) * dokkaebi->availableSkillCount);
    
    currentGameData.companionCount++;
    
    // 초기 게임 데이터 저장
    saveGame(currentStage, currentTime, "시작", "평지", currentSpeedIndex);
    
    while (1) {
        Stage currentStageInfo = createStageInfo(currentStage, currentTime);
        displayStageInfo(currentStageInfo);
        
        progressStage();
    }
}

// 게임 이어하기 함수
void continueGame(int stageNumber, int gameTime, const char* location, const char* terrain, int savedSpeed) {
    srand(time(NULL));
    loadLocations();
    
    // 저장된 게임 데이터 불러오기
    SaveData* loadedData = loadGame();
    if (loadedData) {
        memcpy(&currentGameData, loadedData, sizeof(SaveData));
        free(loadedData);
    }
    
    // 저장된 게임 속도 적용
    currentSpeedIndex = savedSpeed;
    
    // 저장된 스테이지 정보 적용
    currentStage = stageNumber;
    currentTime = gameTime;
    
    while (1) {
        Stage currentStageInfo = createStageInfo(currentStage, currentTime);
        displayStageInfo(currentStageInfo);
        
        progressStage();
    }
}
