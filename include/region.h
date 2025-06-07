#ifndef REGION_H
#define REGION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_REGIONS 8
#define MAX_CONNECTED 4
#define REGION_NAME_LENGTH 20

typedef struct {
    char name[REGION_NAME_LENGTH];           // 지역 이름
    int visited;                            // 방문 여부
    int connectedCount;                     // 연결된 지역 수
    char connected[MAX_CONNECTED][REGION_NAME_LENGTH];  // 연결된 지역 이름 배열
} Region;

// 지역 시스템 초기화
void initRegionSystem(void);

// 현재 지역 가져오기
const char* getCurrentRegion(void);

// 다음 지역으로 이동
int moveToNextRegion(void);

// 새 게임 시작 시 초기 지역 설정
int setInitialRegion(int choice);

// 지역 방문 기록 저장
void saveRegionData(FILE* file);

// 지역 방문 기록 로드
void loadRegionData(FILE* file);

// 현재 지역의 연결된 지역 목록 표시
void displayConnectedRegions(void);

#endif // REGION_H 