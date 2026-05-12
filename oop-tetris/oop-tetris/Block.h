#pragma once

enum class Color {
    BLACK         = 0,
    DARK_BLUE     = 1,
    DARK_GREEN    = 2,
    DARK_SKY_BLUE = 3,
    DARK_RED      = 4,
    DARK_VIOLET   = 5,
    DARK_YELLOW   = 6,
    GRAY          = 7,
    DARK_GRAY     = 8,
    BLUE          = 9,
    GREEN         = 10,
    SKY_BLUE      = 11,
    RED           = 12,
    VIOLET        = 13,
    YELLOW        = 14,
    WHITE         = 15,
};

class Block {
protected:
    int x;                    // 현재 기준 열 좌표
    int y;                    // 현재 기준 행 좌표
    int angle;                // 회전 상태 (0~3)
    int shapeMatrix[4][4][4]; // 블록 형태 데이터 ? [각도][행][열], 자식 클래스에서 초기화
    Color color;              // 블록 색상

public:
    virtual ~Block() = default;

    void move(int dx, int dy) { x += dx; y += dy; }

    virtual void rotate() = 0;

    int   getX()     const { return x; }
    int   getY()     const { return y; }
    int   getAngle() const { return angle; }
    Color getColor() const { return color; }

    // 현재 angle 기준 형태 데이터 반환
    const int (*getShapeData() const)[4] {
        return shapeMatrix[angle];
    }

    // 임의의 angle 기준 형태 데이터 반환
    const int (*getShapeData(int targetAngle) const)[4] {
        return shapeMatrix[targetAngle];
    }
};
