#include <fstream> // .txt file
#include <vector> // pair
#include <algorithm> // sort
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
	// 파일에서 데이터 읽기 준비
	vector<pair<string, int>> rankings;
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
		renderer.show_total_block(board.getGrid(), level, true);
		renderer.show_next_block(*nextBlock, level);
		renderer.show_gamestat(true, level, score, clearedLines, stageData[level]);
		for (int i = 1; ; i++) {
			if (_kbhit()) {
				Command key = inputHandler.getCommand();

				// 입력한 키에 따라 그에 대한 동작을 할 수 있는지 확인하고,
				// 가능하다면 수행한다.
				switch (key) {
				case Command::ROTATE:		// 회전
					update(currentBlock->getX(), currentBlock->getAngle() + 1);
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
					renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
					PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
					break;
				case Command::HARD_DROP:	// 아래로 더 이상 내려갈 수 없을 때까지 이동
				{
					bool collided = false;
					while (!collided) collided = down();
					renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
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
						renderer.show_next_block(*nextBlock, level);
						renderer.show_gamestat(true, level, score, clearedLines, stageData[level]);
						renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
					}
					break;
				}
				};
			}
			if (i % stageData[level].getSpeed() == 0) {
				down();
				renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
			}

			// 줄을 제거한 개수가 목표에 도달하면 다음 스테이지로 넘어간다.
			if (stageData[level].getClearLine() <= clearedLines)
			{
				level++;
				clearedLines = 0;
				renderer.show_total_block(board.getGrid(), level, true);
				renderer.show_gamestat(false, level, score, clearedLines, stageData[level]);
				renderer.show_next_block(*nextBlock, level);
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
	board.resetBoard();
}

// 블록의 회전, 왼쪽 이동, 오른쪽 이동 및 그로 인한 상태 변화를 갱신한다.
void GameManager::update(int x, int angle) {
	if (!board.checkCollision(*currentBlock, x, currentBlock->getY(), angle)) {
		renderer.erase_cur_block(*currentBlock);
		currentBlock->rotate(angle - currentBlock->getAngle());
		currentBlock->move(x - currentBlock->getX(), 0);
		renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
	}
}

// 블록의 낙하 및 그로 인한 상태 변화를 갱신한다.
// 충돌한 경우 true 반환
bool GameManager::down()
{
	renderer.erase_cur_block(*currentBlock);
	currentBlock->move(0, 1);
	if (board.checkCollision(*currentBlock, currentBlock->getX(), currentBlock->getY(),
		currentBlock->getAngle())) {

		currentBlock->move(0, -1);

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
		renderer.show_next_block(*nextBlock, level);
		return true;
	}
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
