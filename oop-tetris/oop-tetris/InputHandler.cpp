#include "InputHandler.h"

int KeyConfig::readKey() {
    int key = _getch();
    if (key == 0 || key == 224) {
        return 256 + _getch();
    }
    return key;
}

string KeyConfig::keyName(int key) {
    if (key > 255) {
        int scan = key - 256;
        switch (scan) {
        case 72: return "Up";
        case 80: return "Down";
        case 75: return "Left";
        case 77: return "Right";
        case 71: return "Home";
        case 79: return "End";
        case 73: return "PgUp";
        case 81: return "PgDn";
        default: return "Ext(" + to_string(scan) + ")";
        }
    }
    switch (key) {
    case 32: return "Space";
    case 27: return "ESC";
    case 13: return "Enter";
    case 9:  return "Tab";
    case 8:  return "Backspace";
    default:
        if (key >= 33 && key < 127) return string(1, (char)key);
        return "Key(" + to_string(key) + ")";
    }
}

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
        // �Ŀ� Board Ŭ���� ���� �� �����ؾߵ�.
        break;

    case Command::EXIT:
        break;

    default:
        break;
    }
}
