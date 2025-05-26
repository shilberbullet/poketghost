#ifndef ESCAPE_H
#define ESCAPE_H

#include "types.h"
#include "stage.h"

// 도망치기 시도 결과
#define ESCAPE_SUCCESS 103  // 도망 성공
#define ESCAPE_FAIL 0      // 도망 실패

// 도망치기 시도 함수
int tryToEscape(void);

// 보스 스테이지에서 도망치기 가능 여부 확인
int canEscapeFromStage(void);

#endif // ESCAPE_H 