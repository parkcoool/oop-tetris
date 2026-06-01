#pragma once
#include <Windows.h>
#include <chrono>
#include "Block.h"
#include "Board.h"
#include "ConsoleRenderer.h"
#include "InputHandler.h"
#include "KeyConfig.h"
#include "StageData.h"

class GameManager {
private:
	Board board;
	Block* currentBlock = nullptr, * nextBlock = nullptr, * holdBlock = nullptr;
	int ghostY;
	Block* randomBlocks[7];
	ConsoleRenderer renderer;
	KeyConfig keyConfig;
	InputHandler inputHandler;
	std::chrono::steady_clock::time_point startTime;

	// 레벨별 스테이지의 난이도 정보를 저장한다.
	StageData stageData[10] = {
		{40, 20, 20}, {38, 18, 20}, {35, 18, 20}, {30, 17, 20}, {25, 16, 20},
		{20, 14, 20}, {15, 14, 20}, {10, 13, 20}, {6, 12, 20}, {4, 11, 99999}
	};

	int score, level, clearedLines;
	bool isGameOver, isRestarting;
	
	// 하나의 블록당 한 번만 홀드가 가능하다.
	bool canHold = true;
public:
	GameManager();
	~GameManager();

	// 게임을 끝날 때까지 실행한다.
	void run();

	// 게임 데이터를 초기 상태로 되돌린다.
	void initGame();
	
	// 블록의 회전, 왼쪽 이동, 오른쪽 이동 및 그로 인한 상태 변화를 갱신한다.
	void update(int x, int angle);

	// 블록의 낙하 및 그로 인한 상태 변화를 갱신한다.
	// 블록이 고정되어 다음 블록으로 넘어간 경우 true 반환
	bool down(bool forceLock = false);

	// 7개의 블록들 중 하나를 생성하여 반환한다.
	Block* makeNewBlock();

	// 유령 블록의 위치를 반환한다.
	int getGhostPos(const Block& block);

	// 현재 블록을 홀드 슬롯에 저장한다.
	void hold();

private:
	void drawActiveBlock();

	// 현재 블록이 바닥/고정 블록 위에 닿아 있는지 확인한다.
	bool isCurrentBlockGrounded() const;

	// 현재 블록을 보드에 고정하고 다음 블록으로 교체한다.
	bool lockCurrentBlock();

	// 락다운 딜레이 시간(ms)
	static constexpr ULONGLONG LOCK_DELAY_MS = 500;
	bool lockDelayActive = false;
	ULONGLONG lockDelayStartTick = 0;
};

