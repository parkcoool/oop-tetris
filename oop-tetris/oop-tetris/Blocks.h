#pragma once

#include "Block.h"

//■■■■ 모양

class IBlock : public Block {
public:
    IBlock(int startX, int startY)
        : Block(startX, startY, BlockColor::SKY_BLUE, I_DATA) {
    }
    // 겹치는 모양 제거
    void rotate() override {
        angle = (angle + 1) % 2;
    }
private:
    static inline int I_DATA[2][4][4] = {
        { {1,0,0,0},
          {1,0,0,0}, 
          {1,0,0,0}, 
          {1,0,0,0} },

        { {1,1,1,1},
          {0,0,0,0},
          {0,0,0,0},
          {0,0,0,0} }
    };

};


//■■
//■■ 모양

class OBlock : public Block {
public:
    OBlock(int startX, int startY)
        : Block(startX, startY, BlockColor::YELLOW, O_DATA) {
    }

    // 겹치는 모양 제거
    void rotate() override { }

private:
    static inline int O_DATA[4][4][4] = {
        { {1,1,0,0}, 
          {1,1,0,0}, 
          {0,0,0,0}, 
          {0,0,0,0} }
    };
};


//■
//■■
//■ 모양

class TBlock : public Block {
public:
    TBlock(int startX, int startY)
        : Block(startX, startY, BlockColor::VOILET, T_DATA) {
    }

    // 기본 rotate 사용

private:
    static inline int T_DATA[4][4][4] = {
        { {0,1,0,0}, 
          {1,1,1,0}, 
          {0,0,0,0}, 
          {0,0,0,0} },

        { {0,1,0,0}, 
          {0,1,1,0}, 
          {0,1,0,0}, 
          {0,0,0,0} },

        { {1,1,1,0}, 
          {0,1,0,0}, 
          {0,0,0,0}, 
          {0,0,0,0} },

        { {0,1,0,0}, 
          {1,1,0,0}, 
          {0,1,0,0}, 
          {0,0,0,0} }
    };
};


//■□□
//■■■ 모양

class JBlock : public Block {
public:
    JBlock(int startX, int startY)
        : Block(startX, startY, BlockColor::BLUE, J_DATA) {
    }

    // 기본 rotate 사용

private:
    static inline int J_DATA[4][4][4] = {
        { {1,0,0,0}, 
          {1,1,1,0}, 
          {0,0,0,0}, 
          {0,0,0,0} },

        { {0,1,1,0}, 
          {0,1,0,0}, 
          {0,1,0,0}, 
          {0,0,0,0} },

        { {1,1,1,0},
          {0,0,1,0}, 
          {0,0,0,0}, 
          {0,0,0,0} },

        { {0,1,0,0}, 
          {0,1,0,0}, 
          {1,1,0,0}, 
          {0,0,0,0} }
    };
};

//□□■
//■■■ 모양

class LBlock : public Block {
public:
    LBlock(int startX, int startY)
        : Block(startX, startY, BlockColor::DARK_YELLOW, L_DATA) {
    }

    // 기본 rotate 사용

private:
    static inline int L_DATA[4][4][4] = {
        { {0,0,1,0}, 
          {1,1,1,0}, 
          {0,0,0,0}, 
          {0,0,0,0} },

        { {0,1,0,0},
          {0,1,0,0},
          {0,1,1,0},
          {0,0,0,0} },

        { {1,1,1,0}, 
          {1,0,0,0}, 
          {0,0,0,0},  
          {0,0,0,0} },

        { {1,1,0,0},
          {0,1,0,0},
          {0,1,0,0}, 
          {0,0,0,0} }
    };
};

//■■□
//□■■ 모양

class ZBlock : public Block {
public:
    ZBlock(int startX, int startY)
        : Block(startX, startY, BlockColor::RED, Z_DATA) {
    }

    // 겹치는 모양 제거
    void rotate() override {
        angle = (angle + 1) % 2;
    }

private:
    static inline int Z_DATA[2][4][4] = {
        { {1,1,0,0},
          {0,1,1,0},
          {0,0,0,0}, 
          {0,0,0,0} },

        { {0,0,1,0},
          {0,1,1,0}, 
          {0,1,0,0}, 
          {0,0,0,0} }
    };
};

//□■■
//■■□ 모양

class SBlock : public Block {
public:
    SBlock(int startX, int startY)
        : Block(startX, startY, BlockColor::GREEN, S_DATA) {
    }

    // 겹치는 모양 제거
    void rotate() override {
        angle = (angle + 1) % 2;
    }

private:
    static inline int S_DATA[2][4][4] = {
        { {0,1,1,0},
          {1,1,0,0},
          {0,0,0,0},
          {0,0,0,0} },

        { {0,1,0,0},
          {0,1,1,0},
          {0,0,1,0},
          {0,0,0,0} }
    };
};