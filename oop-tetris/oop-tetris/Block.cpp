#include "Block.h"
#include <string> // memcpy

// 블록의 x, y 좌표를 각각 dx, dy만큼 이동시킵니다.
void Block::move(int dx, int dy) {
    x += dx;
    y += dy;
}

// 블록을 90도 회전시킵니다.
// 1(기본값)이면 시계 방향으로 회전시키고, -1이면 반시계 방향으로 회전시킵니다.
// 반시계 방향으로 회전하여 각도가 음수가 되어도 0~3 범위를 벗어나지 않도록 보정해줍니다.
void Block::rotate(int r) {
    angle = ((angle + r) % 4 + 4) % 4;
}


// 블록의 x좌표, y좌표 등의 정보를 반환하는 Getter 함수들입니다.
int Block::getX() const { return x; }

int Block::getY() const { return y; }

int Block::getAngle() const { return angle; }

Block::BlockColor Block::getColor() const { return color; }

// 특정 각도에 해당하는 블록의 정보를 반환합니다.
const int (*Block::getShapeData(int angle) const)[4] {
    return shapeMatrix[angle % 4];
}

// 현재 각도에 해당하는 블록의 정보를 반환합니다.
const int (*Block::getShapeData() const)[4] {
    return shapeMatrix[angle];
}

// 블록이 새로 생성될 때 위치 및 각도를 초기화합니다.
void Block::reset() {
    x = 5; y = -4;
    angle = 0;
}

// 해당 블록이 폭탄 블록인지 판단합니다.
bool Block::isBomb() const {
    return false; // 폭탄 블록이 아닌 기본 블록임을 의미합니다.
}

Block::Block(int startX, int startY, BlockColor c, const int data[][4][4])
    : x(startX), y(startY), angle(0), color(c) {
    memcpy(shapeMatrix, data, sizeof(shapeMatrix));
}