#pragma once
#include <string>
using namespace std;

/*
 * 각 동작마다 최대 2개의 입력 키를 지정할 수 있도록 묶어둔 구조체입니다. 
 */
struct KeyBinding {
    int key1;

    // -1은 해당 동작에 2번째 키가 지정되지 않음을 의미합니다.
    int key2 = -1;

    // 게임 도중 입력한 키가 해당 동작에 지정된 키와 일치하는지 검사하고,
    // 그에 따른 참 / 거짓을 반환합니다.
    bool matches(int k) const { return k == key1 || (key2 != -1 && k == key2); }
};

/*
 * 동작에 지정되어 있는 입력 키의 정보를 관리하는 클래스입니다.
 * 일반 키 : ASCII 값(0 - 255)으로 저장합니다.
 * 확장 키(Home키, 방향키 등): 일반 키와 구분을 위해 256 + 스캔 코드로 저장합니다.
 */
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

    // 한 번의 키 입력을 읽어 인코딩된 값으로 변환합니다.
    static int readKey();

    // 인코딩된 키 값을 사람이 읽을 수 있는 이름으로 변환합니다.
    static string keyName(int key);

    // 해당 동작에 지정된 키를 출력하기 위해,
    // "Up / X" 와 같이 KeyBinding의 표시 문자열을 반환합니다.
    static string bindingName(const KeyBinding& b);
};
