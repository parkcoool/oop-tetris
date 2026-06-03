#include "Board.h"
#include "Block.h"
#include <windows.h> 
#include <mmsystem.h> // PlaySound

#pragma comment(lib, "winmm.lib")

static constexpr int WALL = -1;

// 이동 또는 회전이 가능한지 확인하기 위하여, 특정 각도로 회전된 블록을 해당 위치에 놓았을 때
// 충돌이 발생하는지 검사하고 그에 따른 참 / 거짓을 반환합니다.
bool Board::checkCollision(const Block& block, int nextX, int nextY, int nextAngle) const {
    const int (*shape)[4] = block.getShapeData(nextAngle); // 특정 각도로 회전된 블록을 가져옵니다.

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (shape[row][col] == 0) // 현재 블록의 모양이 아닌 위치는 확인하지 않습니다.
                continue;

            int boardRow = nextY + row;
            int boardCol = nextX + col;

            // 1. 벽 또는 바닥과 겹치거나 그 밖으로 나간 경우, 충돌한 것으로 처리합니다.
            if (boardRow >= 20 || boardCol <= 0 || boardCol >= 13)
                return true;

            // 2. 천장(게임판의 맨 위)보다 위에 있는 경우는 확인하지 않습니다.
            if (boardRow < 0)
                continue;

            // 3. 게임판에 고정된 블록과 겹치는 경우, 충돌한 것으로 처리합니다.
            if (grid[boardRow][boardCol] != 0)
                return true;
        }
    }
    return false; // 충돌하지 않았으므로, 블록이 해당 위치로 이동할 수 있습니다.
}

// 움직임이 끝난 블록을 게임판에 고정시키고,
// 그 블록의 색상 값을 게임판에 기록합니다.
void Board::mergeBlock(const Block& block) {
    const int (*shape)[4] = block.getShapeData();
    int x = block.getX();
    int y = block.getY();
    int color = static_cast<int>(block.getColor()); // 블록의 색상 값을 정수로 형변환하여 가져옵니다.

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (shape[row][col] != 0) {
                grid[y + row][x + col] = color; // 블록이 고정된 위치에 색상 값을 기록합니다.
            }
        }
    }
}

// 해당 행의 모든 열이 블록으로 빈틈없이 채워졌는지 확인합니다.
bool Board::checkRow(int row)
{
    for (int col = 1; col < 13; ++col) {
        if (grid[row][col] == 0) {
            return false;
        }
    }

    // 비어 있는 칸이 존재하지 않는, 꽉 찬 줄임을 의미합니다.
    return true;
}

// 꽉 찬 행을 제거하고 그 위에 있는 블록들을 한 칸씩 내립니다.
void Board::clearRow(int row) {

    // 해당 행 위에 있는 모든 블록들을 한 칸씩 내림.
    for (int k = row; k > 0; --k) {
        for (int col = 1; col < 13; ++col)
            grid[k][col] = grid[k - 1][col];
    }

    // 가장 위에 있는 행은 내려올 블록이 없으므로 비웁니다.
    for (int col = 1; col < 13; ++col)
        grid[0][col] = 0;
    PlaySound(TEXT("se_game_single.wav"), NULL, SND_FILENAME | SND_ASYNC); // 효과음을 비동기 방식으로 재생합니다.
}

// 현재 게임판을 초기화합니다.
void Board::resetBoard()
{
    for (int row = 0; row < 21; ++row) {
        for (int col = 0; col < 14; ++col) {
            if (col == 0 || col == 13 || row == 20)
                grid[row][col] = WALL;
            else
                grid[row][col] = 0;
        }
    }
}

// 현재 게임판의 정보를 반환하는 Getter 함수입니다.
const int (*Board::getGrid() const)[14] {
    return grid;
}
