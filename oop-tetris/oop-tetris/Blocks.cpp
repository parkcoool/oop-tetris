#include "Blocks.h"


// ■■■■

IBlock::IBlock(int startX, int startY)
    : Block(startX, startY, BlockColor::SKY_BLUE, I_DATA) { }

const int IBlock::I_DATA[4][4][4] = {
        { {0,0,0,0},
          {1,1,1,1},
          {0,0,0,0},
          {0,0,0,0} },

        { {0,0,1,0},
          {0,0,1,0},
          {0,0,1,0},
          {0,0,1,0} },

        { {0,0,0,0},
          {0,0,0,0},
          {1,1,1,1},
          {0,0,0,0} },

        { {0,1,0,0},
          {0,1,0,0},
          {0,1,0,0},
          {0,1,0,0} }
};


// ■■
// ■■

OBlock::OBlock(int startX, int startY)
    : Block(startX, startY, BlockColor::YELLOW, O_DATA) { }

// 회전해도 모양이 변하지 않으므로 아무것도 수행하지 않는 함수로 재정의합니다.
void OBlock::rotate(int r) {}

// 블록 상태가 하나만 존재하므로 인덱스 0의 블록만 반환합니다.
const int (*OBlock::getShapeData(int angle) const)[4] {
    return shapeMatrix[0];
}

const int OBlock::O_DATA[1][4][4] = {
        { {0,0,0,0},
          {0,1,1,0},
          {0,1,1,0},
          {0,0,0,0} }
};


//   ■
// ■■■

TBlock::TBlock(int startX, int startY)
    : Block(startX, startY, BlockColor::VOILET, T_DATA) { }

const int TBlock::T_DATA[4][4][4] = {
        { {0,0,0,0},
          {0,1,0,0},
          {1,1,1,0},
          {0,0,0,0} },

        { {0,0,0,0},
          {0,1,0,0},
          {0,1,1,0},
          {0,1,0,0} },

        { {0,0,0,0},
          {0,0,0,0},
          {1,1,1,0},
          {0,1,0,0} },

        { {0,0,0,0},
          {0,1,0,0},
          {1,1,0,0},
          {0,1,0,0} }
};


// ■
// ■■■

JBlock::JBlock(int startX, int startY)
    : Block(startX, startY, BlockColor::BLUE, J_DATA) { }

const int JBlock::J_DATA[4][4][4] = {
        { {0,0,0,0},
          {1,0,0,0},
          {1,1,1,0},
          {0,0,0,0} },

        { {0,0,0,0},
          {0,1,1,0},
          {0,1,0,0},
          {0,1,0,0} },

        { {0,0,0,0},
          {0,0,0,0},
          {1,1,1,0},
          {0,0,1,0} },

        { {0,0,0,0},
          {0,1,0,0},
          {0,1,0,0},
          {1,1,0,0} },
};


//     ■
// ■■■

LBlock::LBlock(int startX, int startY)
    : Block(startX, startY, BlockColor::DARK_YELLOW, L_DATA) { }

const int LBlock::L_DATA[4][4][4] = {
        { {0,0,0,0},
          {0,0,1,0},
          {1,1,1,0},
          {0,0,0,0} },

        { {0,0,0,0},
          {0,1,0,0},
          {0,1,0,0},
          {0,1,1,0} },

        { {0,0,0,0},
          {0,0,0,0},
          {1,1,1,0},
          {1,0,0,0} },

        { {0,0,0,0},
          {1,1,0,0},
          {0,1,0,0},
          {0,1,0,0} },
};


// ■■ 
//   ■■

ZBlock::ZBlock(int startX, int startY)
    : Block(startX, startY, BlockColor::RED, Z_DATA) {
}

const int ZBlock::Z_DATA[4][4][4] = {
        { {0,0,0,0},
          {1,1,0,0},
          {0,1,1,0},
          {0,0,0,0} },

        { {0,0,0,0},
          {0,0,1,0},
          {0,1,1,0},
          {0,1,0,0} },

        { {0,0,0,0},
          {0,0,0,0},
          {1,1,0,0},
          {0,1,1,0} },

        { {0,0,0,0},
          {0,1,0,0},
          {1,1,0,0},
          {1,0,0,0} }
};


//   ■■
// ■■ 

SBlock::SBlock(int startX, int startY)
    : Block(startX, startY, BlockColor::GREEN, S_DATA) {
}

const int SBlock::S_DATA[4][4][4] = {
        { {0,0,0,0},
          {0,1,1,0},
          {1,1,0,0},
          {0,0,0,0} },

        { {0,0,0,0},
          {0,1,0,0},
          {0,1,1,0},
          {0,0,1,0} },

        { {0,0,0,0},
          {0,0,0,0},
          {0,1,1,0},
          {1,1,0,0} },

        { {0,0,0,0},
          {1,0,0,0},
          {1,1,0,0},
          {0,1,0,0} }
};

// 폭탄 블록(■)
BombBlock::BombBlock(int startX, int startY)
    : Block(startX, startY, BlockColor::WHITE, BB_DATA)
{

}

const int BombBlock::BB_DATA[1][4][4] = {
        { {0,0,0,0},
          {0,1,0,0},
          {0,0,0,0},
          {0,0,0,0} },

};

// 회전해도 모양이 변하지 않으므로 아무것도 수행하지 않는 함수로 재정의합니다.
void BombBlock::rotate(int r) {}

// 블록 상태가 하나만 존재하므로 인덱스 0의 블록만 반환합니다.
const int (*BombBlock::getShapeData(int angle) const)[4] {
    return BB_DATA[0];
}

// 폭탄 블록임을 판별하기 위하여 true를 반환합니다.
bool BombBlock::isBomb() const {
    return true;
}
