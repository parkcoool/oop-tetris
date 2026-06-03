#pragma once
#include "Block.h"

/*
 * 블록들이 이동하고 회전한다는 공통된 기능을 가지고 있으므로,
 * 8종류의 블록들을 부모 클래스 Block을 상속받은 클래스로 정의합니다.
 * OBlock과 BombBlock은 회전해도 모양이 변하지 않으므로 관련 함수를 재정의합니다.
 */


// ■■■■

class IBlock : public Block {
public:
    IBlock(int startX, int startY);
private:
    static const int I_DATA[4][4][4];
};


// ■■
// ■■

class OBlock : public Block {
public:
    OBlock(int startX, int startY);
    void rotate(int r = 1) override;
    const int (*getShapeData(int angle) const)[4] override;
private:
    static const int O_DATA[1][4][4]; // 회전해도 모양이 변하지 않으므로 상태가 하나만 존재합니다.
};


//   ■
// ■■■

class TBlock : public Block {
public:
    TBlock(int startX, int startY);
private:
    static const int T_DATA[4][4][4];
};


// ■
// ■■■

class JBlock : public Block {
public:
    JBlock(int startX, int startY);
private:
    static const int J_DATA[4][4][4];
};


//     ■
// ■■■

class LBlock : public Block {
public:
    LBlock(int startX, int startY);
private:
    static const int L_DATA[4][4][4];
};


// ■■ 
//   ■■

class ZBlock : public Block {
public:
    ZBlock(int startX, int startY);
private:
    static const int Z_DATA[4][4][4];
};


//   ■■
// ■■ 

class SBlock : public Block {
public:
    SBlock(int startX, int startY);
private:
    static const int S_DATA[4][4][4];
};


// 폭탄 블록(■)
class BombBlock : public Block {

public:
    BombBlock(int startX, int startY);
    void rotate(int r = 1) override;
    const int (*getShapeData(int angle) const)[4] override;

    // 폭탄 블록임을 판별하기 위하여 true를 반환합니다.
    bool isBomb() const override;
private:
    static const int BB_DATA[1][4][4]; // 회전해도 모양이 변하지 않으므로 상태가 하나만 존재합니다.
};