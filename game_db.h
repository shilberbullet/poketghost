#ifndef GAME_DB_H
#define GAME_DB_H

#define MAX_SPEEDS 10
#define MAX_COMPANIONS 5
#define MAX_SKILLS 4
#define MAX_AVAILABLE_SKILLS 10

// 동료 요괴의 기술 정보
typedef struct {
    char name[50];
    int power;
    int accuracy;
    char description[100];
} CompanionSkill;

// 동료 요괴 정보
typedef struct {
    char name[50];
    int level;
    int hp;
    int maxHp;
    int attack;
    int defense;
    int evasion;
    CompanionSkill currentSkills[MAX_SKILLS];  // 현재 사용 중인 기술
    CompanionSkill availableSkills[MAX_AVAILABLE_SKILLS];  // 사용 가능한 기술 목록
    int currentSkillCount;  // 현재 사용 중인 기술 수
    int availableSkillCount;  // 사용 가능한 기술 수
} Companion;

// 스테이지 정보를 저장하는 구조체
typedef struct {
    int stageNumber;
    char location[50];
    char terrain[50];
    int time;
} Stage;

// 게임 속도 설정을 저장하는 구조체
typedef struct {
    char name[20];
    int charDelay;    // 글자당 대기시간 (ms)
    int lineDelay;    // 줄간격 대기시간 (ms)
} GameSpeed;

// 게임 저장 데이터 구조체
typedef struct {
    int stageNumber;
    int time;
    char lastLocation[50];
    char lastTerrain[50];
    int gameSpeed;  // 게임 속도 설정
    Companion companions[MAX_COMPANIONS];  // 동료 요괴 목록
    int companionCount;  // 현재 보유한 동료 요괴 수
} SaveData;

// 파일 시스템 초기화
int initializeDB();

// 게임 저장
void saveGame(int stageNumber, int gameTime, const char* location, const char* terrain, int gameSpeed);

// 게임 불러오기
SaveData* loadGame();

// 파일 시스템 정리
void closeDB();

// 전투 후 시스템 함수 선언
void itemSystem();

#endif 