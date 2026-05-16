#pragma once
#include "Block.h"
#include "Board.h"
#include "ConsoleRenderer.h"
#include "InputHandler.h"
#include "StageData.h"

class GameManager {
private:
	Board board;
	Block* currentBlock = nullptr, * nextBlock = nullptr;
	ConsoleRenderer renderer;
	InputHandler inputHandler;

	// 레벨별 스테이지의 난이도 정보를 저장한다.
	StageData* stageData[10] = {};

	int score, level, clearedLines;
	bool isGameOver;
public:
	GameManager();
	~GameManager();

	// 게임을 끝날 때까지 실행한다.
	void run();

	// 게임 데이터를 초기 상태로 되돌린다.
	void initGame();

	// 블록의 낙하 및 그로 인한 상태 변화를 갱신한다.
	// 충돌한 경우 true 반환
	bool update();

	// 7개의 블록들 중 하나를 생성하여 반환한다.
	Block* makeNewBlock();
};

