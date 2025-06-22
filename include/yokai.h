/**
 * @file yokai.h
 * @brief 포켓요괴 게임의 요괴 시스템 헤더 파일
 * @details 요괴의 구조체 정의, 생성, 관리 등의 함수를 선언합니다.
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef YOKAI_H           // 중복 포함 방지를 위한 전처리기 지시문 시작
#define YOKAI_H           // 중복 포함 방지를 위한 매크로 정의

#include "types.h"       // 기본 타입 정의 (공용 타입, 상수 등)
#include <stdbool.h>      // 불린 타입 사용을 위한 헤더 (bool, true, false)
#include "move.h"       // 기술 관련 헤더 (Move, MoveInstance 등)
#include "item.h"       // 아이템 관련 헤더 (Item, InventoryItem 등)

#define MAX_MOVES 4                // 요괴가 가질 수 있는 최대 기술 수
#define MAX_LEARNABLE_MOVES 50     // 요괴가 배울 수 있는 최대 기술 수
#define YOKAI_NAME_MAX 32          // 요괴 이름의 최대 길이 (문자열 배열 크기)
#define MAX_YOKAI 150              // 최대 요괴 종류 수 (일반 요괴)
#define MAX_BOSS_YOKAI 12          // 최대 보스 요괴 종류 수
#define YOKAI_DESC_MAX 128         // 요괴 설명의 최대 길이 (도감 설명)
#define MAX_PARADOX_YOKAI 16       // 최대 패러독스 요괴 종류 수
#define MAX_MAGNIFIER_COUNT 5      // 최대 돋보기 개수 (요괴별 보유 가능 수)

// 상성 이름 배열 (각 타입의 이름)
extern const char* typeNames[]; // 상성별 이름을 담는 배열 (예: 불, 물, 풀 등)

// 상성 설명 배열 (각 타입의 설명)
extern const char* typeDescriptions[]; // 상성별 설명을 담는 배열

// 요괴 구조체 (모든 요괴의 상태와 정보를 저장)
typedef struct {
    unsigned long long id;  // 요괴의 고유 ID (식별자)
    char name[YOKAI_NAME_MAX]; // 요괴 이름 (최대 YOKAI_NAME_MAX 길이)
    int level;         // 레벨 (성장 단계)
    int exp;           // 경험치 필드 (레벨업에 사용)
    int attack;         // 공격력 (전투 시 사용)
    int defense;        // 방어력 (전투 시 사용)
    int stamina;         // 체력 종족값 (최대 HP 계산에 사용)
    int speed;         // 스피드 (행동 순서 결정)
    float currentHP;    // 현재 HP (실제 체력)
    YokaiType type;    // 상성 (타입)
    YokaiStatus status; // 상태 (정상, 기절 등)
    char desc[YOKAI_DESC_MAX];    // 도감설명 (설명 텍스트)
    MoveInstance moves[MAX_MOVES];  // 실제 전투/동료가 가진 기술 (최대 4개)
    int moveCount;     // 현재 배운 기술 수
    Move learnableMoves[MAX_LEARNABLE_MOVES]; // 배울 수 있는 기술 목록
    int learnableMoveCount; // 배울 수 있는 기술 개수
    Move learnedMoves[MAX_LEARNABLE_MOVES];  // 배운 기술 목록 (과거 이력)
    int learnedMoveCount;                    // 배운 기술 개수
    int forgottenMoveCount;  // 잊은 기술 개수
    Move forgottenMoves[MAX_LEARNABLE_MOVES];  // 잊은 기술 목록
    // 요괴 인벤토리 추가
    InventoryItem yokaiInventory[INVENTORY_MAX]; // 요괴별 인벤토리 (개별 아이템 보유)
    int yokaiInventoryCount; // 요괴 인벤토리 아이템 개수
    int magnifierCount;  // 보유 중인 돋보기 개수
} Yokai; // 요괴의 모든 정보를 담는 구조체

// 요괴 이름 배열 (모든 요괴의 이름)
extern const char* yokaiNames[]; // 모든 요괴 이름을 담는 배열

// 요괴 이름 개수 (총 등록된 요괴 수)
extern const int YOKAI_COUNT; // 총 요괴 종류 수

extern Yokai yokaiList[MAX_YOKAI]; // 일반 요괴 목록 (최대 MAX_YOKAI)
extern int yokaiListCount; // 일반 요괴 목록 개수

extern Yokai bossYokaiList[MAX_BOSS_YOKAI]; // 보스 요괴 목록 (최대 MAX_BOSS_YOKAI)
extern int bossYokaiListCount; // 보스 요괴 목록 개수

extern Yokai paradoxYokaiList[MAX_PARADOX_YOKAI]; // 패러독스 요괴 목록 (최대 MAX_PARADOX_YOKAI)
extern int paradoxYokaiListCount; // 패러독스 요괴 목록 개수

/**
 * @brief 파일에서 요괴 데이터를 로드하는 함수
 * @param filename 로드할 파일명 (텍스트 파일 경로)
 */
void loadYokaiFromFile(const char* filename); // 파일에서 요괴 데이터를 읽어오는 함수 선언

/**
 * @brief 이름으로 요괴를 찾는 함수
 * @param name 찾을 요괴 이름 (문자열)
 * @return 찾은 요괴 포인터, 없으면 NULL
 */
Yokai* findYokaiByName(const char* name); // 이름으로 요괴를 검색하는 함수 선언

// HP 계산 함수
/**
 * @brief 요괴의 최대 HP를 계산하는 함수
 * @param yokai HP를 계산할 요괴 (포인터)
 * @return 계산된 최대 HP (float)
 */
float calculateHP(const Yokai* yokai); // 요괴의 최대 HP를 계산하는 함수 선언

// 랜덤 요괴 생성 함수
/**
 * @brief 랜덤한 일반 요괴를 생성하는 함수
 * @return 생성된 요괴 (Yokai 구조체)
 */
Yokai createRandomYokai(); // 랜덤 일반 요괴를 생성하는 함수 선언

// 랜덤 보스 요괴 생성 함수
/**
 * @brief 랜덤한 보스 요괴를 생성하는 함수
 * @return 생성된 보스 요괴 (Yokai 구조체)
 */
Yokai createRandomBossYokai(); // 랜덤 보스 요괴를 생성하는 함수 선언

// 요괴 정보 출력 함수
/**
 * @brief 요괴의 상세 정보를 화면에 출력하는 함수
 * @param yokai 출력할 요괴 (포인터)
 */
void printYokaiInfo(const Yokai* yokai); // 요괴의 상세 정보를 출력하는 함수 선언

// 요괴 인벤토리 관련 함수들
/**
 * @brief 요괴의 인벤토리를 초기화하는 함수
 * @param yokai 초기화할 요괴 (포인터)
 */
void initYokaiInventory(Yokai* yokai); // 요괴의 인벤토리를 초기화하는 함수 선언

/**
 * @brief 요괴의 인벤토리를 화면에 출력하는 함수
 * @param yokai 인벤토리를 출력할 요괴 (포인터)
 */
void printYokaiInventory(const Yokai* yokai); // 요괴의 인벤토리를 출력하는 함수 선언

/**
 * @brief 요괴의 인벤토리에 아이템을 추가하는 함수
 * @param yokai 아이템을 추가할 요괴 (포인터)
 * @param item 추가할 아이템 (포인터)
 * @return 성공 여부 (true/false)
 */
bool addItemToYokaiInventory(Yokai* yokai, const Item* item); // 요괴 인벤토리에 아이템을 추가하는 함수 선언

/**
 * @brief 요괴의 인벤토리에서 아이템을 제거하는 함수
 * @param yokai 아이템을 제거할 요괴 (포인터)
 * @param itemIndex 제거할 아이템 인덱스 (int)
 * @return 성공 여부 (true/false)
 */
bool removeItemFromYokaiInventory(Yokai* yokai, int itemIndex); // 요괴 인벤토리에서 아이템을 제거하는 함수 선언

/**
 * @brief 요괴의 인벤토리를 비우는 함수
 * @param yokai 인벤토리를 비울 요괴 (포인터)
 */
void clearYokaiInventory(Yokai* yokai); // 요괴의 인벤토리를 비우는 함수 선언

// 상성 관계 확인 함수 (공격자의 공격력이 방어자에게 얼마나 효과적인지 반환)
/**
 * @brief 상성 효과를 확인하는 함수
 * @param attacker 공격자 타입 (YokaiType)
 * @param defender 방어자 타입 (YokaiType)
 * @return 상성 효과 배율 (float)
 */
float getTypeEffectiveness(YokaiType attacker, YokaiType defender); // 상성 효과를 반환하는 함수 선언

// 상성 효과를 계산하는 함수
/**
 * @brief 상성 효과를 계산하는 함수
 * @param attackerType 공격자 타입 (YokaiType)
 * @param defenderType 방어자 타입 (YokaiType)
 * @return 상성 효과 배율 (float)
 */
float calculateTypeEffectiveness(YokaiType attackerType, YokaiType defenderType); // 상성 효과를 계산하는 함수 선언

// 스테이지 번호에 따른 레벨 범위 계산 함수
/**
 * @brief 스테이지에 따른 요괴 레벨 범위를 계산하는 함수
 * @param stage 스테이지 번호 (int)
 * @param minLevel 최소 레벨 (출력, int*)
 * @param maxLevel 최대 레벨 (출력, int*)
 */
void calculateLevelRange(int stage, int* minLevel, int* maxLevel); // 스테이지에 따라 요괴 레벨 범위를 계산하는 함수 선언

// 랜덤 요괴 생성 함수 (레벨 지정)
/**
 * @brief 지정된 레벨의 랜덤 일반 요괴를 생성하는 함수
 * @param level 생성할 레벨 (int)
 * @return 생성된 요괴 (Yokai 구조체)
 */
Yokai createRandomYokaiWithLevel(int level); // 지정 레벨의 랜덤 일반 요괴 생성 함수 선언

// 랜덤 보스 요괴 생성 함수 (레벨 지정)
/**
 * @brief 지정된 레벨의 랜덤 보스 요괴를 생성하는 함수
 * @param level 생성할 레벨 (int)
 * @return 생성된 보스 요괴 (Yokai 구조체)
 */
Yokai createRandomBossYokaiWithLevel(int level); // 지정 레벨의 랜덤 보스 요괴 생성 함수 선언

// 랜덤 패러독스 요괴 생성 함수 (레벨 지정)
/**
 * @brief 지정된 레벨의 랜덤 패러독스 요괴를 생성하는 함수
 * @param level 생성할 레벨 (int)
 * @return 생성된 패러독스 요괴 (Yokai 구조체)
 */
Yokai createRandomParadoxYokaiWithLevel(int level); // 지정 레벨의 랜덤 패러독스 요괴 생성 함수 선언

// Yokai 구조체 정의 이후에 추가
/**
 * @brief 찹쌀경단 효과를 시도하는 함수
 * @param yokai 효과를 시도할 요괴 (포인터)
 * @return 성공 여부 (true/false)
 */
bool tryActivateChapsalgyungdan(Yokai* yokai); // 찹쌀경단 효과를 시도하는 함수 선언

// learnableMoves에서 랜덤 4개를 moves에 복사하는 함수
/**
 * @brief 배울 수 있는 기술 중 랜덤 4개를 실제 기술로 할당하는 함수
 * @param y 할당할 요괴 (포인터)
 * @param level 요괴 레벨 (int)
 */
void assignRandomMoves(Yokai* y, int level); // 배울 수 있는 기술 중 랜덤 4개를 moves에 복사하는 함수 선언

#endif // YOKAI_H           // 중복 포함 방지를 위한 전처리기 지시문 종료 