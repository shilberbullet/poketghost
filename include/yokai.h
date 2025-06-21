/**
 * @file yokai.h
 * @brief 포켓요괴 게임의 요괴 시스템 헤더 파일
 * @details 요괴의 구조체 정의, 생성, 관리 등의 함수를 선언합니다.
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef YOKAI_H
#define YOKAI_H

#include "types.h" // 기본 타입 정의
#include <stdbool.h> // 불린 타입 사용을 위한 헤더
#include "move.h" // 기술 관련 헤더
#include "item.h" // 아이템 관련 헤더

#define MAX_MOVES 4 // 요괴가 가질 수 있는 최대 기술 수
#define MAX_LEARNABLE_MOVES 50 // 배울 수 있는 최대 기술 수
#define YOKAI_NAME_MAX 32 // 요괴 이름의 최대 길이
#define MAX_YOKAI 150 // 최대 요괴 종류 수
#define MAX_BOSS_YOKAI 12 // 최대 보스 요괴 종류 수
#define YOKAI_DESC_MAX 128 // 요괴 설명의 최대 길이
#define MAX_PARADOX_YOKAI 16 // 최대 패러독스 요괴 종류 수
#define MAX_MAGNIFIER_COUNT 5  // 최대 돋보기 개수

// 상성 이름 배열
extern const char* typeNames[]; // 상성별 이름을 담는 배열

// 상성 설명 배열
extern const char* typeDescriptions[]; // 상성별 설명을 담는 배열

// 요괴 구조체
typedef struct {
    unsigned long long id;  // 요괴의 고유 ID
    char name[YOKAI_NAME_MAX]; // 요괴 이름
    int level;         // 레벨
    int exp;           // 경험치 필드
    int attack;         // 공격력
    int defense;        // 방어력
    int stamina;         // 체력 종족값
    int speed;         // 스피드
    float currentHP;    // 현재 HP
    YokaiType type;    // 상성
    YokaiStatus status; // 상태
    char desc[YOKAI_DESC_MAX];    // 도감설명
    MoveInstance moves[MAX_MOVES];  // 실제 전투/동료가 가진 기술
    int moveCount;     // 현재 배운 기술 수
    Move learnableMoves[MAX_LEARNABLE_MOVES]; // 배울 수 있는 기술 목록
    int learnableMoveCount; // 배울 수 있는 기술 개수
    Move learnedMoves[MAX_LEARNABLE_MOVES];  // 배운 기술 목록
    int learnedMoveCount;                    // 배운 기술 개수
    int forgottenMoveCount;  // 잊은 기술 개수
    Move forgottenMoves[MAX_LEARNABLE_MOVES];  // 잊은 기술 목록
    // 요괴 인벤토리 추가
    InventoryItem yokaiInventory[INVENTORY_MAX]; // 요괴별 인벤토리
    int yokaiInventoryCount; // 요괴 인벤토리 아이템 개수
    int magnifierCount;  // 보유 중인 돋보기 개수
} Yokai; // 요괴의 모든 정보를 담는 구조체

// 요괴 이름 배열
extern const char* yokaiNames[]; // 모든 요괴 이름을 담는 배열

// 요괴 이름 개수
extern const int YOKAI_COUNT; // 총 요괴 종류 수

extern Yokai yokaiList[MAX_YOKAI]; // 일반 요괴 목록
extern int yokaiListCount; // 일반 요괴 목록 개수

extern Yokai bossYokaiList[MAX_BOSS_YOKAI]; // 보스 요괴 목록
extern int bossYokaiListCount; // 보스 요괴 목록 개수

extern Yokai paradoxYokaiList[MAX_PARADOX_YOKAI]; // 패러독스 요괴 목록
extern int paradoxYokaiListCount; // 패러독스 요괴 목록 개수

/**
 * @brief 파일에서 요괴 데이터를 로드하는 함수
 * @param filename 로드할 파일명
 */
void loadYokaiFromFile(const char* filename);

/**
 * @brief 이름으로 요괴를 찾는 함수
 * @param name 찾을 요괴 이름
 * @return 찾은 요괴 포인터, 없으면 NULL
 */
Yokai* findYokaiByName(const char* name);

// HP 계산 함수
/**
 * @brief 요괴의 최대 HP를 계산하는 함수
 * @param yokai HP를 계산할 요괴
 * @return 계산된 최대 HP
 */
float calculateHP(const Yokai* yokai);

// 랜덤 요괴 생성 함수
/**
 * @brief 랜덤한 일반 요괴를 생성하는 함수
 * @return 생성된 요괴
 */
Yokai createRandomYokai();

// 랜덤 보스 요괴 생성 함수
/**
 * @brief 랜덤한 보스 요괴를 생성하는 함수
 * @return 생성된 보스 요괴
 */
Yokai createRandomBossYokai();

// 요괴 정보 출력 함수
/**
 * @brief 요괴의 상세 정보를 화면에 출력하는 함수
 * @param yokai 출력할 요괴
 */
void printYokaiInfo(const Yokai* yokai);

// 요괴 인벤토리 관련 함수들
/**
 * @brief 요괴의 인벤토리를 초기화하는 함수
 * @param yokai 초기화할 요괴
 */
void initYokaiInventory(Yokai* yokai);

/**
 * @brief 요괴의 인벤토리를 화면에 출력하는 함수
 * @param yokai 인벤토리를 출력할 요괴
 */
void printYokaiInventory(const Yokai* yokai);

/**
 * @brief 요괴의 인벤토리에 아이템을 추가하는 함수
 * @param yokai 아이템을 추가할 요괴
 * @param item 추가할 아이템
 * @return 성공 여부
 */
bool addItemToYokaiInventory(Yokai* yokai, const Item* item);

/**
 * @brief 요괴의 인벤토리에서 아이템을 제거하는 함수
 * @param yokai 아이템을 제거할 요괴
 * @param itemIndex 제거할 아이템 인덱스
 * @return 성공 여부
 */
bool removeItemFromYokaiInventory(Yokai* yokai, int itemIndex);

/**
 * @brief 요괴의 인벤토리를 비우는 함수
 * @param yokai 인벤토리를 비울 요괴
 */
void clearYokaiInventory(Yokai* yokai);

// 상성 관계 확인 함수 (공격자의 공격력이 방어자에게 얼마나 효과적인지 반환)
/**
 * @brief 상성 효과를 확인하는 함수
 * @param attacker 공격자 타입
 * @param defender 방어자 타입
 * @return 상성 효과 배율
 */
float getTypeEffectiveness(YokaiType attacker, YokaiType defender);

// 상성 효과를 계산하는 함수
/**
 * @brief 상성 효과를 계산하는 함수
 * @param attackerType 공격자 타입
 * @param defenderType 방어자 타입
 * @return 상성 효과 배율
 */
float calculateTypeEffectiveness(YokaiType attackerType, YokaiType defenderType);

// 스테이지 번호에 따른 레벨 범위 계산 함수
/**
 * @brief 스테이지에 따른 요괴 레벨 범위를 계산하는 함수
 * @param stage 스테이지 번호
 * @param minLevel 최소 레벨 (출력)
 * @param maxLevel 최대 레벨 (출력)
 */
void calculateLevelRange(int stage, int* minLevel, int* maxLevel);

// 랜덤 요괴 생성 함수 (레벨 지정)
/**
 * @brief 지정된 레벨의 랜덤 일반 요괴를 생성하는 함수
 * @param level 생성할 레벨
 * @return 생성된 요괴
 */
Yokai createRandomYokaiWithLevel(int level);

// 랜덤 보스 요괴 생성 함수 (레벨 지정)
/**
 * @brief 지정된 레벨의 랜덤 보스 요괴를 생성하는 함수
 * @param level 생성할 레벨
 * @return 생성된 보스 요괴
 */
Yokai createRandomBossYokaiWithLevel(int level);

// 랜덤 패러독스 요괴 생성 함수 (레벨 지정)
/**
 * @brief 지정된 레벨의 랜덤 패러독스 요괴를 생성하는 함수
 * @param level 생성할 레벨
 * @return 생성된 패러독스 요괴
 */
Yokai createRandomParadoxYokaiWithLevel(int level);

// Yokai 구조체 정의 이후에 추가
/**
 * @brief 찹쌀경단 효과를 시도하는 함수
 * @param yokai 효과를 시도할 요괴
 * @return 성공 여부
 */
bool tryActivateChapsalgyungdan(Yokai* yokai);

// learnableMoves에서 랜덤 4개를 moves에 복사하는 함수
/**
 * @brief 배울 수 있는 기술 중 랜덤 4개를 실제 기술로 할당하는 함수
 * @param y 할당할 요괴
 * @param level 요괴 레벨
 */
void assignRandomMoves(Yokai* y, int level);

#endif // YOKAI_H 