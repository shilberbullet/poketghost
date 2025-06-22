// 표준 입출력, 메모리, 시간, 문자열, 게임 관련 헤더
#include <stdio.h> // 표준 입출력 함수 사용을 위한 헤더
#include <stdlib.h> // 메모리 할당, 랜덤 함수 등 사용을 위한 헤더
#include <time.h> // 시간 관련 함수 사용을 위한 헤더
#include <string.h> // 문자열 처리 함수 사용을 위한 헤더
#include "stage.h" // 스테이지 관련 구조체 및 함수 선언
#include "normal_stage.h" // 일반 스테이지 관련 함수 선언
#include "boss_stage.h" // 보스 스테이지 관련 함수 선언
#include "text.h" // 텍스트 출력 관련 함수 선언
#include "input.h" // 입력 처리 관련 함수 선언
#include "battle.h" // 전투 시스템 관련 함수 선언
#include "savefile.h" // 저장 파일 관련 함수 선언
#include "party.h" // 파티 관련 함수 선언
#include "yokai.h" // 요괴 관련 함수 선언
#include "game.h" // 게임 상태 관련 함수 선언
#include "region.h" // 지역 관련 함수 선언
#include "item.h" // 아이템 관련 함수 선언
#include "event_system.h" // 이벤트 시스템 관련 함수 선언
#include "../core/state.h" // 게임 상태 관리 헤더
#include "logger.h" // 로깅 시스템 관련 함수 선언
#include <windows.h> // Windows API 함수 사용을 위한 헤더
#include "statistics.h" // 통계 관련 함수 선언
#include "dialogue.h" // 대화 시스템 관련 함수 선언
#include "encyclopedia.h" // 도감 시스템 관련 함수 선언

// 지형 이름 배열
const char* terrainNames[] = { // 일반 스테이지에서 사용되는 지형 이름들
    "산", "강", "바다", "논", "마을", "차원의 균열" // 각 지형의 한글 이름
};

// 파이널 스테이지 지형 순서 정의
const int terrainSequence[] = { // 백두산 스테이지들의 지형 순서 (81~90스테이지)
    0, 0,  // 백두산 입구 (2개) - 81, 82스테이지
    1, 1,  // 백두산 숲길 (2개) - 83, 84스테이지
    2, 2,  // 백두산 계곡 (2개) - 85, 86스테이지
    3, 3, 3,  // 백두산 중턱 (3개) - 87, 88, 89스테이지
    4  // 백두산 천지 (1개) - 90스테이지
};

const char* finalTerrainNames[] = { // 파이널 스테이지 지형의 한글 이름들
    "백두산 입구", // 지형 인덱스 0
    "백두산 숲길", // 지형 인덱스 1
    "백두산 계곡", // 지형 인덱스 2
    "백두산 중턱", // 지형 인덱스 3
    "백두산 천지" // 지형 인덱스 4
};

// 스테이지 정보는 state 모듈에서 관리

// 스테이지 초기화 함수
void initStage(StageInfo* stage, int stageNumber) { // 스테이지 정보를 초기화하는 함수
    LOG_FUNCTION_EXECUTION("initStage"); // 함수 실행 로그 기록
    if (stageNumber >= 81) { // 파이널 스테이지(81~90스테이지)인 경우
        // 파이널 스테이지 초기화
        stage->stageNumber = stageNumber; // 스테이지 번호 설정
        stage->isBossStage = false; // 보스 스테이지가 아님을 표시
        strcpy(stage->region, "백두산"); // 지역을 백두산으로 설정
        
        // 스테이지 번호에 따른 지형 설정 (81부터 시작)
        int terrainIndex = stageNumber - 81; // 81스테이지를 0번 인덱스로 변환
        if (terrainIndex >= 0 && terrainIndex < FINAL_TERRAIN_SEQUENCE_LENGTH) { // 유효한 지형 인덱스인지 확인
            stage->terrain = terrainSequence[terrainIndex]; // 지형 순서 배열에서 지형 인덱스 가져오기
            strcpy(stage->terrainName, finalTerrainNames[terrainSequence[terrainIndex]]); // 지형 이름 설정
        } else { // 범위를 벗어난 경우
            stage->terrain = FINAL_TERRAIN_COUNT - 1; // 마지막 지형(백두산 천지)으로 설정
            strcpy(stage->terrainName, finalTerrainNames[FINAL_TERRAIN_COUNT - 1]); // 마지막 지형 이름 설정
        }
        
        // 레벨 범위 계산
        calculateLevelRange(stageNumber, &stage->minLevel, &stage->maxLevel); // 스테이지 번호에 따른 레벨 범위 계산
        int level = stage->minLevel + (rand() % (stage->maxLevel - stage->minLevel + 1)); // 랜덤 레벨 생성
        
        // 보스 스테이지(90스테이지)에서는 최종보스 출현
        if (stageNumber == 90) { // 최종 스테이지인 경우
            // 적 배열 전체 초기화
            memset(stage->enemies, 0, sizeof(stage->enemies)); // 적 배열을 0으로 초기화
            stage->enemyCount = 1; // 적 개수를 1로 설정
            Yokai* y = findYokaiByName("두억시니"); // 두억시니 요괴를 찾기
            if (y) { // 두억시니를 찾은 경우
                stage->enemies[0] = *y; // 찾은 요괴를 적 배열에 복사
                stage->enemies[0].level = level; // 레벨 설정
                // HP 계산 및 설정
                stage->enemies[0].currentHP = calculateHP(&stage->enemies[0]); // 최대 HP 계산하여 현재 HP 설정
                
                // 최종보스는 모든 기술을 사용할 수 있도록 설정
                stage->enemies[0].moveCount = stage->enemies[0].learnableMoveCount; // 배울 수 있는 기술 개수를 현재 기술 개수로 설정
                if (stage->enemies[0].moveCount > MAX_MOVES) { // 최대 기술 개수를 초과하는 경우
                    stage->enemies[0].moveCount = MAX_MOVES; // 최대 기술 개수로 제한
                }
                
                // 모든 기술을 moves 배열에 복사
                for (int i = 0; i < stage->enemies[0].moveCount; i++) { // 모든 기술에 대해 반복
                    stage->enemies[0].moves[i].move = stage->enemies[0].learnableMoves[i]; // 배울 수 있는 기술을 현재 기술로 복사
                    stage->enemies[0].moves[i].currentPP = stage->enemies[0].learnableMoves[i].pp; // 현재 PP를 최대 PP로 설정
                }
                
                printf("[DEBUG] 최종보스 생성 성공: %s, 레벨: %d, HP: %.1f, 기술 수: %d\n", 
                       stage->enemies[0].name, stage->enemies[0].level, stage->enemies[0].currentHP, stage->enemies[0].moveCount); // 디버그 메시지 출력
            } else { // 두억시니를 찾지 못한 경우
                printf("[DEBUG] 두억시니를 찾을 수 없음, yokai.txt 재로드 시도\n"); // 디버그 메시지 출력
                loadYokaiFromFile("data/yokai.txt"); // 요괴 파일을 다시 로드
                y = findYokaiByName("두억시니"); // 다시 두억시니를 찾기
                if (y) { // 재로드 후 두억시니를 찾은 경우
                    stage->enemies[0] = *y; // 찾은 요괴를 적 배열에 복사
                    stage->enemies[0].level = level; // 레벨 설정
                    // HP 계산 및 설정
                    stage->enemies[0].currentHP = calculateHP(&stage->enemies[0]); // 최대 HP 계산하여 현재 HP 설정
                    
                    // 최종보스는 모든 기술을 사용할 수 있도록 설정
                    stage->enemies[0].moveCount = stage->enemies[0].learnableMoveCount; // 배울 수 있는 기술 개수를 현재 기술 개수로 설정
                    if (stage->enemies[0].moveCount > MAX_MOVES) { // 최대 기술 개수를 초과하는 경우
                        stage->enemies[0].moveCount = MAX_MOVES; // 최대 기술 개수로 제한
                    }
                    
                    // 모든 기술을 moves 배열에 복사
                    for (int i = 0; i < stage->enemies[0].moveCount; i++) { // 모든 기술에 대해 반복
                        stage->enemies[0].moves[i].move = stage->enemies[0].learnableMoves[i]; // 배울 수 있는 기술을 현재 기술로 복사
                        stage->enemies[0].moves[i].currentPP = stage->enemies[0].learnableMoves[i].pp; // 현재 PP를 최대 PP로 설정
                    }
                    
                    printf("[DEBUG] 최종보스 생성 성공(재로드 후): %s, 레벨: %d, HP: %.1f, 기술 수: %d\n", 
                           stage->enemies[0].name, stage->enemies[0].level, stage->enemies[0].currentHP, stage->enemies[0].moveCount); // 디버그 메시지 출력
                } else { // 재로드 후에도 두억시니를 찾지 못한 경우
                    printf("[DEBUG] 두억시니를 여전히 찾을 수 없음\n"); // 디버그 메시지 출력
                }
            }
        } else { // 일반 파이널 스테이지인 경우
            // 일반 스테이지에서는 패러독스 요괴 출현
            stage->enemyCount = 1; // 적 개수를 1로 설정
            stage->enemies[0] = createRandomParadoxYokaiWithLevel(level); // 랜덤 패러독스 요괴 생성
        }
    } else if (stageNumber % 10 == 0) { // 10의 배수 스테이지인 경우
        initBossStage(stage, stageNumber);  // 10의 배수 스테이지는 보스 스테이지 초기화
    } else { // 일반 스테이지인 경우
        initNormalStage(stage, stageNumber);  // 그 외는 일반 스테이지 초기화
    }
    
    // 새 게임일 경우 시간을 0으로 초기화
    if (gGameState.isNewGame) { // 새 게임인 경우
        stage->hour = 0; // 시간을 0으로 초기화
        gGameState.isNewGame = 0;  // 플래그 초기화
    }
}

// 다음 스테이지로 진행하는 함수
void nextStage() { // 다음 스테이지로 진행하는 함수
    LOG_FUNCTION_EXECUTION("nextStage"); // 함수 실행 로그 기록
    gStage.stageNumber++;  // 스테이지 번호 증가
    gPlayer.stats.stages_completed++; // 클리어한 스테이지 수 증가
    turnCount = 0;              // 턴 카운트 초기화
    gStage.hour = (gStage.hour + 1) % 24;  // 시간 증가 (24시간 주기)
    
    // 10스테이지마다 지역 변경
    if (gStage.stageNumber % 10 == 1) {  // 1, 11, 21... 스테이지에서 지역 변경
        // 81스테이지부터 파이널 스테이지로 진입
        if (gStage.stageNumber >= 81) { // 81스테이지 이상인 경우
            strcpy(gStage.region, "백두산"); // 지역을 백두산으로 설정
            initStage(&gStage, gStage.stageNumber); // 스테이지 초기화
            return; // 함수 종료
        }

        int hasMap = 0; // 지도 보유 여부 플래그
        for (int i = 0; i < inventoryCount; i++) { // 인벤토리 검사
            if (strcmp(inventory[i].item.name, "지도") == 0 && inventory[i].count > 0) { // 지도 아이템이 있는지 확인
                hasMap = 1; // 지도 보유 플래그 설정
                break; // 지도를 찾았으므로 루프 종료
            }
        }
        if (hasMap) { // 지도가 있는 경우
            if (moveToNextRegionWithMap()) { // 지도를 사용하여 다음 지역으로 이동
                char buffer[256];
                sprintf(buffer, "\n지도 아이템을 사용하여 %s로 이동했습니다.\n", getCurrentRegion()); // 이동 메시지 생성
                printText(buffer); // 이동 메시지 출력
                displayConnectedRegions(); // 연결된 지역 표시
            }
        } else { // 지도가 없는 경우
            if (moveToNextRegion()) { // 일반적인 방법으로 다음 지역으로 이동
                char buffer[256];
                sprintf(buffer, "\n%s로 이동했습니다.\n", getCurrentRegion()); // 이동 메시지 생성
                printText(buffer); // 이동 메시지 출력
                displayConnectedRegions(); // 연결된 지역 표시
            }
        }
    }
    
    // 새로운 스테이지 초기화
    initStage(&gStage, gStage.stageNumber); // 현재 스테이지 번호로 스테이지 초기화
    
    // 5의 배수 스테이지 완료 시 자동 저장 (수동 저장이 아닌 경우에만)
    if ((gStage.stageNumber - 1) % 5 == 0 && !gGameState.isLoadedGame && !gGameState.isManualSave) { // 5의 배수 스테이지이고 로드된 게임이나 수동 저장이 아닌 경우
        saveGame(); // 게임 저장
        char buffer[128];
        sprintf(buffer, "\n%d스테이지 완료! 게임이 자동 저장되었습니다.\n", gStage.stageNumber - 1); // 자동 저장 메시지 생성
        printText(buffer); // 자동 저장 메시지 출력
        fastSleep(500); // 0.5초 대기
    }
}

// 스테이지 정보를 표시하는 함수
void showStageInfo() { // 스테이지 정보를 화면에 표시하는 함수
    LOG_FUNCTION_EXECUTION("showStageInfo"); // 함수 실행 로그 기록
    char buffer[256]; // 텍스트 버퍼 선언
    system("cls");  // 화면 지우기
    printText("=== 스테이지 정보 ===\n\n"); // 스테이지 정보 제목 출력
    sprintf(buffer, "스테이지: %d\n", gStage.stageNumber);  // 스테이지 번호
    printText(buffer); // 스테이지 번호 출력
    sprintf(buffer, "지역: %s\n", gStage.region);  // 현재 지역명 출력
    printText(buffer); // 지역명 출력
    sprintf(buffer, "지형: %s\n", gStage.terrainName);  // 지형 이름
    printText(buffer); // 지형 이름 출력
    int hour = (gStage.stageNumber - 1) % 24;  // 현재 시간 계산 (스테이지 번호를 24시간 주기로 변환)
    sprintf(buffer, "시간: %02d시\n", hour); // 시간 정보 생성
    printText(buffer); // 시간 정보 출력
    sprintf(buffer, "보유 전: %d전\n\n", gPlayer.money);  // 보유 금액
    printText(buffer); // 보유 금액 출력
}

// 전투 인터페이스를 표시하고 전투를 시작하는 함수
void showBattleInterface() { // 전투 인터페이스를 표시하고 전투를 시작하는 함수
    LOG_FUNCTION_EXECUTION("showBattleInterface"); // 함수 실행 로그 기록
    
    // 이벤트가 발생한 경우(enemyCount가 0) 전투를 건너뛰고 다음 스테이지로 진행
    if (gStage.enemyCount == 0) { // 적이 없는 경우 (이벤트 발생)
        handleNormalStageClear();  // 일반 스테이지 클리어 처리
        nextStage();               // 다음 스테이지로 진행
        // 이벤트 처리 후에는 showStageInfo를 호출하지 않음 (이미 이벤트 UI에서 출력됨)
        return; // 함수 종료
    }
    
    int minLevel, maxLevel; // 레벨 범위 변수 선언
    calculateLevelRange(gStage.stageNumber, &minLevel, &maxLevel);  // 레벨 범위 계산

    Yokai enemy; // 적 요괴 변수 선언
    if (gGameState.isLoadedGame) { // 이어하기인 경우
        enemy = currentEnemy;  // 이어하기 시 저장된 적 요괴 사용
        gGameState.isLoadedGame = 0;  // 플래그 초기화
    } else if (gStage.isBossStage) { // 보스 스테이지인 경우
        enemy = gStage.enemies[0];  // 보스 스테이지의 경우 첫 번째 적 요괴 사용
    } else { // 일반 스테이지인 경우
        enemy = gStage.enemies[rand() % gStage.enemyCount];  // 랜덤 적 요괴 선택
    }

    int battleResult = startBattle(&enemy);  // 전투 시작

    if (gStage.isBossStage) { // 보스 스테이지인 경우
        if (battleResult == 101 || battleResult == 102) {  // 승리한 경우
            handleBossStageClear();  // 보스 스테이지 클리어 처리
            nextStage();  // 다음 스테이지로 진행
        }
    } else if (gStage.stageNumber >= 81) {  // 파이널 스테이지인 경우
        if (battleResult == 101 || battleResult == 102) {  // 승리한 경우
            if (gStage.stageNumber == 90) {  // 최종 보스 클리어인 경우
                startDialogue(90); // 최종 보스 클리어 대화 시작
                printText("게임 클리어를 축하합니다!\n"); // 게임 클리어 축하 메시지
                gPlayer.stats.games_cleared++; // 게임 클리어 횟수 증가
                update_total_statistics_on_save(); // 통계 저장
                // 잡은 요괴 데이터 저장
                saveCaughtYokaiData(); // 포획한 요괴 데이터 저장
                fastSleep(2000); // 2초 대기
                resetGameAfterClear();  // 게임 데이터 초기화
                exit(0); // 프로그램 종료
            }
            nextStage();  // 다음 스테이지로 진행
        }
    } else { // 일반 스테이지인 경우
        handleNormalStageClear();  // 일반 스테이지 클리어 처리
        nextStage();               // 다음 스테이지로 진행
    }
}

// 현재 스테이지의 지형 이름을 반환하는 함수
const char* getCurrentTerrain(void) { // 현재 스테이지의 지형 이름을 반환하는 함수
    LOG_FUNCTION_EXECUTION("getCurrentTerrain"); // 함수 실행 로그 기록
    return gStage.terrainName; // 현재 스테이지의 지형 이름 반환
} 