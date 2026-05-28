#include <fstream> // .txt file
#include <windows.h> 
#include <mmsystem.h> // PlaySound

#pragma comment(lib, "winmm.lib")
#include "GameManager.h"
#include "Blocks.h"

GameManager::GameManager()
	: randomBlocks(
		new TBlock(5, -4), new OBlock(5, -4), new ZBlock(5, -4),
		new SBlock(5, -4), new JBlock(5, -4), new LBlock(5, -4), new IBlock(5, -4)
	)
{
	ifstream inFile("rank.txt");

	if (!inFile.is_open()) {
		ofstream outFile("rank.txt");
	}
	initGame();
	inputHandler.setConfig(keyConfig);

	// 게임 실행 전 시작 화면을 출력한다.
	renderer.initScreen();
	run();
}

GameManager::~GameManager()
{
	for (int i = 0; i < 7; i++)
		delete randomBlocks[i];
}

// 게임을 끝날 때까지 실행한다.
void GameManager::run()
{
	bool skipToStart = false;

	while (true) {
		if (!skipToStart) {
			renderer.clearScreen();
			ConsoleRenderer::MainMenuResult menu = renderer.show_main_menu();

			if (menu == ConsoleRenderer::MainMenuResult::QUIT) {
				return;
			}
			if (menu == ConsoleRenderer::MainMenuResult::SETTINGS) {
				PlaySound(TEXT("right.wav"), NULL, SND_FILENAME | SND_ASYNC);
				renderer.clearScreen();
				renderer.show_key_settings(keyConfig);
				continue;
			}
			if (menu == ConsoleRenderer::MainMenuResult::RANKING) {
				PlaySound(TEXT("right.wav"), NULL, SND_FILENAME | SND_ASYNC);
				renderer.clearScreen();
				renderer.show_ranking();
				continue;
			}
			// START 선택
		}
		PlaySound(TEXT("right.wav"), NULL, SND_FILENAME | SND_ASYNC);
		skipToStart = false;

		level = renderer.input_data(keyConfig);
		currentBlock = makeNewBlock();
		nextBlock = makeNewBlock();
		ghostY = getGhostPos(*currentBlock);
		renderer.show_total_block(board.getGrid(), level, true);
		renderer.show_next_block(*nextBlock, level, true);
		renderer.show_hold_block(holdBlock, level, true);
		drawActiveBlock();
		renderer.show_gamestat(true, level, score, clearedLines, stageData[level]);
		for (int i = 1; ; i++) {
			if (_kbhit()) {
				Command key = inputHandler.getCommand();

				// 입력한 키에 따라 그에 대한 동작을 할 수 있는지 확인하고,
				// 가능하다면 수행한다.
				switch (key) {
				case Command::ROTATE_CW:	// 시계 방향 회전
					update(currentBlock->getX(), currentBlock->getAngle() + 1);
					PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
					break;
				case Command::ROTATE_CCW:	// 반시계 방향 회전
					update(currentBlock->getX(), currentBlock->getAngle() - 1);
					PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
					break;
				case Command::MOVE_LEFT:	// 왼쪽으로 한 칸 이동
					update(currentBlock->getX() - 1, currentBlock->getAngle());
					PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
					break;
				case Command::MOVE_RIGHT:	// 오른쪽으로 한 칸 이동
					update(currentBlock->getX() + 1, currentBlock->getAngle());
					PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
					break;
				case Command::MOVE_DOWN:	// 아래로 한 칸 이동
					down();
					drawActiveBlock();
					PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
					break;
				case Command::HARD_DROP:	// 아래로 더 이상 내려갈 수 없을 때까지 이동
				{
					renderer.erase_cur_block(*currentBlock, currentBlock->getY());
					renderer.erase_cur_block(*currentBlock, ghostY);
					currentBlock->move(0, ghostY - currentBlock->getY());
					lockCurrentBlock();
					PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
					break;
				}
				case Command::EXIT:
				{
					ConsoleRenderer::MenuResult result = renderer.show_pause_menu();
					PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
					if (result == ConsoleRenderer::MenuResult::QUIT) {
						return;
					} else if (result == ConsoleRenderer::MenuResult::RESTART) {
						renderer.clearScreen();
						isRestarting = true;
					} else {
						// RESUME: 보드를 다시 그려서 메뉴 오버레이를 지운다.
						renderer.show_total_block(board.getGrid(), level, true);
						drawActiveBlock();
					}
					break;
				}
				case Command::HOLD:
					hold();
					break;
				};
			}
			if (i % stageData[level].getSpeed() == 0) {
				down();
				drawActiveBlock();
			}

			// 바닥 접지 시 일정 시간 후 블록을 고정한다.
			if (isCurrentBlockGrounded()) {
				if (!lockDelayActive) {
					lockDelayActive = true;
					lockDelayStartTick = GetTickCount64();
				}
				else if (GetTickCount64() - lockDelayStartTick >= LOCK_DELAY_MS) {
					lockCurrentBlock();
				}
			}
			else {
				lockDelayActive = false;
			}

			// 줄을 제거한 개수가 목표에 도달하면 다음 스테이지로 넘어간다.
			if (stageData[level].getClearLine() <= clearedLines)
			{
				level++;
				clearedLines = 0;
				renderer.show_total_block(board.getGrid(), level, true);
				renderer.show_gamestat(false, level, score, clearedLines, stageData[level]);
				renderer.show_next_block(*nextBlock, level, true);
			}
			if (isGameOver || isRestarting)
			{
				PlaySound(TEXT("lose.wav"), NULL, SND_FILENAME | SND_ASYNC);
				if (isGameOver) renderer.show_gameover();
				break;
			}

			// 커서가 계속 움직이는 것을 방지하기 위해 커서를 고정시킨다.
			renderer.fixCursor();
			Sleep(15);
		}

		// 재시작이었다면 메인 메뉴를 건너뛰고 바로 레벨 선택으로 간다.
		bool wasRestarting = isRestarting;

		// 게임 오버 시 루프가 종료되고, 여기서 게임이 초기화되어 새로 시작된다.

		ofstream outFile("rank.txt", ios::app);
		if (outFile.is_open()) {
			outFile << score << "\n"; // 파일에 점수 쓰고 줄바꿈
			outFile.close();
		}
		initGame();

		if (wasRestarting) {
			skipToStart = true;
		}
	}
}

// 게임 데이터를 초기 상태로 되돌린다.
void GameManager::initGame()
{
	score = 0;
	level = 0;
	clearedLines = 0;
	isGameOver = false;
	isRestarting = false;
	canHold = true;
	lockDelayActive = false;
	lockDelayStartTick = 0;
	holdBlock = nullptr;
	board.resetBoard();
}

// 블록의 회전, 왼쪽 이동, 오른쪽 이동 및 그로 인한 상태 변화를 갱신한다.
void GameManager::update(int x, int angle) {
	const int currentX = currentBlock->getX();
	const int currentY = currentBlock->getY();
	const int currentAngle = currentBlock->getAngle();
	const int normalizedAngle = ((angle % 4) + 4) % 4;
	const bool rotating = (normalizedAngle != currentAngle);

	int targetX = x;
	int targetY = currentY;
	bool canApply = false;

	if (rotating) {
		const bool isIBlock = dynamic_cast<const IBlock*>(currentBlock) != nullptr;
		const int rotationDiff = (normalizedAngle - currentAngle + 4) % 4;

		static constexpr int JLSTZ_CW[4][5][2] = {
			{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}, // 0 -> 1
			{{0, 0}, { 1, 0}, { 1,-1}, {0,  2}, { 1,  2}}, // 1 -> 2
			{{0, 0}, { 1, 0}, { 1, 1}, {0, -2}, { 1, -2}}, // 2 -> 3
			{{0, 0}, {-1, 0}, {-1,-1}, {0,  2}, {-1,  2}}  // 3 -> 0
		};
		static constexpr int JLSTZ_CCW[4][5][2] = {
			{{0, 0}, { 1, 0}, { 1, 1}, {0, -2}, { 1, -2}}, // 0 -> 3
			{{0, 0}, { 1, 0}, { 1,-1}, {0,  2}, { 1,  2}}, // 1 -> 0
			{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}, // 2 -> 1
			{{0, 0}, {-1, 0}, {-1,-1}, {0,  2}, {-1,  2}}  // 3 -> 2
		};
		static constexpr int I_CW[4][5][2] = {
			{{0, 0}, {-2, 0}, { 1, 0}, {-2,-1}, { 1, 2}}, // 0 -> 1
			{{0, 0}, {-1, 0}, { 2, 0}, {-1, 2}, { 2,-1}}, // 1 -> 2
			{{0, 0}, { 2, 0}, {-1, 0}, { 2, 1}, {-1,-2}}, // 2 -> 3
			{{0, 0}, { 1, 0}, {-2, 0}, { 1,-2}, {-2, 1}}  // 3 -> 0
		};
		static constexpr int I_CCW[4][5][2] = {
			{{0, 0}, {-1, 0}, { 2, 0}, {-1, 2}, { 2,-1}}, // 0 -> 3
			{{0, 0}, { 2, 0}, {-1, 0}, { 2, 1}, {-1,-2}}, // 1 -> 0
			{{0, 0}, { 1, 0}, {-2, 0}, { 1,-2}, {-2, 1}}, // 2 -> 1
			{{0, 0}, {-2, 0}, { 1, 0}, {-2,-1}, { 1, 2}}  // 3 -> 2
		};

		const int (*kicks)[2] = nullptr;
		if (rotationDiff == 1) {
			kicks = isIBlock ? I_CW[currentAngle] : JLSTZ_CW[currentAngle];
		}
		else if (rotationDiff == 3) {
			kicks = isIBlock ? I_CCW[currentAngle] : JLSTZ_CCW[currentAngle];
		}

		if (kicks != nullptr) {
			for (int i = 0; i < 5; ++i) {
				const int kickedX = currentX + kicks[i][0];
				const int kickedY = currentY + kicks[i][1];
				if (!board.checkCollision(*currentBlock, kickedX, kickedY, normalizedAngle)) {
					targetX = kickedX;
					targetY = kickedY;
					canApply = true;
					break;
				}
			}
		}
		else if (!board.checkCollision(*currentBlock, x, currentY, normalizedAngle)) {
			// 180도 회전 등 SRS 표에 없는 경우는 기본 충돌 검사만 수행
			targetX = x;
			targetY = currentY;
			canApply = true;
		}
	}
	else {
		if (!board.checkCollision(*currentBlock, x, currentY, currentAngle)) {
			targetX = x;
			targetY = currentY;
			canApply = true;
		}
	}

	if (canApply) {
		renderer.erase_cur_block(*currentBlock, currentY);
		renderer.erase_cur_block(*currentBlock, ghostY);
		currentBlock->rotate(normalizedAngle - currentAngle);
		currentBlock->move(targetX - currentX, targetY - currentY);
		lockDelayActive = false;
		ghostY = getGhostPos(*currentBlock);
		drawActiveBlock();
	}
}

// 블록의 낙하 및 그로 인한 상태 변화를 갱신한다.
// 블록이 고정되어 다음 블록으로 넘어간 경우 true 반환
bool GameManager::down(bool forceLock)
{
	renderer.erase_cur_block(*currentBlock, currentBlock->getY());
	currentBlock->move(0, 1);
	if (board.checkCollision(*currentBlock, currentBlock->getX(), currentBlock->getY(),
		currentBlock->getAngle())) {

		currentBlock->move(0, -1);

		if (forceLock) {
			return lockCurrentBlock();
		}

		// 바닥에 닿아 있는 동안에는 run 루프에서 락다운 딜레이를 계산한다.
		return false;
	}
	lockDelayActive = false;
	ghostY = getGhostPos(*currentBlock);
	return false;
}

// 7개의 블록들 중 하나를 생성하여 반환한다.
Block* GameManager::makeNewBlock()
{
	if (rand() % 100 <= stageData[level].getRate()) {
		randomBlocks[6]->reset();
		return randomBlocks[6];
	}

	int idx = rand() % 6;
	randomBlocks[idx]->reset();
	return randomBlocks[idx];
}

int GameManager::getGhostPos(const Block& block)
{
	int x = block.getX(), y = block.getY();
	while (!board.checkCollision(block, x, y + 1, block.getAngle())) y++;
	return y;
}

void GameManager::hold() {
	if (!canHold) return;

	renderer.erase_cur_block(*currentBlock, currentBlock->getY());
	renderer.erase_cur_block(*currentBlock, ghostY);

	if (holdBlock == nullptr) {
		currentBlock->reset(); nextBlock->reset();
		holdBlock = currentBlock;
		currentBlock = nextBlock;
		nextBlock = makeNewBlock();
	}
	else {
		swap(holdBlock, currentBlock);
		currentBlock->reset();
		canHold = false;
	}

	ghostY = getGhostPos(*currentBlock);
	lockDelayActive = false;
	drawActiveBlock();
	renderer.show_hold_block(holdBlock, level);
}

void GameManager::drawActiveBlock()
{
	// 지연 고정 상태에서는 고스트를 숨기고, 평상시에는 고스트를 먼저 그리고
	// 현재 블록을 나중에 그려 겹쳐도 현재 블록이 항상 위에 오도록 한다.
	if (!lockDelayActive) {
		renderer.show_cur_block(*currentBlock, currentBlock->getX(), ghostY, true);
	}
	renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
}

bool GameManager::isCurrentBlockGrounded() const
{
	return board.checkCollision(*currentBlock, currentBlock->getX(), currentBlock->getY() + 1,
		currentBlock->getAngle());
}

bool GameManager::lockCurrentBlock()
{
	// 충돌했을 때 블록의 일부가 그리드 위로 벗어난 경우(게임 오버)
	if (currentBlock->getY() < 0) {
		isGameOver = true;
		return true;
	}

	// 현재 블록을 그리드에 고정 시킨 후
	// 위에서부터 한 줄씩 검사하여 꽉 찬 줄을 제거한다.
	board.mergeBlock(*currentBlock);
	bool cleared = false;
	for (int row = 0; row < 20; row++) {
		if (board.checkRow(row)) {
			clearedLines++; cleared = true;
			score += 100 + level * 10 + rand() % 10;
			renderer.show_total_block(board.getGrid(), level, true);
			renderer.show_clear_animation(row);
			board.clearRow(row);
			renderer.show_gamestat(false, level, score, clearedLines, stageData[level]);
			PlaySound(TEXT("right.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
	}
	renderer.show_total_block(board.getGrid(), level, cleared);

	// 현재 블록과 다음 나올 블록을 갱신한다.
	nextBlock->reset();
	currentBlock = nextBlock;
	nextBlock = makeNewBlock();
	canHold = true;
	lockDelayActive = false;
	ghostY = getGhostPos(*currentBlock);
	renderer.show_next_block(*nextBlock, level);
	renderer.show_cur_block(*currentBlock, currentBlock->getX(), ghostY, true);
	return true;
}
