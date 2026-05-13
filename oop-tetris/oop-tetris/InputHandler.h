#pragma once

#include <conio.h>
#include "block.h"

enum class Command {
    NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    ROTATE,
    HARD_DROP,
    EXIT
};

class InputHandler {
public:
    Command getCommand() {
        if (!_kbhit()) return Command::NONE;

        int key = _getch();

        if (key == 0 || key == 224) {
            key = _getch();
            switch (key) {
            case 72: return Command::ROTATE;
            case 80: return Command::MOVE_DOWN;
            case 75: return Command::MOVE_LEFT;
            case 77: return Command::MOVE_RIGHT;
            }
        }

        if (key == 32) return Command::HARD_DROP;
        if (key == 27) return Command::EXIT; 

        return Command::NONE;
    }

    void processInput(Block* currentBlock, InputHandler& handler);
};