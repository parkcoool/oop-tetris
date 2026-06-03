#include "InputHandler.h"

// 한 번의 키 입력을 읽어 인코딩된 값으로 변환합니다.
int KeyConfig::readKey() {
    int key = _getch();
    if (key == 0 || key == 224) { // 확장 키는 0 또는 224를 먼저 반환하므로 이를 확인합니다.
        return 256 + _getch(); // 확장 키는 스캔 코드에 256을 더하여 구분합니다.
    }

    // 알파벳의 경우, 소문자를 입력해도 대문자로 변환해
    // 대소문자를 구분하지 않고 인식이 가능하도록 합니다.
    if (key >= 'a' && key <= 'z') key = key - 'a' + 'A';
    return key;
}

// 인코딩된 키 값을 사람이 읽을 수 있는 이름으로 변환합니다.
string KeyConfig::keyName(int key) {
    if (key > 255) { // 확장 키는 256 + 스캔 코드 로 따로 인코딩하였기 때문에 이를 확인합니다.
        int scan = key - 256; // 실제 스캔 코드를 가져옵니다.
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
    switch (key) { // 255 이하 ASCII 값을 가지는 일반 키들을 처리합니다.
    case 32: return "Space";
    case 27: return "ESC";
    case 13: return "Enter";
    case 9:  return "Tab";
    case 8:  return "Backspace";
    default:
        if (key >= 33 && key < 127) return string(1, (char)key); // 알파벳, 숫자, 일부 기호 등은 그대로 변환하도록 합니다.
        return "Key(" + to_string(key) + ")";
    }
}

// 해당 동작에 지정된 키를 조합하여 하나의 문자열을 반환합니다.
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
