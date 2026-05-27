#include "Blocks.h"

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

OBlock::OBlock(int startX, int startY)
    : Block(startX, startY, BlockColor::YELLOW, O_DATA) { }

// 겹치는 모양 제거
void OBlock::rotate(int r) {}

const int (*OBlock::getShapeData(int angle) const)[4] {
    return shapeMatrix[0];
}

const int OBlock::O_DATA[1][4][4] = {
        { {0,0,0,0},
          {0,1,1,0},
          {0,1,1,0},
          {0,0,0,0} }
};

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