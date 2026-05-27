#pragma once
#include "Block.h"

// ■■■■ 모양

class IBlock : public Block {
public:
    IBlock(int startX, int startY);
private:
    static const int I_DATA[4][4][4];
};


// ■■
// ■■ 모양

class OBlock : public Block {
public:
    OBlock(int startX, int startY);
    void rotate(int r = 1) override;
    const int (*getShapeData(int angle) const)[4] override;
private:
    static const int O_DATA[1][4][4];
};

//  ■
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

//   ■
// ■■■

class LBlock : public Block {
public:
    LBlock(int startX, int startY);
private:
    static const int L_DATA[4][4][4];
};

// ■■ 
//  ■■

class ZBlock : public Block {
public:
    ZBlock(int startX, int startY);
private:
    static const int Z_DATA[4][4][4];
};

//  ■■
// ■■ 

class SBlock : public Block {
public:
    SBlock(int startX, int startY);
private:
    static const int S_DATA[4][4][4];
};