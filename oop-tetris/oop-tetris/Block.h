#pragma once

class Block {
public:
    enum class BlockColor {
        BLACK, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE,
        DARK_RED, DARK_VOILET, DARK_YELLOW, GRAY,
        DARK_GRAY, BLUE, GREEN, SKY_BLUE,
        RED, VOILET, YELLOW, WHITE
    };

    virtual ~Block() {}

    void move(int dx, int dy);

    // 기본 rotate 설정
    virtual void rotate(int r = 1);

    // Getter 함수 모음
    int getX() const;
    int getY() const;
    int getAngle() const;
    BlockColor getColor() const;

    // 현재 블록의 정보를 반환하는 기본 설정
    virtual const int (*getShapeData(int angle) const)[4];
    const int (*getShapeData() const)[4];

    // 블록의 정보를 초기화
    void reset();
protected:
    int x, y;
    int angle;
    int shapeMatrix[4][4][4];
    BlockColor color;

    Block(int startX, int startY, BlockColor c, const int data[][4][4]);
};