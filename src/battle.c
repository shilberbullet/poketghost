#include <stdio.h> // 표준 입출력 함수 사용
#include <stdlib.h> // 표준 라이브러리 함수 사용
#include <string.h> // 문자열 처리 함수 사용
#include <time.h> // 시간 관련 함수 사용
#include "battle.h" // 전투 시스템 헤더
#include "yokai.h"  // Yokai 구조체 정의를 위해 추가
#include "text.h" // 텍스트 출력 함수
#include "input.h" // 입력 처리 함수
#include "stage.h" // 스테이지 관련 함수
#include "party.h" // 파티 관리 함수
#include "savefile.h" // 저장 파일 처리 함수
#include "menu.h" // 메뉴 시스템
#include "game.h" // 게임 상태 관리
#include "item.h" // 아이템 시스템
#include "move.h" // 기술 시스템
#include "reward.h"  // 새로 추가
#include "escape.h"  // 도망치기 시스템 추가
#include "hp_system.h"  // HP 시스템 헤더 추가
#include "exp_system.h"  // 경험치 시스템 헤더 추가
#include "battle_system.h"  // 새로 추가
#include "encyclopedia.h"  // 도감 시스템 추가
#include "../core/state.h" // 게임 상태 관리
#include "roguelite.h"  // 로그라이트 시스템 추가
#include "settings.h" // 설정 시스템
#include "region.h" // 지역 시스템
#include "logger.h" // 로그 시스템
#include "statistics.h" // 통계 시스템
#include <windows.h> // Windows API 사용
#include "dialogue.h" // 대화 시스템
#define MAX_PARTY 6  // 최대 파티 요괴 수

// 현재 전투 중인 상대 요괴
Yokai currentEnemy; // 전투 중인 적 요괴 정보 저장

// 전역 또는 static 변수로 턴 카운트 선언
int turnCount = 0; // 현재 턴 수
int lastYokaiIdx = 0;  // 전역 변수로 선언

// 전투 참여 요괴 인덱스, ID 추적용 배열 및 카운트
int participatedIdx[MAX_PARTY]; // 전투에 참여한 요괴의 인덱스 배열
unsigned long long participatedId[MAX_PARTY]; // 전투에 참여한 요괴의 ID 배열
int participatedCount = 0; // 참여한 요괴 수

// 참여 요괴 인덱스 추가 함수(중복 방지)
static void addParticipatedIdx(int idx) {
    LOG_FUNCTION_EXECUTION("addParticipatedIdx"); // 함수 실행 로그 기록
    for (int i = 0; i < participatedCount; i++) {
        if (participatedIdx[i] == idx) return; // 이미 존재하면 추가하지 않음
    }
    participatedIdx[participatedCount] = idx; // 인덱스 추가
    participatedId[participatedCount] = gParty[idx].id; // ID 추가
    participatedCount++; // 참여 요괴 수 증가
}

// 상대 요괴 이름 색상 반환 함수
static const char* getEnemyNameColor() {
    LOG_FUNCTION_EXECUTION("getEnemyNameColor"); // 함수 실행 로그 기록
    return (gStage.stageNumber % 10 == 0) ? "\033[35m" : "\033[34m"; // 보스 스테이지면 보라색, 아니면 파란색
}

// 외부에서 사용할 수 있도록 export 함수 추가
const char* getEnemyNameColorExport() {
    LOG_FUNCTION_EXECUTION("getEnemyNameColorExport"); // 함수 실행 로그 기록
    return getEnemyNameColor(); // 내부 함수 호출
}

// 요괴가 기절했을 때의 처리 함수
int handleFaintedYokai(int faintedIdx) {
    LOG_FUNCTION_EXECUTION("handleFaintedYokai"); // 함수 실행 로그 기록
    char buffer[256]; // 메시지 버퍼
    sprintf(buffer, "\n%s(이)가 쓰러졌다!\n", gParty[faintedIdx].name); // 기절 메시지 생성
    printText(buffer); // 메시지 출력
    fastSleep(500); // 0.5초 대기
    
    // 다른 기절하지 않은 요괴가 있는지 확인
    bool hasActiveYokai = false; // 활성 요괴 존재 여부
    for (int i = 0; i < gPartyCount; i++) {
        if (gParty[i].status != YOKAI_FAINTED) {
            hasActiveYokai = true; // 기절하지 않은 요괴 발견
            break;
        }
    }
    
    if (!hasActiveYokai) {
        // 모든 요괴가 기절한 경우 즉시 로그라이트 시스템 실행
        handleRogueliteSystem(); // 로그라이트 시스템 호출
        return 104; // 전투 패배 (101, 102, 103, 104 중 하나로 통일)
    }
    
    // 도망가기 실패 후 요괴가 기절한 경우에도 즉시 동료 요괴 선택 메뉴 표시
    int newIdx = selectPartyYokai(); // 새로운 요괴 선택
    
    // 선택된 요괴가 기절한 경우 다시 선택하도록 함
    while (gParty[newIdx].status == YOKAI_FAINTED) {
        printTextAndWait("\n기절한 요괴는 선택할 수 없습니다. 다시 선택하세요."); // 안내 메시지
        newIdx = selectPartyYokai(); // 다시 선택
        if (newIdx == -1) {
            return 0; // 뒤로 돌아가기
        }
    }
    
    return newIdx; // 선택된 요괴 인덱스 반환
}

int startBattle(const Yokai* enemy) {
    LOG_FUNCTION_EXECUTION("startBattle"); // 함수 실행 로그 기록
    if (gameSettings.debugMode) {
        char debugbuf[128]; // 디버그 메시지 버퍼
        sprintf(debugbuf, "[DEBUG] startBattle 진입 - isLoadedGame: %d, turnCount: %d\n", gGameState.isLoadedGame, turnCount);
        printTextAndWait(debugbuf); // 디버그 메시지 출력
        fastSleep(500); // 0.5초 대기
    }
    
    // 전투 시작 대화 표시
    startDialogue(15); // 전투 시작 대화
    
    // 현재 전투 중인 상대 요괴 정보 저장
    currentEnemy = *enemy; // 적 요괴 정보 복사
    
    // 타입에 따른 색상 설정
    const char* colorCode;
    switch (enemy->type) {
        case TYPE_EVIL_SPIRIT:
            colorCode = "\033[31m";  // 빨간색
            break;
        case TYPE_GHOST:
            colorCode = "\033[35m";  // 보라색
            break;
        case TYPE_MONSTER:
            colorCode = "\033[33m";  // 노란색
            break;
        case TYPE_HUMAN:
            colorCode = "\033[36m";  // 청록색
            break;
        case TYPE_ANIMAL:
            colorCode = "\033[32m";  // 초록색
            break;
        default:
            colorCode = "\033[0m";   // 기본색
    }
    
    // 이름 색상 적용
    const char* nameColor = getEnemyNameColor(); // 이름 색상 가져오기
    
    // 요괴 등장 메시지 출력
    char buffer[256]; // 메시지 버퍼
    sprintf(buffer, "%s%s %s[%s]\033[0m Lv.%d (이)가 싸움을 걸어왔다!\n", 
        nameColor, enemy->name, colorCode, typeToString(enemy->type), enemy->level); // 등장 메시지 생성
    printText(buffer); // 메시지 출력
    
    // HP 바 출력
    float maxHP = calculateHP(enemy); // 최대 HP 계산
    float currentHP = enemy->currentHP; // 현재 HP
    float hpPercentage = (currentHP / maxHP) * 100.0f; // HP 비율 계산
    int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH); // 채워진 바 길이 계산
    
    // HP 바 전체를 하나의 문자열로 구성
    char hpBuffer[512]; // HP 바 버퍼
    sprintf(hpBuffer, "%s%s\033[0m HP[", nameColor, enemy->name); // HP 바 시작
    
    // HP 비율에 따른 색상 설정
    if (hpPercentage <= 20.0f) {
        strcat(hpBuffer, "\033[31m"); // 빨간색
    } else if (hpPercentage <= 50.0f) {
        strcat(hpBuffer, "\033[33m"); // 노란색
    } else {
        strcat(hpBuffer, "\033[1;32m"); // 초록색
    }
    
    // HP 바 시각화
    for (int i = 0; i < HP_BAR_LENGTH; i++) {
        if (i < filledLength) {
            strcat(hpBuffer, "█"); // 채워진 부분
        } else {
            strcat(hpBuffer, "░"); // 빈 부분
        }
    }
    
    // 색상 초기화 및 HP 바 종료
    char tempBuffer[256]; // 임시 버퍼
    sprintf(tempBuffer, "\033[0m] %.0f/%.0f", enemy->currentHP, maxHP); // HP 수치 추가
    strcat(hpBuffer, tempBuffer);
    if (enemy->status == YOKAI_FAINTED) {
        strcat(hpBuffer, " (기절)"); // 기절 상태 표시
    }
    strcat(hpBuffer, "\n"); // 줄바꿈 추가
    
    // HP 바 전체를 한 번에 출력
    printTextAndWait(hpBuffer); // HP 바 출력
    
    // 이어하기가 아닌 경우에만 초기화
    if (turnCount == 0) {  // turnCount가 0이면 새 전투 시작
        turnCount = 1;  // 첫 턴을 1로 시작
        lastYokaiIdx = 0;  // 마지막 요괴 인덱스 초기화
        participatedCount = 0; // 참여 요괴 수 초기화
    } else {
    }
    while (1) {
        int done = showBattleMenu(enemy); // 전투 메뉴 표시
        if (done == 101 || done == 102) {
            int reward = calculateBattleReward(); // 전투 보상 금액 계산
            addMoney(reward); // 플레이어 소지금에 보상 추가
            
            // 전투 승리 대화 표시
            
            // 경험치 지급: 참여했고 기절하지 않은 모든 요괴에게 개별 경험치 지급
            for (int i = 0; i < participatedCount; i++) {
                int idx = participatedIdx[i]; // 참여한 요괴 인덱스
                // ID가 일치하는 경우에만 경험치 지급
                if (gParty[idx].status != YOKAI_FAINTED && gParty[idx].status != YOKAI_RELEASED &&
                    gParty[idx].id == participatedId[i]) {
                    // 각 요괴의 레벨에 따라 개별적으로 경험치 계산
                    int exp = calculateIndividualExp(&currentEnemy, &gParty[idx]);
                    gainExp(&gParty[idx], exp); // 경험치 지급
                }
            }
            
            // 고대의 서적 효과: bench 요괴에게 경험치 분배
            for (int i = 0; i < gPartyCount; i++) {
                int isParticipated = 0; // 참여 여부 플래그
                for (int j = 0; j < participatedCount; j++) {
                    if (i == participatedIdx[j]) {
                        isParticipated = 1;
                        break;
                    }
                }
                if (isParticipated) continue; // 이미 참여한 요괴는 건너뜀
                if (gParty[i].status == YOKAI_FAINTED || gParty[i].status == YOKAI_RELEASED) continue; // 기절/방출 요괴 제외
                int bookCount = 0; // 고대의 서적 개수
                for (int k = 0; k < gParty[i].yokaiInventoryCount; k++) {
                    if (strcmp(gParty[i].yokaiInventory[k].item.name, "고대의 서적") == 0) {
                        bookCount = gParty[i].yokaiInventory[k].count;
                        if (bookCount > 5) bookCount = 5; // 최대 5개 제한
                        break;
                    }
                }
                if (bookCount > 0) {
                    int exp = calculateIndividualExp(&currentEnemy, &gParty[i]); // 경험치 계산
                    int bonusExp = (exp * 20 * bookCount) / 100; // 보너스 경험치 계산
                    if (bonusExp < 1) bonusExp = 1; // 최소 1 경험치 보장
                    gainExp(&gParty[i], bonusExp); // 보너스 경험치 지급
                }
            }
            
            // 90스테이지(최종보스)가 아닐 때만 아이템 보상창 호출
            if (gStage.stageNumber != 90) {
                itemRewardSystem(); // 아이템 보상 창 호출
            }
            
            // 함경도 보스 스테이지 완료 후 모든 지역 방문 확인
            if (strcmp(getCurrentRegion(), "함경도") == 0 && gStage.stageNumber % 10 == 0) {
                // 파이널 스테이지 진입 조건 체크 제거
            }
            
            return done;  // 전투 결과 반환
        } else if (done == 103) {
            // 도망 성공: 보상 없음
            startDialogue(330); // 도망 성공 대화
            return done;
        } else if (done == 2) {
            // 저장 후 종료
            return done;
        }
    }
}

int showBattleMenu(const Yokai* enemy) {
    LOG_FUNCTION_EXECUTION("showBattleMenu"); // 함수 실행 로그 기록
    (void)enemy;
    int choice;
    
    if (gameSettings.debugMode) {
        char buffer[128]; // 디버그 메시지 버퍼
        sprintf(buffer, "\n[DEBUG] 현재 턴: %d, 마지막 요괴 인덱스: %d\n", turnCount, lastYokaiIdx);
        printTextAndWait(buffer); // 디버그 메시지 출력
    }
    
    printText("\n무엇을 하시겠습니까?\n");
    printText("1. 싸운다\n");
    printText("2. 부적을 던진다\n");
    printText("3. 동료 요괴를 본다\n");
    if (turnCount > 1) {  // turnCount > 0에서 turnCount > 1로 변경
        printText("4. 동료 요괴 교체\n");
        printText("5. 도망간다\n");
        printText("6. 인벤토리를 본다\n");
        printText("7. 게임을 저장하고 메뉴로 돌아간다\n\n");
        printText("숫자를 입력해주세요: ");
        
        choice = getIntInput(); // 사용자 입력
        if (choice >= 1 && choice <= 7) {
            return handleBattleChoice((BattleChoice)choice, &currentEnemy); // 선택 처리
        }
    } else {
        printText("4. 도망간다\n");
        printText("5. 인벤토리를 본다\n");
        printText("6. 게임을 저장하고 메뉴로 돌아간다\n\n");
        printText("숫자를 입력해주세요: ");
        
        choice = getIntInput(); // 사용자 입력
        if (choice >= 1 && choice <= 6) {
            // 첫 턴에는 교체 옵션이 없으므로 메뉴 번호 조정
            if (choice >= 4) choice++; // 4,5,6 -> 5,6,7
            return handleBattleChoice((BattleChoice)choice, &currentEnemy); // 선택 처리
        }
    }
    
    printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
        return 0;
}

// 동료 요괴 선택 함수
int selectPartyYokai() {
    LOG_FUNCTION_EXECUTION("selectPartyYokai"); // 함수 실행 로그 기록
    printText("\n동료 요괴를 선택하세요:\n");
    for (int i = 0; i < gPartyCount; i++) {
        char buffer[256]; // 출력 버퍼
        float maxHP = calculateHP(&gParty[i]); // 최대 HP 계산
        
        // 타입에 따른 색상 설정
        const char* colorCode;
        switch (gParty[i].type) {
            case TYPE_EVIL_SPIRIT:
                colorCode = "\033[31m";  // 빨간색
                break;
            case TYPE_GHOST:
                colorCode = "\033[35m";  // 보라색
                break;
            case TYPE_MONSTER:
                colorCode = "\033[33m";  // 노란색
                break;
            case TYPE_HUMAN:
                colorCode = "\033[36m";  // 청록색
                break;
            case TYPE_ANIMAL:
                colorCode = "\033[32m";  // 초록색
                break;
            default:
                colorCode = "\033[0m";   // 기본색
        }
        
        // 기절 상태 표시
        const char* statusText = "";
        const char* statusColor = "";
        if (gParty[i].status == YOKAI_FAINTED) {
            statusText = " [기절]";
            statusColor = "\033[31m";  // 빨간색
        }
        
        sprintf(buffer, "%d. %s %s[%s]\033[0m Lv.%d (HP: %.0f/%.0f)%s%s\033[0m\n", 
            i+1, 
            gParty[i].name,
            colorCode,
            typeToString(gParty[i].type),
            gParty[i].level,
            gParty[i].currentHP,
            maxHP,
            statusColor,
            statusText); // 요괴 정보 출력
        printText(buffer);
    }
    printText("숫자를 입력하세요: ");
    int idx = getIntInput() - 1; // 입력값을 인덱스로 변환
    if (idx < 0 || idx >= gPartyCount) {
        printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
        return selectPartyYokai(); // 재귀 호출로 재선택
    }
    if (gParty[idx].status == YOKAI_FAINTED) {
        printTextAndWait("\n기절한 요괴는 선택할 수 없습니다!");
        fastSleep(500);
        return selectPartyYokai(); // 재귀 호출로 재선택
    }
    return idx; // 선택된 인덱스 반환
}

// 기술 선택 함수
int selectMove(const Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("selectMove"); // 함수 실행 로그 기록
    // 모든 기술의 PP가 0인지 확인
    bool allPPZero = true;
    for (int i = 0; i < yokai->moveCount; i++) {
        if (yokai->moves[i].currentPP > 0) {
            allPPZero = false;
            break;
        }
    }
    printText("\n사용할 기술을 선택하세요:\n");
    for (int i = 0; i < yokai->moveCount; i++) {
        char buffer[128]; // 출력 버퍼
        const char* colorCode;
        switch (yokai->moves[i].move.type) {
            case TYPE_EVIL_SPIRIT:
                colorCode = "\033[31m";  // 빨간색
                break;
            case TYPE_GHOST:
                colorCode = "\033[35m";  // 보라색
                break;
            case TYPE_MONSTER:
                colorCode = "\033[33m";  // 노란색
                break;
            case TYPE_HUMAN:
                colorCode = "\033[36m";  // 청록색
                break;
            case TYPE_ANIMAL:
                colorCode = "\033[32m";  // 초록색
                break;
            default:
                colorCode = "\033[0m";   // 기본색
        }
        // 상성 힌트 생성
        char typeHint[64] = "";
        if (gameSettings.showTypeHint) {
            float effectiveness = getTypeEffectiveness(yokai->moves[i].move.type, currentEnemy.type); // 상성 효과 계산
            if (effectiveness > 1.0f) {
                sprintf(typeHint, " \033[32m(강한 공격)\033[0m"); // 강한 공격 표시
            } else if (effectiveness < 1.0f) {
                sprintf(typeHint, " \033[31m(약한 공격)\033[0m"); // 약한 공격 표시
            }
        }
        sprintf(buffer, "%d. %s%s%s\033[0m (공격력: %d, 명중률: %d%%, PP: %d/%d)%s\n", 
            i+1, 
            colorCode,
            yokai->moves[i].move.name,
            colorCode,
            yokai->moves[i].move.power, 
            yokai->moves[i].move.accuracy, 
            yokai->moves[i].currentPP, 
            yokai->moves[i].move.pp,
            typeHint); // 기술 정보 및 상성 힌트 출력
        printText(buffer);
    }
    printText("숫자를 입력하세요: ");
    int idx = getIntInput() - 1; // 입력값을 인덱스로 변환
    if (idx < 0 || idx >= yokai->moveCount) {
        printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
        return selectMove(yokai); // 재귀 호출로 재선택
    }
    if (allPPZero) {
        // 발버둥(스트러글) 상황: 어떤 기술을 선택해도 발버둥
        return -100; // 특수 플래그 값
    }
    if (yokai->moves[idx].currentPP <= 0) {
        printTextAndWait("\n해당 기술의 PP가 부족합니다. 다른 기술을 선택하세요.");
        return selectMove(yokai); // 재귀 호출로 재선택
    }
    return idx; // 선택된 기술 인덱스 반환
}

// 부적 아이템 선택 함수
int selectTalismanFromInventory() {
    LOG_FUNCTION_EXECUTION("selectTalismanFromInventory"); // 함수 실행 로그 기록
    typedef struct {
        int idx;
        ItemGrade grade;
    } TalismanInfo;
    TalismanInfo talismans[INVENTORY_MAX]; // 부적 정보 배열
    int talismanCount = 0; // 부적 개수
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i].item.type == ITEM_TALISMAN && inventory[i].count > 0) {
            talismans[talismanCount].idx = i; // 인덱스 저장
            talismans[talismanCount].grade = inventory[i].item.grade; // 등급 저장
            talismanCount++;
        }
    }
    if (talismanCount == 0) {
        printTextAndWait("\n사용할 수 있는 부적이 없습니다!");
        return -1;
    }
    // 등급 기준 오름차순 정렬
    for (int i = 0; i < talismanCount - 1; i++) {
        for (int j = i + 1; j < talismanCount; j++) {
            if (talismans[i].grade > talismans[j].grade) {
                TalismanInfo temp = talismans[i];
                talismans[i] = talismans[j];
                talismans[j] = temp;
            }
        }
    }
    printText("\n사용할 부적을 선택하세요:\n");
    printText("0. 뒤로 가기\n");
    for (int i = 0; i < talismanCount; i++) {
        char buffer[128]; // 출력 버퍼
        const char* colorCode = "\033[0m";
        switch (inventory[talismans[i].idx].item.grade) {
            case ITEM_COMMON:
                colorCode = "\033[0m"; // 기본색
                break;
            case ITEM_RARE:
                colorCode = "\033[33m"; // 노란색
                break;
            case ITEM_SUPERRARE:
                colorCode = "\033[31m"; // 빨간색
                break;
            default:
                colorCode = "\033[0m";
        }
        sprintf(buffer, "%s%d. %s (보유: %d개)\033[0m\n", colorCode, i+1, inventory[talismans[i].idx].item.name, inventory[talismans[i].idx].count); // 부적 정보 출력
        printText(buffer);
    }
    printText("숫자를 입력해주세요: ");
    int idx = getIntInput();
    if (idx == 0) return -1;
    idx--;
    if (idx < 0 || idx >= talismanCount) {
        printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
        return selectTalismanFromInventory(); // 재귀 호출로 재선택
    }
    return talismans[idx].idx; // 선택된 부적 인벤토리 인덱스 반환
}

// 동료 요괴 교체 함수
int switchYokai() {
    LOG_FUNCTION_EXECUTION("switchYokai"); // 함수 실행 로그 기록
    printText("\n교체할 동료 요괴를 선택하세요:\n");
    for (int i = 0; i < gPartyCount; i++) {
        char buffer[256]; // 출력 버퍼
        float maxHP = calculateHP(&gParty[i]); // 최대 HP 계산
        
        // 기절 상태 표시
        const char* statusText = "";
        const char* statusColor = "";
        if (gParty[i].status == YOKAI_FAINTED) {
            statusText = " [기절]";
            statusColor = "\033[31m";  // 빨간색
        }
        
        sprintf(buffer, "%d. %s Lv.%d (HP: %.0f/%.0f)%s%s\033[0m\n", 
            i+1, 
            gParty[i].name, 
            gParty[i].level,
            gParty[i].currentHP,
            maxHP,
            statusColor,
            statusText); // 요괴 정보 출력
        printText(buffer);
    }
    printText("0. 뒤로 가기\n");
    printText("숫자를 입력해주세요: ");
    
    int choice = getIntInput();
    if (choice == 0) {
        return -1; // 뒤로 가기
    }
    
    if (choice > 0 && choice <= gPartyCount) {
        int idx = choice - 1;
        if (idx == lastYokaiIdx) {
            printTextAndWait("\n이미 전투 중인 요괴입니다!");
            return -1;
        }
        if (gParty[idx].status == YOKAI_FAINTED) {
            printTextAndWait("\n기절한 요괴는 선선택할 수 없습니다!");
            return -1;
        }
        addParticipatedIdx(idx);  // 교체된 요괴의 인덱스 추가
        return idx; // 선택된 인덱스 반환
    }
    
    printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
    return -1;
}

int handleBattleChoice(BattleChoice choice, Yokai* enemy) {
    LOG_FUNCTION_EXECUTION("handleBattleChoice"); // 함수 실행 로그 기록
    if (gameSettings.debugMode) {
        char buffer[128]; // 디버그 메시지 버퍼
        sprintf(buffer, "\n[DEBUG] handleBattleChoice 진입 - 현재 턴: %d, 마지막 요괴 인덱스: %d\n", turnCount, lastYokaiIdx);
        printTextAndWait(buffer); // 디버그 메시지 출력
        fastSleep(500);
    }
    
    switch (choice) {
        case BATTLE_FIGHT: {
            int yokaiIdx;
            if (turnCount == 1) {  // 첫 턴(턴 1)일 때만 요괴 선택
                yokaiIdx = selectPartyYokai(); // 동료 요괴 선택
                if (yokaiIdx == -1) {
                    return 0; // 뒤로 돌아가기
                }
            } else {
                yokaiIdx = lastYokaiIdx;
                if (gParty[yokaiIdx].status == YOKAI_FAINTED) {
                    // 모든 요괴가 기절했는지 먼저 확인
                    bool allFainted = true;
                    for (int i = 0; i < gPartyCount; i++) {
                        if (gParty[i].status != YOKAI_FAINTED) {
                            allFainted = false;
                            break;
                        }
                    }
                    
                    if (allFainted) {
                        handleRogueliteSystem(); // 로그라이트 시스템 호출
                        return 104; // 전투 패배
                    }
                    
                    // 기절하지 않은 요괴가 있으면 즉시 교체 메뉴 표시
                    yokaiIdx = selectPartyYokai(); // 동료 요괴 선택
                    if (yokaiIdx == -1) {
                        return 0; // 뒤로 돌아가기
                    }
                    
                    // 선택된 요괴가 기절한 경우 다시 선택하도록 함
                    while (gParty[yokaiIdx].status == YOKAI_FAINTED) {
                        printTextAndWait("\n기절한 요괴는 선택할 수 없습니다. 다시 선택하세요."); // 안내 메시지 출력
                        yokaiIdx = selectPartyYokai(); // 재선택
                        if (yokaiIdx == -1) {
                            return 0; // 뒤로 돌아가기
                        }
                    }
                }
            }
            // 참여 요괴 인덱스 기록
            addParticipatedIdx(yokaiIdx); // 전투 참여 요괴 기록
            int moveIdx = selectMove(&gParty[yokaiIdx]); // 사용할 기술 선택
            if (moveIdx == -1) {
                return 0; // 뒤로 돌아가기
            }
            int result;
            if (moveIdx == -100) {
                // 발버둥(스트러글) 상황
                result = struggleBattle(&gParty[yokaiIdx], enemy); // 발버둥 처리
            } else {
                result = executeTurnBattle(&gParty[yokaiIdx], enemy, moveIdx); // 일반 기술 처리
            }
            
            // 전투 결과 처리 전에 요괴가 기절했는지 확인
            if (gParty[yokaiIdx].currentHP <= 0) {
                gParty[yokaiIdx].status = YOKAI_FAINTED; // 상태 변경
                gParty[yokaiIdx].currentHP = 0; // HP 0으로 고정
                
                // 모든 요괴가 기절했는지 확인
                bool allFainted = true;
                for (int i = 0; i < gPartyCount; i++) {
                    if (gParty[i].status != YOKAI_FAINTED) {
                        allFainted = false;
                        break;
                    }
                }
                
                if (allFainted) {
                    handleRogueliteSystem(); // 로그라이트 시스템 호출
                    return 104; // 전투 패배
                }
                
                // 남은 동료가 있으면 즉시 교체 메뉴
                int newIdx = selectPartyYokai();
                while (gParty[newIdx].status == YOKAI_FAINTED) {
                    printTextAndWait("\n기절한 요괴는 선택할 수 없습니다. 다시 선택하세요.");
                    newIdx = selectPartyYokai();
                }
                lastYokaiIdx = newIdx;
                applyPeachHealingToParty(); // 복숭아 효과 적용
                turnCount++;
                return 0;
            }
            
            handleBattleResult(&gParty[yokaiIdx], enemy, result); // 전투 결과 처리
            applyPeachHealingToParty(); // 복숭아 효과 적용
            turnCount++;
            lastYokaiIdx = yokaiIdx;
            if (result == 1) {
                gPlayer.stats.yokai_defeated++; // 쓰러트린 요괴 수 증가
                // 경험치 지급은 startBattle에서 일괄 처리
                return 101;
            } else if (result == -1) {
                return 104;
            }
            return 0;
        }
        case BATTLE_TALISMAN: {
            // 패러독스 요괴 및 최종보스 포획 불가 처리
            const char* uncatchable[] = {
                "장산범", "홍콩할매귀신", "퉁퉁퉁퉁 사후르", "사이렌헤드",
                "슬렌더맨", "Entit", "자유로귀신", "갓파", "오니", "누라리횬",  "두억시니"
            };
            int uncatchableCount = 11;
            for (int i = 0; i < uncatchableCount; i++) {
                if (strcmp(enemy->name, uncatchable[i]) == 0) {
                    printTextAndWait("\n이 요괴는 포획할 수 없습니다!\n"); // 안내 메시지 출력
                    fastSleep(700);
                    return 0;
                }
            }
            if (gStage.stageNumber % 10 == 0) {
                printTextAndWait("\n알 수 없는 힘이 부적을 던질 수 없게 합니다!"); // 보스 스테이지 포획 불가
                fastSleep(500);
                return 0;
            }
            int idx = selectTalismanFromInventory(); // 사용할 부적 선택
            if (idx == -1) {
                return 0; // 부적 없음: 메뉴 반복
            }
            char buffer[128];
            if (useTalisman(&inventory[idx].item, enemy)) {
                sprintf(buffer, "\n%s를 던졌다! 요괴를 잡았다!", inventory[idx].item.name);
                printTextAndWait(buffer);
                
                // 도감에 요괴 기록 (중복 체크 포함)
                bool alreadyCaught = false;
                for (int i = 0; i < yokaiListCount; i++) {
                    if (strcmp(yokaiList[i].name, enemy->name) == 0) {
                        // 이미 잡은 요괴인지 확인
                        if (!isYokaiCaught(i + 1)) {
                            markYokaiAsCaught(i + 1);
                            gTotalCaughtCount++;
                        } else {
                            alreadyCaught = true;
                        }
                        break;
                    }
                }
                
                // 보스 요괴 체크
                if (!alreadyCaught) {
                    for (int i = 0; i < bossYokaiListCount; i++) {
                        if (strcmp(bossYokaiList[i].name, enemy->name) == 0) {
                            if (!isYokaiCaught(yokaiListCount + i + 1)) {
                                markYokaiAsCaught(yokaiListCount + i + 1);
                                gTotalCaughtCount++;
                            }
                            break;
                        }
                    }
                }
                
                // 패러독스 요괴 체크
                if (!alreadyCaught) {
                    for (int i = 0; i < paradoxYokaiListCount; i++) {
                        if (strcmp(paradoxYokaiList[i].name, enemy->name) == 0) {
                            if (!isYokaiCaught(yokaiListCount + bossYokaiListCount + i + 1)) {
                                markYokaiAsCaught(yokaiListCount + bossYokaiListCount + i + 1);
                                gTotalCaughtCount++;
                            }
                            break;
                        }
                    }
                }
                
                // 요괴를 파티에 추가 (현재 전투 중인 요괴의 정보 사용)
                if (addYokaiToParty(enemy)) {
                    sprintf(buffer, "\n%s가 동료가 되었습니다!", enemy->name);
                    printTextAndWait(buffer);
                    fastSleep(500);
                }
                if (inventory[idx].count == 1) {
                    for (int i = idx; i < inventoryCount - 1; i++)
                        inventory[i] = inventory[i + 1];
                    inventoryCount--;
                } else {
                    inventory[idx].count--;
                }
                return 102; // BATTLE_TALISMAN 성공
            } else {
                sprintf(buffer, "\n%s를 던졌다! 하지만 요괴를 잡지 못했다...", inventory[idx].item.name);
                printTextAndWait(buffer);
                fastSleep(500);
                if (inventory[idx].count == 1) {
                    for (int i = idx; i < inventoryCount - 1; i++)
                        inventory[i] = inventory[i + 1];
                    inventoryCount--;
                } else {
                    inventory[idx].count--;
                }
                
                // 첫 턴이면 요괴를 선택한 후 상대 요괴의 공격을 받음
                int yokaiIdx;
                if (turnCount == 1) {
                    yokaiIdx = selectPartyYokai(); // 동료 요괴 선택
                    if (yokaiIdx == -1) {
                        return 0; // 뒤로 돌아가기
                    }
                    lastYokaiIdx = yokaiIdx;
                } else {
                    // 첫 턴이 아니면 이전에 선택한 요괴 사용
                    yokaiIdx = lastYokaiIdx;
                }
                
                // 상대 요괴의 랜덤 기술 선택
                int enemyMoveIdx = rand() % enemy->moveCount;
                
                // 상대 요괴의 공격만 실행 (플레이어는 공격하지 않음)
                float damage = calculateDamage(enemy, &gParty[yokaiIdx], &enemy->moves[enemyMoveIdx].move);
                float actualDamage = damage;
                if (actualDamage > gParty[yokaiIdx].currentHP) {
                    actualDamage = gParty[yokaiIdx].currentHP;
                }
                
                gParty[yokaiIdx].currentHP -= actualDamage;
                
                // 데미지 메시지 출력
                char buffer[256];
                sprintf(buffer, "\n%s에게 %.0f의 데미지를 입었다!", gParty[yokaiIdx].name, actualDamage);
                printTextAndWait(buffer);
                
                // HP 바 업데이트
                float maxHP = calculateHP(&gParty[yokaiIdx]);
                float hpPercentage = (gParty[yokaiIdx].currentHP / maxHP) * 100.0f;
                int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
                
                sprintf(buffer, "\n%s HP[", gParty[yokaiIdx].name);
                if (hpPercentage <= 20.0f) {
                    strcat(buffer, "\033[31m"); // 빨간색
                } else if (hpPercentage <= 50.0f) {
                    strcat(buffer, "\033[33m"); // 노란색
                } else {
                    strcat(buffer, "\033[1;32m"); // 초록색
                }
                
                for (int i = 0; i < HP_BAR_LENGTH; i++) {
                    if (i < filledLength) {
                        strcat(buffer, "█");
                    } else {
                        strcat(buffer, "░");
                    }
                }
                
                char tempBuffer[256];
                sprintf(tempBuffer, "\033[0m] %.0f/%.0f", gParty[yokaiIdx].currentHP, maxHP);
                strcat(buffer, tempBuffer);
                if (gParty[yokaiIdx].status == YOKAI_FAINTED) {
                    strcat(buffer, " (기절)");
                }
                strcat(buffer, "\n");
                
                printTextAndWait(buffer);
                
                // 요괴가 기절했는지 확인
                if (gParty[yokaiIdx].currentHP <= 0) {
                    gParty[yokaiIdx].status = YOKAI_FAINTED;
                    gParty[yokaiIdx].currentHP = 0;
                    
                    // 모든 요괴가 기절했는지 확인
                    bool allFainted = true;
                    for (int i = 0; i < gPartyCount; i++) {
                        if (gParty[i].status != YOKAI_FAINTED) {
                            allFainted = false;
                            break;
                        }
                    }
                    
                    if (allFainted) {
                        handleRogueliteSystem();
                        return 104; // 전투 패배
                    }
                    
                    // 남은 동료가 있으면 즉시 교체 메뉴
                    int newIdx = selectPartyYokai();
                    while (gParty[newIdx].status == YOKAI_FAINTED) {
                        printTextAndWait("\n기절한 요괴는 선택할 수 없습니다. 다시 선택하세요.");
                        newIdx = selectPartyYokai();
                    }
                    lastYokaiIdx = newIdx;
                }
                
                applyPeachHealingToParty(); // 복숭아 효과 적용
                turnCount++;
                return 0;
            }
        }
        case BATTLE_CHECK_PARTY:
            printParty(); // 파티 정보 출력
            return 0;
        case BATTLE_SWITCH: {
            int newYokaiIdx = switchYokai(); // 교체할 요괴 선택
            if (newYokaiIdx == -1) {
                return 0;
            }
            // 참여 요괴 인덱스 기록
            addParticipatedIdx(newYokaiIdx); // 전투 참여 요괴 기록
            // 교체 메시지 출력
            char buffer[256];
            sprintf(buffer, "\n%s를 불러왔다!", gParty[newYokaiIdx].name);
            printText(buffer);
            
            // 상대 요괴의 공격
            int enemyMoveIdx = rand() % enemy->moveCount;
            float damage = calculateDamage(enemy, &gParty[newYokaiIdx], &enemy->moves[enemyMoveIdx].move);
            float actualDamage = damage;
            if (actualDamage > gParty[newYokaiIdx].currentHP) {
                actualDamage = gParty[newYokaiIdx].currentHP;
            }
            gParty[newYokaiIdx].currentHP -= actualDamage;
            
            // 기술 이름 출력
            char moveMsg[256];
            const char* colorCode;
            switch (enemy->type) {
                case TYPE_EVIL_SPIRIT:
                    colorCode = "\033[31m";  // 빨간색
                    break;
                case TYPE_GHOST:
                    colorCode = "\033[35m";  // 보라색
                    break;
                case TYPE_MONSTER:
                    colorCode = "\033[33m";  // 노란색
                    break;
                case TYPE_HUMAN:
                    colorCode = "\033[36m";  // 청록색
                    break;
                case TYPE_ANIMAL:
                    colorCode = "\033[32m";  // 초록색
                    break;
                default:
                    colorCode = "\033[0m";   // 기본색
            }
            sprintf(moveMsg, "\n%s%s\033[0m의 %s%s\033[0m!\n", getEnemyNameColorExport(), enemy->name, colorCode, enemy->moves[enemyMoveIdx].move.name); // 기술명 출력
            printTextAndWait(moveMsg);
            
            // 데미지 메시지 출력
            char damageMsg[256];
            sprintf(damageMsg, "%s에게 %.0f의 데미지를 입혔다!\n", gParty[newYokaiIdx].name, actualDamage);
            printTextAndWait(damageMsg);
            
            // HP 바 업데이트
            float maxHP = calculateHP(&gParty[newYokaiIdx]);
            float hpPercentage = (gParty[newYokaiIdx].currentHP / maxHP) * 100.0f;
            int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
            
            // HP 바 전체를 하나의 문자열로 구성
            char hpBuffer[512];
            sprintf(hpBuffer, "%s HP[", gParty[newYokaiIdx].name);
            
            // HP 비율에 따른 색상 설정
            if (hpPercentage <= 20.0f) {
                strcat(hpBuffer, "\033[31m"); // 빨간색
            } else if (hpPercentage <= 50.0f) {
                strcat(hpBuffer, "\033[33m"); // 노란색
            } else {
                strcat(hpBuffer, "\033[1;32m"); // 초록색
            }
            
            // HP 바 시각화
            for (int i = 0; i < HP_BAR_LENGTH; i++) {
                if (i < filledLength) {
                    strcat(hpBuffer, "█"); // 채워진 부분
                } else {
                    strcat(hpBuffer, "░"); // 빈 부분
                }
            }
            
            // 색상 초기화 및 HP 바 종료
            char tempBuffer[256];
            sprintf(tempBuffer, "\033[0m] %.0f/%.0f", gParty[newYokaiIdx].currentHP, maxHP);
            strcat(hpBuffer, tempBuffer);
            if (gParty[newYokaiIdx].status == YOKAI_FAINTED) {
                strcat(hpBuffer, " (기절)");
            }
            strcat(hpBuffer, "\n");
            
            // HP 바 전체를 한 번에 출력
            printTextAndWait(hpBuffer);
            
            // 전투 결과 확인
            if (gParty[newYokaiIdx].currentHP <= 0) {
                gParty[newYokaiIdx].status = YOKAI_FAINTED; // 상태 변경
                gParty[newYokaiIdx].currentHP = 0; // HP 0으로 고정
                
                // 모든 요괴가 기절했는지 확인
                bool allFainted = true;
                for (int i = 0; i < gPartyCount; i++) {
                    if (gParty[i].status != YOKAI_FAINTED) {
                        allFainted = false;
                        break;
                    }
                }
                
                if (allFainted) {
                    handleRogueliteSystem(); // 로그라이트 시스템 호출
                    return 104; // 전투 패배
                }

                // 남은 동료가 있으면 즉시 교체 메뉴
                int newIdx = selectPartyYokai(); // 교체할 요괴 선택
                while (gParty[newIdx].status == YOKAI_FAINTED) {
                    printTextAndWait("\n기절한 요괴는 선택할 수 없습니다. 다시 선택하세요.");
                    newIdx = selectPartyYokai();
                }
                lastYokaiIdx = newIdx;
                applyPeachHealingToParty(); // 복숭아 효과 적용
                turnCount++;
                return 0;
            }
            
            applyPeachHealingToParty(); // 복숭아 효과 적용
            turnCount++;
            lastYokaiIdx = newYokaiIdx;
            return 0;
        }
        case BATTLE_RUN: {
            int escapeResult = tryToEscape(); // 도망 시도
            if (escapeResult == ESCAPE_SUCCESS) {
                return 103; // 도망 성공
            } else {
                // 도망 실패 시 상대 요괴의 공격을 받음
                // 첫 턴이면 요괴를 선택한 후 상대 요괴의 공격을 받음
                int yokaiIdx;
                if (turnCount == 1) {
                    yokaiIdx = selectPartyYokai(); // 동료 요괴 선택
                    if (yokaiIdx == -1) {
                        return 0; // 뒤로 돌아가기
                    }
                    lastYokaiIdx = yokaiIdx;
                } else {
                    // 첫 턴이 아니면 이전에 선택한 요괴 사용
                    yokaiIdx = lastYokaiIdx;
                }
                
                // 상대 요괴의 랜덤 기술 선택
                int enemyMoveIdx = rand() % enemy->moveCount;
                
                // 상대 요괴의 공격만 실행 (플레이어는 공격하지 않음)
                float damage = calculateDamage(enemy, &gParty[yokaiIdx], &enemy->moves[enemyMoveIdx].move);
                float actualDamage = damage;
                if (actualDamage > gParty[yokaiIdx].currentHP) {
                    actualDamage = gParty[yokaiIdx].currentHP;
                }
                
                gParty[yokaiIdx].currentHP -= actualDamage;
                
                // 데미지 메시지 출력
                char buffer[256];
                sprintf(buffer, "\n%s에게 %.0f의 데미지를 입었다!", gParty[yokaiIdx].name, actualDamage);
                printTextAndWait(buffer);
                
                // HP 바 업데이트
                float maxHP = calculateHP(&gParty[yokaiIdx]);
                float hpPercentage = (gParty[yokaiIdx].currentHP / maxHP) * 100.0f;
                int filledLength = (int)((hpPercentage / 100.0f) * HP_BAR_LENGTH);
                
                sprintf(buffer, "\n%s HP[", gParty[yokaiIdx].name);
                if (hpPercentage <= 20.0f) {
                    strcat(buffer, "\033[31m"); // 빨간색
                } else if (hpPercentage <= 50.0f) {
                    strcat(buffer, "\033[33m"); // 노란색
                } else {
                    strcat(buffer, "\033[1;32m"); // 초록색
                }
                
                for (int i = 0; i < HP_BAR_LENGTH; i++) {
                    if (i < filledLength) {
                        strcat(buffer, "█");
                    } else {
                        strcat(buffer, "░");
                    }
                }
                
                char tempBuffer[256];
                sprintf(tempBuffer, "\033[0m] %.0f/%.0f", gParty[yokaiIdx].currentHP, maxHP);
                strcat(buffer, tempBuffer);
                if (gParty[yokaiIdx].status == YOKAI_FAINTED) {
                    strcat(buffer, " (기절)");
                }
                strcat(buffer, "\n");
                
                printTextAndWait(buffer);
                
                // 요괴가 기절했는지 확인
                if (gParty[yokaiIdx].currentHP <= 0) {
                    gParty[yokaiIdx].status = YOKAI_FAINTED;
                    gParty[yokaiIdx].currentHP = 0;
                    
                    // 모든 요괴가 기절했는지 확인
                    bool allFainted = true;
                    for (int i = 0; i < gPartyCount; i++) {
                        if (gParty[i].status != YOKAI_FAINTED) {
                            allFainted = false;
                            break;
                        }
                    }
                    
                    if (allFainted) {
                        handleRogueliteSystem();
                        return 104; // 전투 패배
                    }
                    
                    // 남은 동료가 있으면 즉시 교체 메뉴
                    int newIdx = selectPartyYokai();
                    while (gParty[newIdx].status == YOKAI_FAINTED) {
                        printTextAndWait("\n기절한 요괴는 선택할 수 없습니다. 다시 선택하세요.");
                        newIdx = selectPartyYokai();
                    }
                    lastYokaiIdx = newIdx;
                }
                
                applyPeachHealingToParty(); // 복숭아 효과 적용
                turnCount++;
                return 0;
            }
        }
        case BATTLE_CHECK_INVENTORY:
            showItemMenu(); // 인벤토리 출력
            break;
        case BATTLE_SAVE_AND_EXIT:
            gGameState.isManualSave = true;  // 수동 저장 플래그 설정
            saveGame(); // 게임 저장
            printTextAndWait("\n게임이 저장되었습니다. 메뉴로 돌아갑니다.");
            fastSleep(500);
            gGameState.isRunning = 0;
            return 2;
    }
    return 0;
}

void printDamageMessage(Yokai* attacker, Yokai* defender, int damage) {
    LOG_FUNCTION_EXECUTION("printDamageMessage"); // 함수 실행 로그 기록
    (void)attacker;
    char buffer[256];
    int actualDamage = damage;
    
    // 실제 데미지가 현재 HP보다 크면 현재 HP만큼만 데미지를 입힘
    if (actualDamage > defender->currentHP) {
        actualDamage = defender->currentHP;
    }
    
    sprintf(buffer, "%s에게 %d의 데미지를 입혔다!", defender->name, actualDamage); // 데미지 메시지 생성
    printTextAndWait(buffer); // 메시지 출력
}

// 복숭아 효과 함수: 전투 중인 요괴에게만 복숭아 1개당 최대 HP의 5% 회복(기절 제외)
void applyPeachHealingToParty() {
    LOG_FUNCTION_EXECUTION("applyPeachHealingToParty"); // 함수 실행 로그 기록
    // 전투에 실제로 참여한 요괴만 대상으로 함
    if (lastYokaiIdx >= 0 && lastYokaiIdx < gPartyCount) {
        // 해당 요괴가 실제로 전투에 참여했는지 확인
        bool isParticipated = false;
        for (int i = 0; i < participatedCount; i++) {
            if (participatedIdx[i] == lastYokaiIdx) {
                isParticipated = true;
                break;
            }
        }
        
        // 전투에 참여하지 않은 요괴는 복숭아 효과 적용 안함
        if (!isParticipated) {
            return;
        }
        
        Yokai* y = &gParty[lastYokaiIdx];
        if (y->status == YOKAI_FAINTED) return; // 기절한 요괴 제외
        
        int peachCount = 0;
        for (int j = 0; j < y->yokaiInventoryCount; j++) {
            if (strcmp(y->yokaiInventory[j].item.name, "복숭아") == 0) {
                peachCount = y->yokaiInventory[j].count;
                break;
            }
        }
        if (peachCount > 0) {
            float maxHP = calculateHP(y);
            float healAmount = (int)(maxHP * 0.05f * peachCount);
            if (healAmount < 1) healAmount = 1; // 최소 1 회복
            float beforeHP = y->currentHP;
            y->currentHP += healAmount;
            if (y->currentHP > maxHP) y->currentHP = maxHP;
            // 회복 메시지 출력
            char buffer[128];
            sprintf(buffer, "\n%s가 복숭아 효과로 %.0f의 체력을 회복했습니다! (%.0f → %.0f)", y->name, y->currentHP - beforeHP, beforeHP, y->currentHP);
            printTextAndWait(buffer);
        }
    }
} 