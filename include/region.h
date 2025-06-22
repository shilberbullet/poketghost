#ifndef REGION_H           // 중복 포함 방지를 위한 전처리기 지시문 시작
#define REGION_H           // 중복 포함 방지를 위한 매크로 정의

#include <stdio.h>   // 표준 입출력 함수 (FILE, printf 등) 사용을 위한 헤더
#include <stdlib.h>  // 표준 라이브러리 함수 (malloc, free 등) 사용을 위한 헤더
#include <string.h>  // 문자열 처리 함수 (strcpy, strcmp 등) 사용을 위한 헤더
#include <time.h>    // 시간 관련 함수 (time, srand 등) 사용을 위한 헤더

#define MAX_REGIONS 8 // 최대 지역 수 (게임 내 존재하는 지역의 총 개수)
#define MAX_CONNECTED 5 // 한 지역당 최대 연결 지역 수 (이웃 지역의 최대 개수)
#define REGION_NAME_LENGTH 20 // 지역 이름 최대 길이 (문자열 배열 크기)

/**
 * @brief 지역 정보 구조체
 * @details 지역의 이름, 방문 여부, 연결된 지역 정보를 저장
 */
typedef struct {
    char name[REGION_NAME_LENGTH];           // 지역 이름 (최대 REGION_NAME_LENGTH 길이)
    int visited;                            // 방문 여부 (1: 방문, 0: 미방문)
    int connectedCount;                     // 연결된 지역 수 (이웃 지역의 개수)
    char connected[MAX_CONNECTED][REGION_NAME_LENGTH];  // 연결된 지역 이름 배열 (이웃 지역 이름 목록)
} Region; // 한 지역의 정보를 저장하는 구조체

/**
 * @brief 지역 시스템을 초기화하는 함수
 */
void initRegionSystem(void); // 지역 배열, 방문 정보 등 내부 상태를 초기화하는 함수 선언

/**
 * @brief 현재 지역 이름을 가져오는 함수
 * @return 현재 지역 이름 (문자열 포인터)
 */
const char* getCurrentRegion(void); // 현재 플레이어가 위치한 지역의 이름을 반환하는 함수 선언

/**
 * @brief 다음 지역으로 이동하는 함수
 * @return 이동 성공 여부 (1: 성공, 0: 실패)
 */
int moveToNextRegion(void); // 연결된 지역 중 하나로 이동하는 함수 선언

/**
 * @brief 새 게임 시작 시 초기 지역을 설정하는 함수
 * @param choice 지역 선택 (1: 경상도, 2: 전라도)
 * @return 설정 성공 여부 (1: 성공, 0: 실패)
 */
int setInitialRegion(int choice); // 게임 시작 시 초기 지역을 선택 및 설정하는 함수 선언

/**
 * @brief 지역 방문 기록을 파일에 저장하는 함수
 * @param file 저장할 파일 포인터 (FILE*)
 */
void saveRegionData(FILE* file); // 지역 방문 정보를 파일에 저장하는 함수 선언

/**
 * @brief 지역 방문 기록을 파일에서 로드하는 함수
 * @param file 로드할 파일 포인터 (FILE*)
 */
void loadRegionData(FILE* file); // 파일에서 지역 방문 정보를 읽어오는 함수 선언

/**
 * @brief 현재 지역의 연결된 지역 목록을 표시하는 함수
 */
void displayConnectedRegions(void); // 현재 지역에서 이동 가능한 이웃 지역 목록을 출력하는 함수 선언

/**
 * @brief 지도를 기반으로 다음 지역으로 이동하는 함수
 * @return 이동 성공 여부 (1: 성공, 0: 실패)
 */
int moveToNextRegionWithMap(void); // 지도 정보를 활용해 다음 지역으로 이동하는 함수 선언

/**
 * @brief 모든 지역을 방문했는지 확인하는 함수
 * @return 모든 지역 방문 여부 (1: 모두 방문, 0: 미방문 지역 존재)
 */
int isAllRegionsVisited(void); // 모든 지역의 방문 여부를 검사하는 함수 선언

/**
 * @brief 방문하지 않은 지역 목록을 가져오는 함수
 * @param regions 방문하지 않은 지역 배열 (출력용)
 * @param maxCount 최대 개수 (배열 크기)
 * @return 방문하지 않은 지역 수 (int)
 */
int getUnvisitedRegions(char regions[][REGION_NAME_LENGTH], int maxCount); // 방문하지 않은 지역 이름을 배열로 반환하는 함수 선언

#endif // REGION_H           // 중복 포함 방지를 위한 전처리기 지시문 종료 