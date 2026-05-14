#pragma once

#include <cstring>

enum class BlockColor {
    BLACK, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE,
    DARK_RED, DARK_VOILET, DARK_YELLOW, GRAY,
    DARK_GRAY, BLUE, GREEN, SKY_BLUE,
    RED, VOILET, YELLOW, WHITE
};

class Block {
protected:
    int x, y;
    int angle;
    int shapeMatrix[4][4][4];
    BlockColor color;

    Block(int startX, int startY, BlockColor c, const int data[4][4][4])
        : x(startX), y(startY), angle(0), color(c) {
        memcpy(shapeMatrix, data, sizeof(shapeMatrix));
    }

public:
    virtual ~Block() {}

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    // 기본 rotate 설정
    virtual void rotate() {
        angle = (angle + 1) % 4;
    }

    // Getter 함수 모음
    int getX() const { return x; }
    int getY() const { return y; }
    int getAngle() const { return angle; }
    BlockColor getColor() const { return color; }

    const int (*getShapeData(int angle) const)[4] {
        return shapeMatrix[angle];
    }

    const int (*getShapeData() const)[4] {
        return shapeMatrix[angle];
    }
};