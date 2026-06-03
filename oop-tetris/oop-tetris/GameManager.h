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
	// 게임판의 상태를 기록하고 관리하기 위한 Board 객체입니다.
	Board board;

	// 각각 현재 내려오는 블록, 다음으로 내려올 블록, 홀드한 블록입니다.
	// 여러 종류의 블록들을 하나의 변수로 처리할 수 있도록 Block 포인터 변수로 업캐스팅합니다.
	Block* currentBlock = nullptr, * nextBlock = nullptr, * holdBlock = nullptr;

	// 유령 블록의 y좌표(블록을 충돌하기 직전까지 내렸을 때의 y좌표)입니다.
	int ghostY;

	// 폭탄 블록을 제외한 7종류의 블록을 저장합니다.
	Block* randomBlocks[7];

	// 5%의 확률로 등장하는 폭탄 블록입니다.
	// 
	Block* bombBlock;

	// 화면에 게임 상태를 출력하기 위한 ConsoleRenderer 객체입니다.
	ConsoleRenderer renderer;

	// 현재 동작에 바인딩된 키 정보를 가지고 있는 KeyConfig 구조체 변수입니다.
	KeyConfig keyConfig;

	// 사용자 입력을 명령으로 변환해 주는 InputHandler 객체입니다.
	InputHandler inputHandler;

	// 새 게임을 시작하고 지금까지 경과한 시간을 저장합니다.
	std::chrono::steady_clock::time_point startTime;

	// 레벨별 스테이지의 난이도 정보를 저장합니다.
	StageData stageData[10] = {
		{40, 20, 20}, {38, 18, 20}, {35, 18, 20}, {30, 17, 20}, {25, 16, 20},
		{20, 14, 20}, {15, 14, 20}, {10, 13, 20}, {6, 12, 20}, {4, 11, 99999}
	};

	// 현재 점수, 레벨, 현재 스테이지에서 제거한 줄의 개수입니다.
	int score, level, clearedLines;

	// 게임 오버 여부와, 게임 오버를 통한 강제 재시작인지 메뉴를 통한 재시작인지 기록하는 플래그입니다.
	bool isGameOver, isRestarting;
	
	// 하나의 블록당 한 번만 홀드가 가능합니다.
	bool canHold = true;
public:
	GameManager();
	~GameManager();

	// 프로그램이 종료될 때까지 게임을 실행합니다.
	void run();

	// 게임 데이터를 초기 상태로 되돌립니다.
	void initGame();
	
	// 블록의 회전, 왼쪽 이동, 오른쪽 이동 및 그로 인한 상태 변화를 갱신합니다.
	void update(int x, int angle);

	// 블록의 낙하 및 그로 인한 상태 변화를 갱신합니다.
	bool down(bool forceLock = false);

	// 8종류의 블록들 중 하나를 무작위로 생성하여 반환합니다.
	Block* makeNewBlock();

	// 유령 블록의 위치를 반환합니다.
	int getGhostPos(const Block& block);

	// 현재 블록을 홀드 슬롯에 저장합니다.
	void hold();

private:
	// 현재 블록과 유령 블록을 게임판에 출력합니다.
	void drawActiveBlock();

	// 현재 블록이 바닥/고정 블록 위에 닿아 있는지 확인합니다.
	bool isCurrentBlockGrounded() const;

	// 현재 블록을 보드에 고정하고 다음 블록으로 교체합니다.
	bool lockCurrentBlock();

	// 락다운 딜레이 시간(ms)
	static constexpr ULONGLONG LOCK_DELAY_MS = 500;
	bool lockDelayActive = false;
	ULONGLONG lockDelayStartTick = 0;
};

