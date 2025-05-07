#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "boss_monsters.h"

// 보스 요괴 데이터
BossMonster bossMonsters[] = {
    {
        "천둥의 지배자",
        "번개",
        200,
        80,
        40,
        "천둥 번개",
        "하늘을 지배하는 강력한 번개 요괴. 번개를 자유자재로 다룰 수 있다."
    },
    {
        "불꽃의 제왕",
        "불",
        180,
        90,
        35,
        "화염 폭발",
        "지옥의 불꽃을 다루는 요괴. 주변의 모든 것을 불태운다."
    },
    {
        "얼음의 여왕",
        "얼음",
        220,
        70,
        50,
        "절대 영도",
        "영하의 차가운 기운을 다루는 요괴. 모든 것을 얼어붙게 만든다."
    },
    {
        "대지의 거인",
        "땅",
        300,
        60,
        80,
        "지진",
        "대지를 움직이는 거대한 요괴. 강력한 방어력을 자랑한다."
    },
    {
        "바람의 수호자",
        "바람",
        150,
        100,
        30,
        "회오리",
        "바람을 자유자재로 다루는 요괴. 빠른 공격 속도가 특징이다."
    }
};

const int bossMonsterCount = sizeof(bossMonsters) / sizeof(BossMonster);

// 랜덤 보스 요괴 선택
BossMonster* getRandomBossMonster() {
    srand((unsigned int)time(NULL));
    int index = rand() % bossMonsterCount;
    return &bossMonsters[index];
}

// 보스 스테이지 확인
int isBossStage(int stageNumber) {
    return stageNumber % 10 == 0;
} 