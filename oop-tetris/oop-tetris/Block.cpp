#include "Block.h"
#include <string> // memcpy

void Block::move(int dx, int dy) {
    x += dx;
    y += dy;
}

void Block::rotate(int r) {
    angle = ((angle + r) % 4 + 4) % 4;
}

int Block::getX() const { return x; }

int Block::getY() const { return y; }

int Block::getAngle() const { return angle; }

Block::BlockColor Block::getColor() const { return color; }

const int (*Block::getShapeData(int angle) const)[4] {
    return shapeMatrix[angle % 4];
}

const int (*Block::getShapeData() const)[4] {
    return shapeMatrix[angle];
}

void Block::reset() {
    x = 5; y = -4;
    angle = 0;
}

Block::Block(int startX, int startY, BlockColor c, const int data[][4][4])
    : x(startX), y(startY), angle(0), color(c) {
    memcpy(shapeMatrix, data, sizeof(shapeMatrix));
}