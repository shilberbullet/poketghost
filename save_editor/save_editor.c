#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h>

#define SAVE_FILE "../data/save.dat"
#define BACKUP_FILE "../data/save_backup.dat"
#define YOKAI_FILE "yokai.txt"
#define ITEMS_FILE "items.txt"
#define MOVES_FILE "move.txt"
#define PARTY_MAX 5
#define MOVE_MAX 4
#define INVENTORY_MAX 20
#define ITEM_NAME_MAX 32
#define ITEM_DESC_MAX 128
#define MAX_LEARNABLE_MOVES 12
#define YOKAI_LIST_MAX 100
#define ITEM_LIST_MAX 100
#define MOVE_LIST_MAX 100
#define REGION_COUNT 30

// 지형 이름
const char* terrainNames[] = {
    "산",
    "강",
    "바다",
    "논",
    "마을"
};

// 지역 이름
const char* regionNames[] = {
    // 1-10 스테이지
    "초보자의 숲", "초보자의 언덕", "초보자의 계곡", "초보자의 호수", "초보자의 들판",
    "초보자의 마을", "초보자의 길", "초보자의 동굴", "초보자의 강", "초보자의 산",
    
    // 11-20 스테이지
    "숙련자의 숲", "숙련자의 언덕", "숙련자의 계곡", "숙련자의 호수", "숙련자의 들판",
    "숙련자의 마을", "숙련자의 길", "숙련자의 동굴", "숙련자의 강", "숙련자의 산",
    
    // 21-30 스테이지
    "전문가의 숲", "전문가의 언덕", "전문가의 계곡", "전문가의 호수", "전문가의 들판",
    "전문가의 마을", "전문가의 길", "전문가의 동굴", "전문가의 강", "전문가의 산"
};

// 아이템 등급
typedef enum {
    ITEM_COMMON,    // 일반
    ITEM_RARE,      // 희귀
    ITEM_SUPERRARE  // 초희귀
} ItemGrade;

// 아이템 타입
typedef enum {
    ITEM_HEAL,      // 회복형
    ITEM_TALISMAN,  // 부적형
    ITEM_PLAYER,    // 플레이어 아이템
    ITEM_YOKAI      // 요괴 아이템
} ItemType;

// 요괴 타입
typedef enum {
    TYPE_EVIL_SPIRIT,  // 악귀
    TYPE_GHOST,        // 원귀
    TYPE_MONSTER,      // 괴수형
    TYPE_HUMAN,        // 인간형
    TYPE_ANIMAL        // 동물형
} YokaiType;

// 기술 등급
typedef enum {
    MOVE_BASIC,    // 초급
    MOVE_MEDIUM,   // 중급
    MOVE_ADVANCED  // 고급
} MoveGrade;

// 기술 구조체
typedef struct {
    char name[32];
    YokaiType type;
    int power;
    int accuracy;
    int pp;
    char description[256];
    MoveGrade grade;
} Move;

// 기술 인스턴스 구조체
typedef struct {
    Move move;
    int currentPP;
} MoveInstance;

// 요괴 구조체
typedef struct {
    char name[32];
    int level;
    int hp;
    int attack;
    int defense;
    int speed;
    YokaiType type;
    char desc[128];
    MoveInstance moves[MOVE_MAX];
    int moveCount;
    Move learnableMoves[MAX_LEARNABLE_MOVES];
    int learnableMoveCount;
} Yokai;

// 지형 타입
typedef enum {
    TERRAIN_MOUNTAIN,
    TERRAIN_RIVER,
    TERRAIN_SEA,
    TERRAIN_FIELD,
    TERRAIN_VILLAGE,
    TERRAIN_COUNT
} TerrainType;

// 스테이지 정보 구조체
typedef struct {
    int stageNumber;
    char region[32];
    TerrainType terrain;
    int hour;
} StageInfo;

// 플레이어 구조체
typedef struct {
    int money;
} Player;

// 아이템 구조체
typedef struct {
    char name[ITEM_NAME_MAX];
    char desc[ITEM_DESC_MAX];
    ItemGrade grade;
    ItemType type;
} Item;

// 인벤토리 아이템 구조체
typedef struct {
    Item item;
    int count;
} InventoryItem;

// 게임 저장 데이터 구조체
typedef struct {
    StageInfo currentStage;
    int partyCount;
    Yokai party[PARTY_MAX];
    Player player;
    int inventoryCount;
    InventoryItem inventory[INVENTORY_MAX];
    Yokai currentEnemy;
} SaveData;

// 요괴 목록
Yokai yokaiList[YOKAI_LIST_MAX];
int yokaiListCount = 0;

// 아이템 목록
Item itemList[ITEM_LIST_MAX];
int itemListCount = 0;

// 기술 목록
Move moveList[MOVE_LIST_MAX];
int moveListCount = 0;

// 함수 선언
void saveToFile(const SaveData* saveData);
void printParty(const SaveData* saveData);
void addYokai(SaveData* saveData);
void removeYokai(SaveData* saveData);
void modifyYokai(SaveData* saveData);
void manageParty(SaveData* saveData);
void printInventory(const SaveData* saveData);

// 요괴 타입을 문자열로 변환
const char* typeToString(YokaiType type) {
    switch (type) {
        case TYPE_MONSTER: return "괴수형";
        case TYPE_ANIMAL: return "동물형";
        case TYPE_GHOST: return "원귀";
        case TYPE_HUMAN: return "인간형";
        case TYPE_EVIL_SPIRIT: return "악귀";
        default: return "알 수 없음";
    }
}

// 메뉴 출력 함수
void printMenu() {
    system("cls");
    printf("=== 포켓고스트 세이브 에디터 ===\n\n");
    printf("1. 세이브 파일 읽기\n");
    printf("2. 스테이지 수정\n");
    printf("3. 돈 수정\n");
    printf("4. 동료 요괴 관리\n");
    printf("5. 인벤토리 관리\n");
    printf("6. 세이브 파일 백업\n");
    printf("7. 백업에서 복원\n");
    printf("8. 종료\n\n");
    printf("선택하세요 (1-8): ");
}

// 동료 요괴 관리 메뉴
void printPartyMenu() {
    system("cls");
    printf("=== 동료 요괴 관리 ===\n\n");
    printf("1. 동료 요괴 목록 보기\n");
    printf("2. 동료 요괴 추가\n");
    printf("3. 동료 요괴 제거\n");
    printf("4. 동료 요괴 수정\n");
    printf("5. 뒤로 가기\n\n");
    printf("선택하세요 (1-5): ");
}

// 동료 요괴 목록 출력
void printParty(const SaveData* saveData) {
    system("cls");
    printf("=== 동료 요괴 목록 ===\n\n");
    if (saveData->partyCount == 0) {
        printf("동료 요괴가 없습니다.\n");
    } else {
        for (int i = 0; i < saveData->partyCount; i++) {
            printf("%d. %s (Lv.%d, %s)\n", i+1, 
                saveData->party[i].name,
                saveData->party[i].level,
                typeToString(saveData->party[i].type));
            printf("   HP: %d, 공격: %d, 방어: %d, 속도: %d\n",
                saveData->party[i].hp,
                saveData->party[i].attack,
                saveData->party[i].defense,
                saveData->party[i].speed);
            printf("   기술:\n");
            for (int j = 0; j < saveData->party[i].moveCount; j++) {
                printf("   - %s (공격력: %d, 명중률: %d%%, PP: %d/%d)\n",
                    saveData->party[i].moves[j].move.name,
                    saveData->party[i].moves[j].move.power,
                    saveData->party[i].moves[j].move.accuracy,
                    saveData->party[i].moves[j].currentPP,
                    saveData->party[i].moves[j].move.pp);
            }
            printf("\n");
        }
    }
    system("pause");
}

// 요괴 데이터 파일 읽기
void loadYokaiFromFile() {
    char cwd[1024];
    _getcwd(cwd, sizeof(cwd));
    printf("[디버그] 현재 작업 디렉토리: %s\n", cwd);
    printf("[디버그] 요괴 데이터 파일 경로: %s\n", YOKAI_FILE);

    FILE* file = fopen(YOKAI_FILE, "r");
    if (!file) {
        printf("요괴 데이터 파일을 열 수 없습니다!\n");
        printf("경로: %s\n", YOKAI_FILE);
        system("pause");
        return;
    }

    // 파일의 첫 3줄 출력
    printf("[디버그] 파일 첫 3줄:\n");
    char debug_line[512];
    for (int i = 0; i < 3; i++) {
        if (fgets(debug_line, sizeof(debug_line), file)) {
            printf("%s", debug_line);
        }
    }
    rewind(file);

    char line[512];
    yokaiListCount = 0;

    while (fgets(line, sizeof(line), file) && yokaiListCount < YOKAI_LIST_MAX) {
        // 주석이나 빈 줄 무시
        if (line[0] == '#' || line[0] == '\n') continue;

        Yokai* yokai = &yokaiList[yokaiListCount];
        char typeStr[32], skills[256];

        if (sscanf(line, "%[^,],%[^,],%d,%d,%d,%d,%[^,],%[^\n]",
            yokai->name,
            typeStr,
            &yokai->attack,
            &yokai->defense,
            &yokai->hp,
            &yokai->speed,
            yokai->desc,
            skills) == 8) {

            // 타입 문자열을 enum으로 변환
            if (strcmp(typeStr, "악귀") == 0) yokai->type = TYPE_EVIL_SPIRIT;
            else if (strcmp(typeStr, "원귀") == 0) yokai->type = TYPE_GHOST;
            else if (strcmp(typeStr, "괴수형") == 0) yokai->type = TYPE_MONSTER;
            else if (strcmp(typeStr, "인간형") == 0) yokai->type = TYPE_HUMAN;
            else if (strcmp(typeStr, "동물형") == 0) yokai->type = TYPE_ANIMAL;

            // 기술 목록 파싱
            yokai->learnableMoveCount = 0;
            char* token = strtok(skills, ";");
            while (token && yokai->learnableMoveCount < MAX_LEARNABLE_MOVES) {
                strncpy(yokai->learnableMoves[yokai->learnableMoveCount].name, token, 31);
                yokai->learnableMoves[yokai->learnableMoveCount].name[31] = '\0';
                yokai->learnableMoveCount++;
                token = strtok(NULL, ";");
            }

            yokai->moveCount = 0;
            yokaiListCount++;
        }
    }
    fclose(file);

    if (yokaiListCount == 0) {
        printf("요괴 데이터를 읽을 수 없습니다!\n");
        printf("yokai.txt 파일이 올바른 형식인지 확인해주세요.\n");
        system("pause");
    } else {
        printf("%d개의 요괴 데이터를 읽었습니다.\n", yokaiListCount);
    }
}

// 아이템 데이터 파일 읽기
void loadItemsFromFile() {
    FILE* file = fopen(ITEMS_FILE, "r");
    if (!file) {
        printf("아이템 데이터 파일을 열 수 없습니다!\n");
        printf("경로: %s\n", ITEMS_FILE);
        system("pause");
        return;
    }

    char line[512];
    itemListCount = 0;

    while (fgets(line, sizeof(line), file) && itemListCount < ITEM_LIST_MAX) {
        Item* item = &itemList[itemListCount];
        char gradeStr[32], typeStr[32];
        
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^\n]",
            item->name,
            gradeStr,
            typeStr,
            item->desc) == 4) {
            
            // 등급 문자열을 enum으로 변환
            if (strcmp(gradeStr, "일반") == 0) item->grade = ITEM_COMMON;
            else if (strcmp(gradeStr, "희귀") == 0) item->grade = ITEM_RARE;
            else if (strcmp(gradeStr, "초희귀") == 0) item->grade = ITEM_SUPERRARE;
            
            // 타입 문자열을 enum으로 변환
            if (strcmp(typeStr, "회복형") == 0) item->type = ITEM_HEAL;
            else if (strcmp(typeStr, "부적형") == 0) item->type = ITEM_TALISMAN;
            else if (strcmp(typeStr, "플레이어") == 0) item->type = ITEM_PLAYER;
            else if (strcmp(typeStr, "요괴") == 0) item->type = ITEM_YOKAI;
            
            itemListCount++;
        }
    }
    fclose(file);
}

// 기술 데이터 파일 읽기
void loadMovesFromFile() {
    FILE* file = fopen(MOVES_FILE, "r");
    if (!file) {
        printf("기술 데이터 파일을 열 수 없습니다!\n");
        printf("경로: %s\n", MOVES_FILE);
        system("pause");
        return;
    }

    char line[512];
    moveListCount = 0;

    while (fgets(line, sizeof(line), file) && moveListCount < MOVE_LIST_MAX) {
        // 주석이나 빈 줄 무시
        if (line[0] == '#' || line[0] == '\n') continue;

        Move* move = &moveList[moveListCount];
        char typeStr[32];
        
        if (sscanf(line, "%[^,],%[^,],%d,%d,%d,%[^\n]",
            move->name,
            typeStr,
            &move->power,
            &move->accuracy,
            &move->pp,
            move->description) == 6) {
            
            // 타입 문자열을 enum으로 변환
            if (strcmp(typeStr, "EVIL_SPIRIT") == 0) move->type = TYPE_EVIL_SPIRIT;
            else if (strcmp(typeStr, "GHOST") == 0) move->type = TYPE_GHOST;
            else if (strcmp(typeStr, "MONSTER") == 0) move->type = TYPE_MONSTER;
            else if (strcmp(typeStr, "HUMAN") == 0) move->type = TYPE_HUMAN;
            else if (strcmp(typeStr, "ANIMAL") == 0) move->type = TYPE_ANIMAL;
            
            // 기술 등급 설정 (PP 기준)
            if (move->pp <= 5) move->grade = MOVE_ADVANCED;
            else if (move->pp <= 15) move->grade = MOVE_MEDIUM;
            else move->grade = MOVE_BASIC;
            
            moveListCount++;
        }
    }
    fclose(file);

    if (moveListCount == 0) {
        printf("기술 데이터를 읽을 수 없습니다!\n");
        printf("move.txt 파일이 올바른 형식인지 확인해주세요.\n");
        system("pause");
    } else {
        printf("%d개의 기술 데이터를 읽었습니다.\n", moveListCount);
    }
}

// 요괴 선택 메뉴
int selectYokai() {
    if (yokaiListCount == 0) {
        printf("\n요괴 데이터가 없습니다!\n");
        printf("yokai.txt 파일을 확인해주세요.\n");
        system("pause");
        return -1;
    }

    system("cls");
    printf("=== 요괴 선택 ===\n\n");
    for (int i = 0; i < yokaiListCount; i++) {
        printf("%d. %s (%s)\n", i+1, 
            yokaiList[i].name,
            typeToString(yokaiList[i].type));
        printf("   HP: %d, 공격: %d, 방어: %d, 속도: %d\n",
            yokaiList[i].hp,
            yokaiList[i].attack,
            yokaiList[i].defense,
            yokaiList[i].speed);
    }
    printf("\n선택하세요 (1-%d): ", yokaiListCount);
    int choice;
    scanf("%d", &choice);
    if (choice < 1 || choice > yokaiListCount) return -1;
    return choice - 1;
}

// 아이템 선택 메뉴
int selectItem() {
    system("cls");
    printf("=== 아이템 선택 ===\n\n");
    int validItemCount = 0;
    for (int i = 0; i < itemListCount; i++) {
        // 주석이나 형식 설명 건너뛰기
        if (itemList[i].name[0] == '#' || strstr(itemList[i].name, "형식:") != NULL) {
            continue;
        }
        validItemCount++;
        const char* gradeStr;
        switch (itemList[i].grade) {
            case ITEM_COMMON: gradeStr = "일반"; break;
            case ITEM_RARE: gradeStr = "희귀"; break;
            case ITEM_SUPERRARE: gradeStr = "초희귀"; break;
            default: gradeStr = "알 수 없음";
        }
        
        const char* typeStr;
        switch (itemList[i].type) {
            case ITEM_HEAL: typeStr = "회복형"; break;
            case ITEM_TALISMAN: typeStr = "부적형"; break;
            case ITEM_PLAYER: typeStr = "플레이어"; break;
            case ITEM_YOKAI: typeStr = "요괴"; break;
            default: typeStr = "알 수 없음";
        }
        
        printf("%d. %s (등급: %s, 타입: %s)\n", validItemCount, 
            itemList[i].name,
            gradeStr,
            typeStr);
        printf("   %s\n", itemList[i].desc);
    }
    
    if (validItemCount == 0) {
        printf("\n선택할 수 있는 아이템이 없습니다!\n");
        system("pause");
        return -1;
    }
    
    printf("\n선택하세요 (1-%d): ", validItemCount);
    int choice;
    scanf("%d", &choice);
    if (choice < 1 || choice > validItemCount) return -1;
    
    // 선택한 번호를 실제 아이템 인덱스로 변환
    int actualIndex = -1;
    int count = 0;
    for (int i = 0; i < itemListCount; i++) {
        if (itemList[i].name[0] == '#' || strstr(itemList[i].name, "형식:") != NULL) {
            continue;
        }
        count++;
        if (count == choice) {
            actualIndex = i;
            break;
        }
    }
    
    return actualIndex;
}

// 기술 선택 메뉴
int selectMove() {
    if (moveListCount == 0) {
        printf("\n기술 데이터가 없습니다!\n");
        printf("move.txt 파일을 확인해주세요.\n");
        system("pause");
        return -1;
    }

    system("cls");
    printf("=== 기술 선택 ===\n\n");
    for (int i = 0; i < moveListCount; i++) {
        printf("%d. %s (%s, 위력: %d, 명중률: %d%%, PP: %d)\n", i+1, 
            moveList[i].name,
            typeToString(moveList[i].type),
            moveList[i].power,
            moveList[i].accuracy,
            moveList[i].pp);
        printf("   %s\n", moveList[i].description);
    }
    printf("\n선택하세요 (1-%d): ", moveListCount);
    int choice;
    scanf("%d", &choice);
    if (choice < 1 || choice > moveListCount) return -1;
    return choice - 1;
}

// 동료 요괴 추가 함수 수정
void addYokai(SaveData* saveData) {
    if (saveData->partyCount >= PARTY_MAX) {
        printf("\n동료 요괴는 최대 %d마리까지만 가질 수 있습니다!\n", PARTY_MAX);
        system("pause");
        return;
    }

    if (yokaiListCount == 0) {
        printf("\n요괴 데이터가 없습니다!\n");
        printf("yokai.txt 파일을 확인해주세요.\n");
        system("pause");
        return;
    }

    int yokaiIdx = selectYokai();
    if (yokaiIdx == -1) return;

    Yokai* yokai = &saveData->party[saveData->partyCount];
    *yokai = yokaiList[yokaiIdx];  // 선택한 요괴 정보 복사
    
    // 기술 선택
    printf("\n기술을 선택하세요 (최대 %d개):\n", MOVE_MAX);
    for (int i = 0; i < MOVE_MAX; i++) {
        int moveIdx = selectMove();
        if (moveIdx == -1) break;
        
        yokai->moves[i].move = moveList[moveIdx];
        yokai->moves[i].currentPP = moveList[moveIdx].pp;
        yokai->moveCount++;
    }
    
    saveData->partyCount++;
    printf("\n동료 요괴가 추가되었습니다!\n");
    system("pause");
}

// 동료 요괴 제거
void removeYokai(SaveData* saveData) {
    if (saveData->partyCount == 0) {
        printf("\n제거할 동료 요괴가 없습니다!\n");
        system("pause");
        return;
    }
    
    printParty(saveData);
    printf("\n제거할 동료 요괴 번호를 입력하세요 (1-%d): ", saveData->partyCount);
    int idx;
    scanf("%d", &idx);
    idx--;
    
    if (idx < 0 || idx >= saveData->partyCount) {
        printf("\n잘못된 번호입니다!\n");
        system("pause");
        return;
    }
    
    // 선택한 요괴를 제거하고 나머지를 앞으로 이동
    for (int i = idx; i < saveData->partyCount - 1; i++) {
        saveData->party[i] = saveData->party[i + 1];
    }
    saveData->partyCount--;
    
    printf("\n동료 요괴가 제거되었습니다!\n");
    system("pause");
}

// 동료 요괴 수정
void modifyYokai(SaveData* saveData) {
    if (saveData->partyCount == 0) {
        printf("\n수정할 동료 요괴가 없습니다!\n");
        system("pause");
        return;
    }
    
    printParty(saveData);
    printf("\n수정할 동료 요괴 번호를 입력하세요 (1-%d): ", saveData->partyCount);
    int idx;
    scanf("%d", &idx);
    idx--;
    
    if (idx < 0 || idx >= saveData->partyCount) {
        printf("\n잘못된 번호입니다!\n");
        system("pause");
        return;
    }
    
    Yokai* yokai = &saveData->party[idx];
    
    printf("\n=== 동료 요괴 수정 ===\n");
    printf("현재 요괴: %s (Lv.%d)\n", yokai->name, yokai->level);
    
    // 레벨 수정
    printf("\n새로운 레벨을 입력하세요 [%d]: ", yokai->level);
    int value;
    scanf("%d", &value);
    if (value > 0) yokai->level = value;
    
    // 기술 수정
    printf("\n기술을 수정하시겠습니까? (1:예, 0:아니오): ");
    scanf("%d", &value);
    if (value == 1) {
        printf("\n기술 개수 (최대 %d) [%d]: ", MOVE_MAX, yokai->moveCount);
        scanf("%d", &value);
        if (value > 0 && value <= MOVE_MAX) yokai->moveCount = value;
        
        for (int i = 0; i < yokai->moveCount; i++) {
            printf("\n기술 %d:\n", i+1);
            int moveIdx = selectMove();
            if (moveIdx == -1) break;
            
            yokai->moves[i].move = moveList[moveIdx];
            yokai->moves[i].currentPP = moveList[moveIdx].pp;
        }
    }
    
    printf("\n동료 요괴가 수정되었습니다!\n");
    system("pause");
}

// 동료 요괴 관리
void manageParty(SaveData* saveData) {
    int choice;
    while (1) {
        printPartyMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printParty(saveData);
                break;
            case 2:
                addYokai(saveData);
                saveToFile(saveData);
                break;
            case 3:
                removeYokai(saveData);
                saveToFile(saveData);
                break;
            case 4:
                modifyYokai(saveData);
                saveToFile(saveData);
                break;
            case 5:
                return;
            default:
                printf("\n잘못된 선택입니다!\n");
                system("pause");
                break;
        }
    }
}

// 인벤토리 관리 메뉴
void printInventoryMenu() {
    system("cls");
    printf("=== 인벤토리 관리 ===\n\n");
    printf("1. 인벤토리 목록 보기\n");
    printf("2. 아이템 추가\n");
    printf("3. 아이템 제거\n");
    printf("4. 아이템 수량 수정\n");
    printf("5. 뒤로 가기\n\n");
    printf("선택하세요 (1-5): ");
}

// 인벤토리 출력
void printInventory(const SaveData* saveData) {
    system("cls");
    printf("=== 인벤토리 목록 ===\n\n");
    if (saveData->inventoryCount == 0) {
        printf("인벤토리가 비어있습니다.\n");
    } else {
        for (int i = 0; i < saveData->inventoryCount; i++) {
            const char* gradeStr;
            switch (saveData->inventory[i].item.grade) {
                case ITEM_COMMON: gradeStr = "일반"; break;
                case ITEM_RARE: gradeStr = "희귀"; break;
                case ITEM_SUPERRARE: gradeStr = "초희귀"; break;
                default: gradeStr = "알 수 없음";
            }
            
            const char* typeStr;
            switch (saveData->inventory[i].item.type) {
                case ITEM_HEAL: typeStr = "회복형"; break;
                case ITEM_TALISMAN: typeStr = "부적형"; break;
                case ITEM_PLAYER: typeStr = "플레이어"; break;
                case ITEM_YOKAI: typeStr = "요괴"; break;
                default: typeStr = "알 수 없음";
            }
            
            printf("%d. %s (등급: %s, 타입: %s) x%d\n", i+1, 
                saveData->inventory[i].item.name,
                gradeStr,
                typeStr,
                saveData->inventory[i].count);
            printf("   %s\n", saveData->inventory[i].item.desc);
        }
    }
    system("pause");
}

// 아이템 추가
void addItem(SaveData* saveData) {
    if (saveData->inventoryCount >= INVENTORY_MAX) {
        printf("\n인벤토리가 가득 찼습니다!\n");
        system("pause");
        return;
    }

    if (itemListCount == 0) {
        printf("\n아이템 데이터가 없습니다!\n");
        printf("items.txt 파일을 확인해주세요.\n");
        system("pause");
        return;
    }

    int itemIdx = selectItem();
    if (itemIdx == -1) return;

    printf("\n수량을 입력하세요: ");
    int count;
    scanf("%d", &count);
    if (count <= 0) {
        printf("\n잘못된 수량입니다!\n");
        system("pause");
        return;
    }

    // 이미 있는 아이템인지 확인
    for (int i = 0; i < saveData->inventoryCount; i++) {
        if (strcmp(saveData->inventory[i].item.name, itemList[itemIdx].name) == 0) {
            saveData->inventory[i].count += count;
            printf("\n아이템 수량이 증가했습니다!\n");
            system("pause");
            return;
        }
    }

    // 새로운 아이템 추가
    saveData->inventory[saveData->inventoryCount].item = itemList[itemIdx];
    saveData->inventory[saveData->inventoryCount].count = count;
    saveData->inventoryCount++;
    
    printf("\n아이템이 추가되었습니다!\n");
    system("pause");
}

// 아이템 제거
void removeItem(SaveData* saveData) {
    if (saveData->inventoryCount == 0) {
        printf("\n제거할 아이템이 없습니다!\n");
        system("pause");
        return;
    }
    
    printInventory(saveData);
    printf("\n제거할 아이템 번호를 입력하세요 (1-%d): ", saveData->inventoryCount);
    int idx;
    scanf("%d", &idx);
    idx--;
    
    if (idx < 0 || idx >= saveData->inventoryCount) {
        printf("\n잘못된 번호입니다!\n");
        system("pause");
        return;
    }
    
    // 선택한 아이템을 제거하고 나머지를 앞으로 이동
    for (int i = idx; i < saveData->inventoryCount - 1; i++) {
        saveData->inventory[i] = saveData->inventory[i + 1];
    }
    saveData->inventoryCount--;
    
    printf("\n아이템이 제거되었습니다!\n");
    system("pause");
}

// 아이템 수량 수정
void modifyItemCount(SaveData* saveData) {
    if (saveData->inventoryCount == 0) {
        printf("\n수정할 아이템이 없습니다!\n");
        system("pause");
        return;
    }
    
    printInventory(saveData);
    printf("\n수정할 아이템 번호를 입력하세요 (1-%d): ", saveData->inventoryCount);
    int idx;
    scanf("%d", &idx);
    idx--;
    
    if (idx < 0 || idx >= saveData->inventoryCount) {
        printf("\n잘못된 번호입니다!\n");
        system("pause");
        return;
    }
    
    printf("\n새로운 수량을 입력하세요 [%d]: ", saveData->inventory[idx].count);
    int count;
    scanf("%d", &count);
    
    if (count <= 0) {
        printf("\n잘못된 수량입니다!\n");
        system("pause");
        return;
    }
    
    saveData->inventory[idx].count = count;
    printf("\n아이템 수량이 수정되었습니다!\n");
    system("pause");
}

// 인벤토리 관리
void manageInventory(SaveData* saveData) {
    int choice;
    while (1) {
        printInventoryMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printInventory(saveData);
                break;
            case 2:
                addItem(saveData);
                saveToFile(saveData);
                break;
            case 3:
                removeItem(saveData);
                saveToFile(saveData);
                break;
            case 4:
                modifyItemCount(saveData);
                saveToFile(saveData);
                break;
            case 5:
                return;
            default:
                printf("\n잘못된 선택입니다!\n");
                system("pause");
                break;
        }
    }
}

// 세이브 파일 읽기 함수
void readSaveFile(SaveData* saveData) {
    FILE* file = fopen(SAVE_FILE, "rb");
    if (file == NULL) {
        printf("\n세이브 파일을 찾을 수 없습니다!\n");
        printf("현재 경로: %s\n", SAVE_FILE);
        system("pause");
        return;
    }
    
    // 모든 데이터를 0으로 초기화
    memset(saveData, 0, sizeof(SaveData));
    
    // 스테이지 정보 불러오기
    if (fread(&saveData->currentStage, sizeof(StageInfo), 1, file) != 1) {
        printf("\n스테이지 정보를 읽을 수 없습니다!\n");
        fclose(file);
        return;
    }
    
    // 동료 요괴 수 불러오기
    if (fread(&saveData->partyCount, sizeof(int), 1, file) != 1) {
        printf("\n동료 요괴 수를 읽을 수 없습니다!\n");
        fclose(file);
        return;
    }
    
    // 동료 요괴 정보 불러오기
    for (int i = 0; i < saveData->partyCount && i < PARTY_MAX; i++) {
        if (fread(&saveData->party[i], sizeof(Yokai), 1, file) != 1) {
            printf("\n동료 요괴 정보를 읽을 수 없습니다!\n");
            fclose(file);
            return;
        }
        // 각 요괴의 기술 currentPP 불러오기
        for (int j = 0; j < saveData->party[i].moveCount && j < MOVE_MAX; j++) {
            if (fread(&saveData->party[i].moves[j].currentPP, sizeof(int), 1, file) != 1) {
                printf("\n기술 PP를 읽을 수 없습니다!\n");
                fclose(file);
                return;
            }
        }
    }
    
    // 플레이어 정보 불러오기
    if (fread(&saveData->player, sizeof(Player), 1, file) != 1) {
        printf("\n플레이어 정보를 읽을 수 없습니다!\n");
        fclose(file);
        return;
    }
    
    // 인벤토리 개수 불러오기
    if (fread(&saveData->inventoryCount, sizeof(int), 1, file) != 1) {
        printf("\n인벤토리 개수를 읽을 수 없습니다!\n");
        fclose(file);
        return;
    }
    
    // 인벤토리 아이템 정보 불러오기
    for (int i = 0; i < saveData->inventoryCount && i < INVENTORY_MAX; i++) {
        if (fread(&saveData->inventory[i], sizeof(InventoryItem), 1, file) != 1) {
            printf("\n인벤토리 아이템 정보를 읽을 수 없습니다!\n");
            fclose(file);
            return;
        }
    }
    
    // 현재 전투 중인 상대 요괴 정보 불러오기
    if (fread(&saveData->currentEnemy, sizeof(Yokai), 1, file) != 1) {
        printf("\n상대 요괴 정보를 읽을 수 없습니다!\n");
        fclose(file);
        return;
    }
    
    fclose(file);
    
    printf("\n=== 현재 세이브 데이터 ===\n");
    printf("현재 스테이지: %d\n", saveData->currentStage.stageNumber);
    printf("지역: %s\n", saveData->currentStage.region);
    printf("지형: %s\n", terrainNames[saveData->currentStage.terrain]);
    printf("시간: %d\n", saveData->currentStage.hour);
    printf("보유 금액: %d\n", saveData->player.money);
    printf("동료 요괴 수: %d\n", saveData->partyCount);
    printf("인벤토리 아이템 수: %d\n\n", saveData->inventoryCount);
    system("pause");
}

// 스테이지 수정 함수
void modifyStage(SaveData* saveData) {
    int choice;
    while (1) {
        system("cls");
        printf("=== 스테이지 수정 ===\n\n");
        printf("현재 스테이지: %d\n", saveData->currentStage.stageNumber);
        printf("현재 지역: %s\n", saveData->currentStage.region);
        printf("현재 지형: %s\n", terrainNames[saveData->currentStage.terrain]);
        printf("현재 시간: %d\n\n", saveData->currentStage.hour);
        
        printf("1. 스테이지 번호 수정\n");
        printf("2. 지역 수정\n");
        printf("3. 지형 수정\n");
        printf("4. 뒤로 가기\n\n");
        printf("선택하세요 (1-4): ");
        
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                printf("\n새로운 스테이지 번호를 입력하세요: ");
                int value;
                scanf("%d", &value);
                if (value > 0) {
                    saveData->currentStage.stageNumber = value;
                    // 스테이지 번호에 따라 시간 자동 계산 (0-23)
                    saveData->currentStage.hour = value % 24;
                    // 스테이지 번호에 따라 지역 자동 설정
                    int regionIndex = (value - 1) % REGION_COUNT;
                    strcpy(saveData->currentStage.region, regionNames[regionIndex]);
                }
                break;
            }
            case 2: {
                printf("\n지역을 선택하세요:\n");
                for (int i = 0; i < REGION_COUNT; i++) {
                    printf("%d: %s\n", i + 1, regionNames[i]);
                }
                int value;
                scanf("%d", &value);
                if (value >= 1 && value <= REGION_COUNT) {
                    strcpy(saveData->currentStage.region, regionNames[value - 1]);
                }
                break;
            }
            case 3: {
                printf("\n지형을 선택하세요:\n");
                printf("0: 산\n");
                printf("1: 강\n");
                printf("2: 바다\n");
                printf("3: 논\n");
                printf("4: 마을\n");
                int value;
                scanf("%d", &value);
                if (value >= 0 && value < TERRAIN_COUNT) {
                    saveData->currentStage.terrain = (TerrainType)value;
                }
                break;
            }
            case 4:
                return;
            default:
                printf("\n잘못된 선택입니다!\n");
                system("pause");
                break;
        }
        
        printf("\n스테이지가 수정되었습니다.\n");
        system("pause");
    }
}

// 돈 수정 함수
void modifyMoney(SaveData* saveData) {
    int newMoney;
    printf("\n새로운 금액을 입력하세요: ");
    scanf("%d", &newMoney);
    
    if (newMoney < 0) {
        printf("\n잘못된 금액입니다!\n");
        system("pause");
        return;
    }
    
    saveData->player.money = newMoney;
    printf("\n금액이 %d로 변경되었습니다.\n", newMoney);
    system("pause");
}

// 세이브 파일 저장 함수
void saveToFile(const SaveData* saveData) {
    FILE* file = fopen(SAVE_FILE, "wb");
    if (file == NULL) {
        printf("\n세이브 파일을 저장할 수 없습니다!\n");
        return;
    }
    
    // 스테이지 정보 저장
    if (fwrite(&saveData->currentStage, sizeof(StageInfo), 1, file) != 1) {
        printf("\n스테이지 정보를 저장할 수 없습니다!\n");
        fclose(file);
        return;
    }
    
    // 동료 요괴 수 저장
    if (fwrite(&saveData->partyCount, sizeof(int), 1, file) != 1) {
        printf("\n동료 요괴 수를 저장할 수 없습니다!\n");
        fclose(file);
        return;
    }
    
    // 동료 요괴 정보 저장
    for (int i = 0; i < saveData->partyCount && i < PARTY_MAX; i++) {
        if (fwrite(&saveData->party[i], sizeof(Yokai), 1, file) != 1) {
            printf("\n동료 요괴 정보를 저장할 수 없습니다!\n");
            fclose(file);
            return;
        }
        // 각 요괴의 기술 currentPP 저장
        for (int j = 0; j < saveData->party[i].moveCount && j < MOVE_MAX; j++) {
            if (fwrite(&saveData->party[i].moves[j].currentPP, sizeof(int), 1, file) != 1) {
                printf("\n기술 PP를 저장할 수 없습니다!\n");
                fclose(file);
                return;
            }
        }
    }
    
    // 플레이어 정보 저장
    if (fwrite(&saveData->player, sizeof(Player), 1, file) != 1) {
        printf("\n플레이어 정보를 저장할 수 없습니다!\n");
        fclose(file);
        return;
    }
    
    // 인벤토리 개수 저장
    if (fwrite(&saveData->inventoryCount, sizeof(int), 1, file) != 1) {
        printf("\n인벤토리 개수를 저장할 수 없습니다!\n");
        fclose(file);
        return;
    }
    
    // 인벤토리 아이템 정보 저장
    for (int i = 0; i < saveData->inventoryCount && i < INVENTORY_MAX; i++) {
        if (fwrite(&saveData->inventory[i], sizeof(InventoryItem), 1, file) != 1) {
            printf("\n인벤토리 아이템 정보를 저장할 수 없습니다!\n");
            fclose(file);
            return;
        }
    }
    
    // 현재 전투 중인 상대 요괴 정보 저장
    if (fwrite(&saveData->currentEnemy, sizeof(Yokai), 1, file) != 1) {
        printf("\n상대 요괴 정보를 저장할 수 없습니다!\n");
        fclose(file);
        return;
    }
    
    fclose(file);
    printf("\n세이브 파일이 저장되었습니다.\n");
}

// 백업 파일 생성 함수
void createBackup() {
    if (CopyFile(SAVE_FILE, BACKUP_FILE, FALSE)) {
        printf("\n백업 파일이 생성되었습니다.\n");
    } else {
        printf("\n백업 파일 생성에 실패했습니다!\n");
    }
    system("pause");
}

// 백업에서 복원 함수
void restoreFromBackup() {
    if (CopyFile(BACKUP_FILE, SAVE_FILE, FALSE)) {
        printf("\n백업에서 복원되었습니다.\n");
    } else {
        printf("\n백업 복원에 실패했습니다!\n");
    }
    system("pause");
}

int main() {
    SaveData saveData = {0};  // 모든 필드를 0으로 초기화
    int choice;
    
    // 한글 출력을 위한 설정
    SetConsoleOutputCP(CP_UTF8);
    
    // 데이터 파일 로드
    loadYokaiFromFile();
    loadItemsFromFile();
    loadMovesFromFile();
    
    while (1) {
        printMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                readSaveFile(&saveData);
                break;
            case 2:
                modifyStage(&saveData);
                saveToFile(&saveData);
                break;
            case 3:
                modifyMoney(&saveData);
                saveToFile(&saveData);
                break;
            case 4:
                manageParty(&saveData);
                break;
            case 5:
                manageInventory(&saveData);
                break;
            case 6:
                createBackup();
                break;
            case 7:
                restoreFromBackup();
                break;
            case 8:
                printf("\n프로그램을 종료합니다.\n");
                return 0;
            default:
                printf("\n잘못된 선택입니다!\n");
                system("pause");
                break;
        }
    }
    
    return 0;
} 