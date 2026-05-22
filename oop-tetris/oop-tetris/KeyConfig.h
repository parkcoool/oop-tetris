#pragma once
#include <string>
using namespace std;

// 일반 키: ASCII 값(0-255)으로 저장.
// 확장 키(0 또는 224 프리픽스): 256 + 스캔코드로 저장.
struct KeyConfig {
    int rotate    = 256 + 72;  // 위 화살표
    int moveDown  = 256 + 80;  // 아래 화살표
    int moveLeft  = 256 + 75;  // 왼쪽 화살표
    int moveRight = 256 + 77;  // 오른쪽 화살표
    int hardDrop  = 32;        // Space
    int pause     = 27;        // ESC

    // 한 번의 키 입력을 읽어 인코딩된 값으로 반환한다.
    static int readKey();

    // 인코딩된 키 값을 사람이 읽을 수 있는 이름으로 반환한다.
    static string keyName(int key);
};
