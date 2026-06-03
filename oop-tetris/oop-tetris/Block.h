#pragma once

/*
 * 블록들의 공통된 속성과 기능들을 묶어서 정의한 클래스입니다.
 * 회전 등을 가상 함수로 정의하여 자식 클래스가 재정의할 수 있도록 합니다.
 */
class Block {
public:
    // 블록의 색상 값들을 열거형으로 정의합니다.
    enum class BlockColor {
        BLACK, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE,
        DARK_RED, DARK_VOILET, DARK_YELLOW, GRAY,
        DARK_GRAY, BLUE, GREEN, SKY_BLUE,
        RED, VOILET, YELLOW, WHITE
    };

    virtual ~Block() {}

    // 블록의 x, y 좌표를 각각 dx, dy만큼 이동시킵니다.
    void move(int dx, int dy);

    // 블록을 90도 회전시킵니다.
    // 1(기본값)이면 시계 방향으로 회전시키고, -1이면 반시계 방향으로 회전시킵니다.
    // 반시계 방향으로 회전하여 각도가 음수가 되어도 0~3 범위를 벗어나지 않도록 보정해줍니다.
    virtual void rotate(int r = 1);

    // 블록의 x좌표, y좌표 등의 정보를 반환하는 Getter 함수들입니다.
    int getX() const;
    int getY() const;
    int getAngle() const;
    BlockColor getColor() const;

    // 특정 각도에 해당하는 블록의 정보를 반환합니다.
    virtual const int (*getShapeData(int angle) const)[4];

    // 현재 각도에 해당하는 블록의 정보를 반환합니다.
    const int (*getShapeData() const)[4];

    // 블록이 새로 생성될 때 위치 및 각도를 초기화합니다.
    void reset();

    // 해당 블록이 폭탄 블록인지 판단합니다.
    virtual bool isBomb() const;

protected:
    int x, y;
    int angle;
    int shapeMatrix[4][4][4];
    BlockColor color;

    Block(int startX, int startY, BlockColor c, const int data[][4][4]);
};