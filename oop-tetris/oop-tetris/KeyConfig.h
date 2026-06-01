#pragma once
#include <string>
using namespace std;

struct KeyBinding {
    int key1 = -1;
    int key2 = -1;  // -1 means unassigned
    bool matches(int k) const { return k == key1 || (key2 != -1 && k == key2); }
};

// 일반 키: ASCII 값(0-255)으로 저장.
// 확장 키(0 또는 224 프리픽스): 256 + 스캔코드로 저장.
class KeyConfig {
public:
    KeyBinding rotateCW   = {256 + 72, 'X'};  // 위 화살표, X
    KeyBinding rotateCCW  = {'Z', -1};          // Z
    KeyBinding moveDown   = {256 + 80, -1};    // 아래 화살표
    KeyBinding moveLeft   = {256 + 75, -1};    // 왼쪽 화살표
    KeyBinding moveRight  = {256 + 77, -1};    // 오른쪽 화살표
    KeyBinding hold       = {'C',  -1};           // C
    KeyBinding hardDrop   = {32, -1};           // Space
    KeyBinding pause      = {27, -1};           // ESC

    // 한 번의 키 입력을 읽어 인코딩된 값으로 반환한다.
    static int readKey();

    // 인코딩된 키 값을 사람이 읽을 수 있는 이름으로 반환한다.
    static string keyName(int key);

    // KeyBinding의 표시 문자열을 반환한다 (예: "Up / X").
    static string bindingName(const KeyBinding& b);
};
