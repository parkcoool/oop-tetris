#pragma once

#include "Block.h"

/*
 * 게임 도중 블록들이 쌓이는 게임판을 관리하는 클래스입니다.
 */
class Board {
private:
    // 벽을 포함해 21행 x 14열 크기를 갖는 배열을 이용하여 쌓이는 블록들을 관리합니다.
    // 해당 위치의 값이 -1이면 벽 또는 바닥을 의미하고, 0이면 빈칸을 의미하며, 
    // 그 외 양수는 고정된 블록의 색상 값을 의미합니다.
    int grid[21][14];

public:
    Board() = default;
    ~Board() = default;

    // 이동 또는 회전이 가능한지 확인하기 위하여, 특정 각도로 회전된 블록을 해당 위치에 놓았을 때
    // 충돌이 발생하는지 검사하고 그에 따른 참 / 거짓을 반환합니다.
    bool checkCollision(const Block& block, int nextX, int nextY, int nextAngle) const;

    // 움직임이 끝난 블록을 게임판에 고정시키고,
    // 그 블록의 색상 값을 게임판에 기록합니다.
    void mergeBlock(const Block& block);

    // 해당 행의 모든 열이 블록으로 빈틈없이 채워졌는지 확인합니다.
    bool checkRow(int row);

    // 꽉 찬 행을 제거하고 그 위에 있는 블록들을 한 칸씩 내립니다.
    void clearRow(int row);

    // 현재 게임판을 초기화합니다.
    void resetBoard();

    // 현재 게임판의 정보를 반환하는 Getter 함수입니다.
    const int (*getGrid() const)[14];
};
