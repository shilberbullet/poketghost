#include "../include/region.h"
#include "../include/text.h"
#include "../include/input.h"
#include "../include/settings.h"
#include "../include/logger.h"
#include <windows.h>
#include <stdbool.h>  // bool 타입을 위해 추가

// 지역 데이터
static Region regions[MAX_REGIONS] = {
    {"함경도", 0, 3, {"평안도", "강원도", "황해도"}},
    {"평안도", 0, 2, {"함경도", "황해도"}},
    {"황해도", 0, 3, {"평안도", "경기도", "강원도"}},
    {"강원도", 0, 5, {"함경도", "황해도", "경기도", "경상도", "충청도"}},
    {"경기도", 0, 3, {"황해도", "강원도", "충청도"}},
    {"충청도", 0, 4, {"경기도", "경상도", "전라도", "강원도"}},
    {"경상도", 0, 3, {"강원도", "충청도", "전라도"}},
    {"전라도", 0, 2, {"충청도", "경상도"}}
};

static char currentRegion[REGION_NAME_LENGTH];

void initRegionSystem(void) {
    LOG_FUNCTION_EXECUTION("initRegionSystem");
    srand((unsigned int)time(NULL));
}

const char* getCurrentRegion(void) {
    LOG_FUNCTION_EXECUTION("getCurrentRegion");
    return currentRegion;
}

int setInitialRegion(int choice) {
    LOG_FUNCTION_EXECUTION("setInitialRegion");
    if (choice == 1) {
        strcpy(currentRegion, "경상도");
        regions[6].visited = 1;  // 경상도 방문 표시
        return 1;
    } else if (choice == 2) {
        strcpy(currentRegion, "전라도");
        regions[7].visited = 1;  // 전라도 방문 표시
        return 1;
    }
    return 0;
}

int moveToNextRegion(void) {
    LOG_FUNCTION_EXECUTION("moveToNextRegion");
    // 현재 지역의 인덱스 찾기
    int currentIndex = -1;
    for (int i = 0; i < MAX_REGIONS; i++) {
        if (strcmp(regions[i].name, currentRegion) == 0) {
            currentIndex = i;
            break;
        }
    }
    
    if (currentIndex == -1) return 0;
    
    // 방문 가능한 지역 찾기
    int availableRegions[MAX_REGIONS];
    int availableCount = 0;
    
    for (int i = 0; i < regions[currentIndex].connectedCount; i++) {
        for (int j = 0; j < MAX_REGIONS; j++) {
            if (strcmp(regions[currentIndex].connected[i], regions[j].name) == 0) {
                if (!regions[j].visited) {  // 방문하지 않은 지역만 추가
                    availableRegions[availableCount++] = j;
                }
                break;
            }
        }
    }
    
    if (availableCount == 0) {
        // 방문하지 않은 지역 찾기
        int unvisitedRegions[MAX_REGIONS];
        int unvisitedCount = 0;
        
        for (int i = 0; i < MAX_REGIONS; i++) {
            if (!regions[i].visited) {
                unvisitedRegions[unvisitedCount++] = i;
            }
        }
        
        if (unvisitedCount > 0) {
            printText("\n더 이상 이동할 수 있는 지역이 없습니다.\n");
            printText("방문하지 않은 랜덤한 지역으로 이동합니다.\n");
            fastSleep(1000);
            
            // 랜덤으로 방문하지 않은 지역 선택
            int nextIndex = unvisitedRegions[rand() % unvisitedCount];
            strcpy(currentRegion, regions[nextIndex].name);
            regions[nextIndex].visited = 1;
            return 1;
        }
        return 0;
    }
    
    // 랜덤으로 다음 지역 선택
    int nextIndex = availableRegions[rand() % availableCount];
    strcpy(currentRegion, regions[nextIndex].name);
    regions[nextIndex].visited = 1;
    
    return 1;
}

// 지도 아이템 사용 시 플레이어가 직접 지역을 선택할 수 있는 함수
int moveToNextRegionWithMap(void) {
    LOG_FUNCTION_EXECUTION("moveToNextRegionWithMap");
    int currentIndex = -1;
    for (int i = 0; i < MAX_REGIONS; i++) {
        if (strcmp(regions[i].name, currentRegion) == 0) {
            currentIndex = i;
            break;
        }
    }
    if (currentIndex == -1) return 0;
    
    // 인접한 지역 중 방문하지 않은 지역 찾기
    int availableRegions[MAX_REGIONS];
    int availableCount = 0;
    
    // 현재 지역과 연결된 모든 지역 확인
    for (int i = 0; i < regions[currentIndex].connectedCount; i++) {
        const char* connectedRegion = regions[currentIndex].connected[i];
        for (int j = 0; j < MAX_REGIONS; j++) {
            if (strcmp(regions[j].name, connectedRegion) == 0) {
                if (!regions[j].visited) {  // 방문하지 않은 지역만 추가
                    // 중복 체크
                    bool isDuplicate = false;
                    for (int k = 0; k < availableCount; k++) {
                        if (availableRegions[k] == j) {
                            isDuplicate = true;
                            break;
                        }
                    }
                    if (!isDuplicate) {
                        availableRegions[availableCount++] = j;
                    }
                }
                break;
            }
        }
    }
    
    if (availableCount == 0) {
        // 방문하지 않은 지역 찾기
        int unvisitedRegions[MAX_REGIONS];
        int unvisitedCount = 0;
        
        for (int i = 0; i < MAX_REGIONS; i++) {
            if (!regions[i].visited) {
                unvisitedRegions[unvisitedCount++] = i;
            }
        }
        
        if (unvisitedCount > 0) {
            printText("\n더 이상 이동할 수 있는 지역이 없습니다.\n");
            printText("방문하지 않은 지역 중 하나를 선택하세요:\n");
            
            for (int i = 0; i < unvisitedCount; i++) {
                char buffer[64];
                sprintf(buffer, "%d. %s\n", i+1, regions[unvisitedRegions[i]].name);
                printTextAndWait(buffer);
            }
            
            printText("선택 (번호): ");
            int choice = getIntInput() - 1;
            while (choice < 0 || choice >= unvisitedCount) {
                printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.\n");
                printText("선택 (번호): ");
                choice = getIntInput() - 1;
            }
            
            int nextIndex = unvisitedRegions[choice];
            strcpy(currentRegion, regions[nextIndex].name);
            regions[nextIndex].visited = 1;
            return 1;
        }
        return 0;
    }
    
    printText("\n지도 아이템을 사용하여 다음 지역을 선택하세요:\n");
    for (int i = 0; i < availableCount; i++) {
        char buffer[64];
        sprintf(buffer, "%d. %s\n", i+1, regions[availableRegions[i]].name);
        printTextAndWait(buffer);
    }
    printText("선택 (번호): ");
    int choice = getIntInput() - 1;
    while (choice < 0 || choice >= availableCount) {
        printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.\n");
        printText("선택 (번호): ");
        choice = getIntInput() - 1;
    }
    int nextIndex = availableRegions[choice];
    strcpy(currentRegion, regions[nextIndex].name);
    regions[nextIndex].visited = 1;
    return 1;
}

void displayConnectedRegions(void) {
    LOG_FUNCTION_EXECUTION("displayConnectedRegions");
    int currentIndex = -1;
    for (int i = 0; i < MAX_REGIONS; i++) {
        if (strcmp(regions[i].name, currentRegion) == 0) {
            currentIndex = i;
            break;
        }
    }
    
    if (currentIndex == -1) return;
    
    // 연결된 지역 표시 (디버그 모드에서만)
    if (gameSettings.debugMode) {
        printText("\n연결된 지역:\n");
        for (int i = 0; i < regions[currentIndex].connectedCount; i++) {
            printText(regions[currentIndex].connected[i]);
            if (i < regions[currentIndex].connectedCount - 1) {
                printText(", ");
            }
        }
        fastSleep(500);
        printText("\n");
    }
}

void saveRegionData(FILE* file) {
    LOG_FUNCTION_EXECUTION("saveRegionData");
    if (!file) return;
    
    // 현재 지역 저장
    fwrite(currentRegion, sizeof(char), REGION_NAME_LENGTH, file);
    
    // 방문 기록 저장
    for (int i = 0; i < MAX_REGIONS; i++) {
        fwrite(&regions[i].visited, sizeof(int), 1, file);
    }
}

void loadRegionData(FILE* file) {
    LOG_FUNCTION_EXECUTION("loadRegionData");
    if (!file) return;
    
    // 현재 지역 로드
    fread(currentRegion, sizeof(char), REGION_NAME_LENGTH, file);
    
    // 방문 기록 로드
    for (int i = 0; i < MAX_REGIONS; i++) {
        fread(&regions[i].visited, sizeof(int), 1, file);
    }
}

// 모든 지역 방문 여부 확인
int isAllRegionsVisited(void) {
    LOG_FUNCTION_EXECUTION("isAllRegionsVisited");
    for (int i = 0; i < MAX_REGIONS; i++) {
        if (!regions[i].visited) return 0;
    }
    return 1;
} 