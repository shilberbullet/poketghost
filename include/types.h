/**
 * @file types.h
 * @brief 포켓요괴 게임의 기본 타입 정의 헤더 파일
 * @details 요괴 타입, 상태, 기술 등급 등의 기본 열거형과 구조체를 정의합니다.
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h> // 불린 타입 사용을 위한 헤더

/** @brief 요괴 이름의 최대 길이 - 문자열 버퍼 크기 제한 */
#define YOKAI_NAME_MAX 32
/** @brief 요괴 타입의 총 개수 - 게임 내 사용 가능한 타입 수 */
#define TYPE_COUNT 5

/**
 * @brief 요괴 타입 열거형
 * @details 요괴의 속성을 분류하는 타입을 정의합니다.
 */
typedef enum {
    TYPE_EVIL_SPIRIT,  /**< 악귀 - 악한 영혼의 요괴, 어둠 속성 기술에 강함 */
    TYPE_GHOST,        /**< 원귀 - 원한을 가진 영혼의 요괴, 정신 속성 기술에 강함 */
    TYPE_MONSTER,      /**< 괴수형 - 괴물 형태의 요괴, 물리 속성 기술에 강함 */
    TYPE_HUMAN,        /**< 인간형 - 인간과 유사한 형태의 요괴, 균형잡힌 능력치 */
    TYPE_ANIMAL        /**< 동물형 - 동물 형태의 요괴, 자연 속성 기술에 강함 */
} YokaiType;

/**
 * @brief 요괴 상태 열거형
 * @details 요괴의 현재 상태를 나타냅니다.
 */
typedef enum {
    YOKAI_NORMAL,     /**< 정상 - 전투 가능한 상태, 모든 행동 가능 */
    YOKAI_FAINTED,    /**< 기절 - 전투 불가능한 상태, 회복 아이템 필요 */
    YOKAI_RELEASED    /**< 성불 - 해방된 상태, 게임에서 완전히 제거됨 */
} YokaiStatus;

/**
 * @brief 기술 등급 열거형
 * @details 기술의 강력함을 나타내는 등급을 정의합니다.
 */
typedef enum {
    MOVE_BASIC,    /**< 초급 - 기본적인 기술, 낮은 공격력이지만 안정적 */
    MOVE_MEDIUM,   /**< 중급 - 중간 수준의 기술, 균형잡힌 성능 */
    MOVE_ADVANCED  /**< 고급 - 고급 기술, 높은 공격력이지만 PP 소모 많음 */
} MoveGrade;

/**
 * @brief 기술 구조체
 * @details 요괴가 사용할 수 있는 기술의 정보를 담습니다.
 */
typedef struct {
    char name[YOKAI_NAME_MAX];    /**< 기술 이름 - 최대 32자까지 저장 가능 */
    YokaiType type;               /**< 기술의 상성 타입 - 요괴 타입과의 상성 계산에 사용 */
    int power;                    /**< 공격력 - 기술의 기본 데미지 수치 */
    int accuracy;                 /**< 명중률 (0-100) - 기술이 성공할 확률 */
    int pp;                       /**< 최대 사용 가능 횟수 (PP) - 기술 사용 제한 */
    char description[256];        /**< 기술 설명 - 플레이어에게 표시되는 설명문 */
    MoveGrade grade;              /**< 기술 등급 - 기술의 강력함과 희귀도 */
} Move;

/**
 * @brief 기술 인스턴스 구조체
 * @details 실제 전투에서 사용되는 기술의 현재 상태를 담습니다.
 */
typedef struct {
    Move move;                    /**< 기술 정보 - 기본 기술 데이터 */
    int currentPP;               /**< 현재 남은 PP - 전투 중 감소하는 사용 횟수 */
} MoveInstance;

/**
 * @brief 요괴 타입을 문자열로 변환하는 함수
 * @param type 변환할 요괴 타입 - 열거형 값
 * @return 타입에 해당하는 문자열 포인터 - 화면 출력용
 */
const char* typeToString(YokaiType type);

/**
 * @brief 기술 등급을 문자열로 변환하는 함수
 * @param grade 변환할 기술 등급 - 열거형 값
 * @return 등급에 해당하는 문자열 포인터 - 화면 출력용
 */
const char* gradeToString(MoveGrade grade);

#endif // TYPES_H 