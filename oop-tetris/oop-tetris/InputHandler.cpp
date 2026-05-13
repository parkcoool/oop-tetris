#include "InputHandler.h"

void InputHandler::processInput(Block* currentBlock, InputHandler& handler)
{
    Command cmd = handler.getCommand();

    switch (cmd) {
    case Command::MOVE_LEFT:
        currentBlock->move(-1, 0);
        break;

    case Command::MOVE_RIGHT:
        currentBlock->move(1, 0);
        break;

    case Command::ROTATE:
        currentBlock->rotate();
        break;

    case Command::MOVE_DOWN:
        currentBlock->move(0, 1);
        break;

    case Command::HARD_DROP:
        // 후에 Board 클래스 구현 후 구현해야됨.
        break;

    case Command::EXIT:
        break;

    default:
        break;
    }
}
