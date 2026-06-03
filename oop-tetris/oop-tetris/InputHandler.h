#pragma once

#include <conio.h>
#include "Block.h"
#include "KeyConfig.h"

/*
 * 입력된 키를 게임 내의 명령어로 변환해 주도록 열거형으로 정의합니다.
 */
enum class Command {
    NONE,
    MOVE_LEFT,  // 왼쪽으로 이동
    MOVE_RIGHT, // 오른쪽으로 이동
    MOVE_DOWN,  // 아래로 한 칸 이동
    ROTATE_CW,  // 시계 방향 회전
    ROTATE_CCW, // 반시계 방향 회전
    HARD_DROP,  // 아래로 내려갈 수 없을 때까지 이동
    HOLD,       // 블록 홀드
    EXIT        // 일시정지 메뉴
};

/*
 * 사용자로부터 키 입력을 받은 후 이를 KeyConfig에 설정되어 있는 키 정보에 따라
 * Command에 있는 명령어로 변환해주는 클래스입니다.
 */
class InputHandler {
private:
    const KeyConfig* config = nullptr;
public:
    void setConfig(const KeyConfig& cfg) { config = &cfg; }

    // 입력한 키 값을 readKey()를 통해 인코딩된 값으로 변환한 후,
    // 동작에 바인딩되어 있는 키와 매칭하여 그에 따른 명령어로 변환합니다.
    Command getCommand() {
        int key = KeyConfig::readKey();
        if (config) {
            if (config->rotateCW.matches(key))   return Command::ROTATE_CW;
            if (config->rotateCCW.matches(key))  return Command::ROTATE_CCW;
            if (config->moveDown.matches(key))    return Command::MOVE_DOWN;
            if (config->moveLeft.matches(key))    return Command::MOVE_LEFT;
            if (config->moveRight.matches(key))   return Command::MOVE_RIGHT;
            if (config->hardDrop.matches(key))    return Command::HARD_DROP;
            if (config->pause.matches(key))       return Command::EXIT;
            if (config->hold.matches(key))        return Command::HOLD;
        }
        return Command::NONE; // 일치하는 키가 존재하지 않는 경우.
    }

    void processInput(Block* currentBlock, InputHandler& handler);
};