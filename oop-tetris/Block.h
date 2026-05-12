#include <cstring>

enum class BlockColor {
    BLACK = 0,  /* 까망 */
    DARK_BLUE = 1,  /* 어두운 파랑 */
    DARK_GREEN = 2,  /* 어두운 초록 */
    DARK_SKY_BLUE = 3,  /* 어두운 하늘 */
    DARK_RED = 4,  /* 어두운 빨강 */
    DARK_VOILET = 5,  /* 어두운 보라 */
    DARK_YELLOW = 6,  /* 어두운 노랑 */
    GRAY = 7,  /* 회색 */
    DARK_GRAY = 8,  /* 어두운 회색 */
    BLUE = 9,  /* 파랑 */
    GREEN = 10, /* 초록 */
    SKY_BLUE = 11, /* 하늘 */
    RED = 12, /* 빨강 */
    VOILET = 13, /* 보라 */
    YELLOW = 14, /* 노랑 */
    WHITE = 15  /* 하양 */
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

    const int (*getShapeData() const)[4] {
        return shapeMatrix[angle];
    }
};