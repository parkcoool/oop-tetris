#pragma once

#include <conio.h>
#include "Block.h"
#include "KeyConfig.h"

enum class Command {
    NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    ROTATE_CW,
    ROTATE_CCW,
    HARD_DROP,
    HOLD,
    EXIT
};

class InputHandler {
private:
    const KeyConfig* config = nullptr;
public:
    void setConfig(const KeyConfig& cfg) { config = &cfg; }

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
        return Command::NONE;
    }

    void processInput(Block* currentBlock, InputHandler& handler);
};