#include "Board.h"
#include "Block.h"

static constexpr int WALL = -1;

// (nextX, nextY)에서 nextAngle 각도로 block을 놓았을 때
// 벽 또는 이미 고정된 블록과 충돌하면 true 반환.
bool Board::checkCollision(const Block& block, int nextX, int nextY, int nextAngle) const {
    const int (*shape)[4] = block.getShapeData(nextAngle);

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (shape[row][col] == 0)
                continue;

            int boardRow = nextY + row;
            int boardCol = nextX + col;

            // 플레이 영역 가로 범위 초과 또는 바닥 아래.
            if (boardRow >= 20 || boardCol <= 0 || boardCol >= 13)
                return true;

            // 블록이 아직 플레이 영역 위에 있지 않으면 충돌로 처리하지 않음.
            if (boardRow < 0)
                continue;

            // 이미 점유된 셀 (게임판에 고정된 블록).
            if (grid[boardRow][boardCol] != 0)
                return true;
        }
    }
    return false;
}

// 현재 위치에서 block을 그리드에 고정하고 색상값을 기록
void Board::mergeBlock(const Block& block) {
    const int (*shape)[4] = block.getShapeData();
    int x = block.getX();
    int y = block.getY();
    int color = static_cast<int>(block.getColor());

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (shape[row][col] != 0) {
                grid[y + row][x + col] = color;
            }
        }
    }
}

bool Board::checkRow(int row)
{
    for (int col = 1; col < 13; ++col) {
        if (grid[row][col] == 0) {
            return false;
        }
    }

    // 비어 있는 칸이 존재하지 않는 꽉 찬 줄임을 의미한다.
    return true;
}

// 꽉 찬 줄(열 1~12)이 있는지 확인한 다음 모두 제거하고
// 위에 있는 행들을 내린다.
void Board::clearRow(int row) {

    // 해당 행 위의 모든 행을 한 칸씩 내림.
    for (int k = row; k > 0; --k) {
        for (int col = 1; col < 13; ++col)
            grid[k][col] = grid[k - 1][col];
    }
    // 최상단 행을 빈칸으로 초기화.
    for (int col = 1; col < 13; ++col)
        grid[0][col] = 0;
}

// 현재 그리드를 초기화
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

const int (*Board::getGrid() const)[14] {
    return grid;
}
