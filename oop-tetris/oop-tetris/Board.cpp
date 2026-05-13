#include "Board.h"
#include "Block.h"

static constexpr int WALL = -1;

Board::Board() {
    for (int row = 0; row < 21; ++row) {
        for (int col = 0; col < 14; ++col) {
            if (col == 0 || col == 13 || row == 20)
                grid[row][col] = WALL;
            else
                grid[row][col] = 0;
        }
    }
}

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

            // 블록이 아직 플레이 영역 위에 있지 않으면 충돌로 처리하지 않음.
            if (boardRow < 0)
                continue;

            // 플레이 영역 가로 범위 초과 또는 바닥 아래.
            if (boardRow >= 21 || boardCol < 0 || boardCol >= 14)
                return true;

            // 이미 점유된 셀 (벽 또는 고정된 블록).
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

// 꽉 찬 줄(열 1~12)을 모두 제거하고 위 행들을 내린 뒤 제거된 줄 수를 반환
int Board::clearFullLines() {
    int cleared = 0;

    for (int row = 0; row < 20; ++row) {
        bool full = true;
        for (int col = 1; col < 13; ++col) {
            if (grid[row][col] == 0) {
                full = false;
                break;
            }
        }

        if (full) {
            ++cleared;
            // 해당 행 위의 모든 행을 한 칸씩 내림.
            for (int k = row; k > 0; --k) {
                for (int col = 1; col < 13; ++col)
                    grid[k][col] = grid[k - 1][col];
            }
            // 최상단 행을 빈칸으로 초기화.
            for (int col = 1; col < 13; ++col)
                grid[0][col] = 0;
            // 새 행이 내려왔으므로 같은 인덱스를 다시 검사.
            --row;
        }
    }
    return cleared;
}

const int (*Board::getGrid() const)[14] {
    return grid;
}
