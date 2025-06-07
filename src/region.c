#include "../include/region.h"
#include "../include/text.h"

// 지역 데이터
static Region regions[MAX_REGIONS] = {
    {"함경도", 0, 2, {"평안도", "강원도"}},
    {"평안도", 0, 2, {"함경도", "황해도"}},
    {"황해도", 0, 3, {"평안도", "경기도", "강원도"}},
    {"강원도", 0, 4, {"함경도", "황해도", "경기도", "경상도"}},
    {"경기도", 0, 3, {"황해도", "강원도", "충청도"}},
    {"충청도", 0, 3, {"경기도", "경상도", "전라도"}},
    {"경상도", 0, 3, {"강원도", "충청도", "전라도"}},
    {"전라도", 0, 2, {"충청도", "경상도"}}
};

static char currentRegion[REGION_NAME_LENGTH];

void initRegionSystem(void) {
    srand((unsigned int)time(NULL));
}

const char* getCurrentRegion(void) {
    return currentRegion;
}

int setInitialRegion(int choice) {
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
                if (!regions[j].visited) {
                    availableRegions[availableCount++] = j;
                }
                break;
            }
        }
    }
    
    if (availableCount == 0) return 0;
    
    // 랜덤으로 다음 지역 선택
    int nextIndex = availableRegions[rand() % availableCount];
    strcpy(currentRegion, regions[nextIndex].name);
    regions[nextIndex].visited = 1;
    
    return 1;
}

void displayConnectedRegions(void) {
    int currentIndex = -1;
    for (int i = 0; i < MAX_REGIONS; i++) {
        if (strcmp(regions[i].name, currentRegion) == 0) {
            currentIndex = i;
            break;
        }
    }
    
    if (currentIndex == -1) return;
    
    printText("\n연결된 지역:\n");
    for (int i = 0; i < regions[currentIndex].connectedCount; i++) {
        printText(regions[currentIndex].connected[i]);
        if (i < regions[currentIndex].connectedCount - 1) {
            printText(", ");
        }
    }
    printText("\n");
}

void saveRegionData(FILE* file) {
    if (!file) return;
    
    // 현재 지역 저장
    fwrite(currentRegion, sizeof(char), REGION_NAME_LENGTH, file);
    
    // 방문 기록 저장
    for (int i = 0; i < MAX_REGIONS; i++) {
        fwrite(&regions[i].visited, sizeof(int), 1, file);
    }
}

void loadRegionData(FILE* file) {
    if (!file) return;
    
    // 현재 지역 로드
    fread(currentRegion, sizeof(char), REGION_NAME_LENGTH, file);
    
    // 방문 기록 로드
    for (int i = 0; i < MAX_REGIONS; i++) {
        fread(&regions[i].visited, sizeof(int), 1, file);
    }
} 