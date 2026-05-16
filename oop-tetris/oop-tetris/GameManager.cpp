#include "GameManager.h"
#include "Blocks.h"

GameManager::GameManager()
{
	initGame();
	stageData[0] = new StageData(40, 20, 20);
	stageData[1] = new StageData(38, 18, 20);
	stageData[2] = new StageData(35, 18, 20);
	stageData[3] = new StageData(30, 17, 20);
	stageData[4] = new StageData(25, 16, 20);
	stageData[5] = new StageData(20, 14, 20);
	stageData[6] = new StageData(15, 14, 20);
	stageData[7] = new StageData(10, 13, 20);
	stageData[8] = new StageData(6, 12, 20);
	stageData[9] = new StageData(4, 11, 99999);

	// 게임 실행 전 시작 화면을 출력한다.
	renderer.initScreen();
	run();
}

GameManager::~GameManager()
{
	if (currentBlock != nullptr) {
		delete currentBlock;
		currentBlock = nullptr;
	}
	if (nextBlock != nullptr) {
		delete nextBlock;
		nextBlock = nullptr;
	}
	for (int i = 0; i < 10; i++) {
		delete stageData[i];
		stageData[i] = nullptr;
	}
}

// 게임을 끝날 때까지 실행한다.
void GameManager::run()
{
	while (true) {
		level = renderer.input_data();
		currentBlock = makeNewBlock();
		nextBlock = makeNewBlock();
		renderer.show_total_block(board.getGrid(), level);
		renderer.show_next_block(*nextBlock, level);
		renderer.show_gamestat(true, level, score, clearedLines, *stageData[level]);
		for (int i = 1; ; i++) {
			if (_kbhit()) {
				Command key = inputHandler.getCommand();

				// 입력한 키에 따라 그에 대한 동작을 할 수 있는지 확인하고,
				// 가능하다면 수행한다.
				switch (key) {
				case Command::ROTATE:		// 회전
					if (!board.checkCollision(*currentBlock, currentBlock->getX(), currentBlock->getY(), 
						currentBlock->getAngle() + 1)) {
						renderer.erase_cur_block(*currentBlock);
						currentBlock->rotate();
						renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
					}
					break;
				case Command::MOVE_LEFT:	// 왼쪽으로 한 칸 이동
					if (!board.checkCollision(*currentBlock, currentBlock->getX() - 1, currentBlock->getY(),
						currentBlock->getAngle())) {
						renderer.erase_cur_block(*currentBlock);
						currentBlock->move(-1, 0);
						renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
					}
					break;
				case Command::MOVE_RIGHT:	// 오른쪽으로 한 칸 이동
					if (!board.checkCollision(*currentBlock, currentBlock->getX() + 1, currentBlock->getY(),
						currentBlock->getAngle())) {
						renderer.erase_cur_block(*currentBlock);
						currentBlock->move(1, 0);
						renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
					}
					break;
				case Command::MOVE_DOWN:	// 아래로 한 칸 이동
					update();
					renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
					break;
				case Command::HARD_DROP:	// 아래로 더 이상 내려갈 수 없을 때까지 이동
					bool collided = false;
					while (!collided) collided = update();
					renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
					break;
				};
			}
			if (i % stageData[level]->getSpeed() == 0) {
				update();
				renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
			}

			// 줄을 제거한 개수가 목표에 도달하면 다음 스테이지로 넘어간다.
			if (stageData[level]->getClearLine() <= clearedLines)
			{
				level++;
				clearedLines = 0;
				renderer.show_total_block(board.getGrid(), level);
				renderer.show_gamestat(false, level, score, clearedLines, *stageData[level]);
				renderer.show_next_block(*nextBlock, level);
			}
			if (isGameOver)
			{
				renderer.show_gameover();
				break;
			}

			// 커서가 계속 움직이는 것을 방지하기 위해 커서를 고정시킨다.
			renderer.fixCursor();
			Sleep(15);
		}

		// 게임 오버 시 루프가 종료되고, 여기서 게임이 초기화되어 새로 시작된다.
		initGame();
	}
}

// 게임 데이터를 초기 상태로 되돌린다.
void GameManager::initGame()
{
	score = 0;
	level = 0;
	clearedLines = 0;
	isGameOver = false;
	board.clearBoard();
}

// 블록의 낙하 및 그로 인한 상태 변화를 갱신한다.
// 충돌한 경우 true 반환
bool GameManager::update()
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
		for (int row = 0; row < 20; row++) {
			if (board.checkRow(row)) {
				clearedLines++;
				score += 100 + level * 10 + rand() % 10;
				renderer.show_total_block(board.getGrid(), level);
				renderer.show_clear_animation(row);
				board.clearRow(row);
				renderer.show_gamestat(false, level, score, clearedLines, *stageData[level]);
			}
		}
		renderer.show_total_block(board.getGrid(), level);

		// 현재 블록과 다음 나올 블록을 갱신한다.
		delete currentBlock;
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
	if (rand() % 100 <= stageData[level]->getRate())
		return new IBlock(5, -4);

	switch (rand() % 6) {
	case 0:
		return new OBlock(5, -4);
	case 1:
		return new SBlock(5, -4);
	case 2:
		return new ZBlock(5, -4);
	case 3:
		return new LBlock(5, -4);
	case 4:
		return new TBlock(5, -4);
	default:
		return new JBlock(5, -4);
	}
}
