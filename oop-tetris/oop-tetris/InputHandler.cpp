#include "InputHandler.h"

int KeyConfig::readKey() {
    int key = _getch();
    if (key == 0 || key == 224) {
        return 256 + _getch();
    }
    // 알파벳인 경우 대소문자 구분 없이 인식
    if (key >= 'a' && key <= 'z') key = key - 'a' + 'A';
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

string KeyConfig::bindingName(const KeyBinding& b) {
    string s = keyName(b.key1);
    if (b.key2 != -1) s += " / " + keyName(b.key2);
    return s;
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

    case Command::ROTATE_CW:
        currentBlock->rotate();
        break;

    case Command::ROTATE_CCW:
        currentBlock->rotate(-1);
        break;

    case Command::MOVE_DOWN:
        currentBlock->move(0, 1);
        break;

    case Command::HARD_DROP:
        break;

    case Command::EXIT:
        break;

    default:
        break;
    }
}
