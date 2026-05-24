#pragma once

#include <conio.h>
#include "Block.h"
#include "KeyConfig.h"

enum class Command {
    NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    ROTATE,
    HARD_DROP,
    HOLD,
    EXIT
};

class InputHandler {
    const KeyConfig* config = nullptr;
public:
    void setConfig(const KeyConfig& cfg) { config = &cfg; }

    Command getCommand() {
        int key = KeyConfig::readKey();
        if (config) {
            if (key == config->rotate)    return Command::ROTATE;
            if (key == config->moveDown)  return Command::MOVE_DOWN;
            if (key == config->moveLeft)  return Command::MOVE_LEFT;
            if (key == config->moveRight) return Command::MOVE_RIGHT;
            if (key == config->hardDrop)  return Command::HARD_DROP;
            if (key == config->pause)     return Command::EXIT;
            if (key == config->hold)      return Command::HOLD;
        }
        return Command::NONE;
    }

    void processInput(Block* currentBlock, InputHandler& handler);
};