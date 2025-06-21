/**
 * @file region.h
 * @brief 포켓요괴 게임의 지역 시스템 헤더 파일
 * @details 지역 관리, 이동, 방문 기록 등의 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef REGION_H
#define REGION_H

#include <stdio.h> // 표준 입출력 함수
#include <stdlib.h> // 표준 라이브러리 함수
#include <string.h> // 문자열 처리 함수
#include <time.h> // 시간 관련 함수

#define MAX_REGIONS 8 // 최대 지역 수
#define MAX_CONNECTED 5 // 한 지역당 최대 연결 지역 수
#define REGION_NAME_LENGTH 20 // 지역 이름 최대 길이

/**
 * @brief 지역 정보 구조체
 * @details 지역의 이름, 방문 여부, 연결된 지역 정보를 저장
 */
typedef struct {
    char name[REGION_NAME_LENGTH];           // 지역 이름
    int visited;                            // 방문 여부
    int connectedCount;                     // 연결된 지역 수
    char connected[MAX_CONNECTED][REGION_NAME_LENGTH];  // 연결된 지역 이름 배열
} Region;

/**
 * @brief 지역 시스템을 초기화하는 함수
 */
void initRegionSystem(void);

/**
 * @brief 현재 지역 이름을 가져오는 함수
 * @return 현재 지역 이름
 */
const char* getCurrentRegion(void);

/**
 * @brief 다음 지역으로 이동하는 함수
 * @return 이동 성공 여부
 */
int moveToNextRegion(void);

/**
 * @brief 새 게임 시작 시 초기 지역을 설정하는 함수
 * @param choice 지역 선택 (1: 경상도, 2: 전라도)
 * @return 설정 성공 여부
 */
int setInitialRegion(int choice);

/**
 * @brief 지역 방문 기록을 파일에 저장하는 함수
 * @param file 저장할 파일 포인터
 */
void saveRegionData(FILE* file);

/**
 * @brief 지역 방문 기록을 파일에서 로드하는 함수
 * @param file 로드할 파일 포인터
 */
void loadRegionData(FILE* file);

/**
 * @brief 현재 지역의 연결된 지역 목록을 표시하는 함수
 */
void displayConnectedRegions(void);

/**
 * @brief 지도를 기반으로 다음 지역으로 이동하는 함수
 * @return 이동 성공 여부
 */
int moveToNextRegionWithMap(void);

/**
 * @brief 모든 지역을 방문했는지 확인하는 함수
 * @return 모든 지역 방문 여부
 */
int isAllRegionsVisited(void);

/**
 * @brief 방문하지 않은 지역 목록을 가져오는 함수
 * @param regions 방문하지 않은 지역 배열
 * @param maxCount 최대 개수
 * @return 방문하지 않은 지역 수
 */
int getUnvisitedRegions(char regions[][REGION_NAME_LENGTH], int maxCount);

#endif // REGION_H 