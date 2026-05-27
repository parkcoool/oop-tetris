#pragma once

#include "Block.h"

class Board {
private:
    // 0: 빈칸, -1: 벽, 양수: 고정된 블록의 색상값
    // 0~19행: 플레이 영역, 20행: 바닥
    // 0열 및 13열: 좌우 벽, 1~12열: 플레이 영역
    int grid[21][14];

public:
    Board() = default;
    ~Board() = default;

    // (nextX, nextY)에서 nextAngle 각도로 block을 놓았을 때
    // 벽 또는 이미 고정된 블록과 충돌하면 true 반환.
    bool checkCollision(const Block& block, int nextX, int nextY, int nextAngle) const;

    // 현재 위치에서 block을 그리드에 고정하고 색상값을 기록
    void mergeBlock(const Block& block);

    // 꽉 찬 줄(열 1~12)이 있는지 확인한다.
    bool checkRow(int row);

    // 꽉 찬 줄을 제거하고 위에 있는 행들을 내린다.
    void clearRow(int row);

    // 현재 그리드를 초기화
    void resetBoard();

    const int (*getGrid() const)[14];
};
