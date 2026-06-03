#include <fstream> // .txt 파일 읽고 쓰기
#include <windows.h> 
#include <mmsystem.h> // PlaySound

#pragma comment(lib, "winmm.lib")
#include "GameManager.h"
#include "Blocks.h"

/*
 * 게임의 전반적인 흐름을 관리하는 클래스입니다.
 * 블록 위치 업데이트, 블록 홀드 등 게임에서 발생하는 대부분의 이벤트를 처리합니다.
 */
GameManager::GameManager()
	: randomBlocks(
		new TBlock(5, -4), new OBlock(5, -4), new ZBlock(5, -4),
		new SBlock(5, -4), new JBlock(5, -4), new LBlock(5, -4), new IBlock(5, -4)
	), bombBlock(new BombBlock(5, -4))
{
	ifstream inFile("rank.txt");

	if (!inFile.is_open()) {
		ofstream outFile("rank.txt"); // 파일이 존재하지 않은 경우 ofstream 객체를 생성하여 txt 파일을 생성해 줍니다.
	}
	initGame(); // 게임 실행 전 데이터를 초기화합니다.
	inputHandler.setConfig(keyConfig);

	// 게임 실행 전 시작 화면을 출력합니다.
	renderer.initScreen();
	run();
}

GameManager::~GameManager()
{
	// 동적 할당한 블록들의 메모리를 해제합니다.
	for (int i = 0; i < 7; i++)
		delete randomBlocks[i];
	delete bombBlock;
}

// 프로그램이 종료될 때까지 게임을 실행합니다.
void GameManager::run()
{
	// 메뉴를 통해 재시작을 한 경우 메인 메뉴를 건너뛰고 바로 레벨 선택으로 넘어갑니다.
	bool skipToStart = false;

	while (true) {
		if (!skipToStart) { // 그 외의 방식으로 게임을 시작하는 경우 메인 메뉴를 띄웁니다.
			renderer.clearScreen();

			// 메인 메뉴를 띄운 후 선택한 결과를 가져옵니다.
			ConsoleRenderer::MainMenuResult menu = renderer.show_main_menu();

			if (menu == ConsoleRenderer::MainMenuResult::QUIT) {
				return; // 프로그램 종료
			}
			if (menu == ConsoleRenderer::MainMenuResult::SETTINGS) {
				PlaySound(TEXT("right.wav"), NULL, SND_FILENAME | SND_ASYNC);
				renderer.clearScreen();
				renderer.show_key_settings(keyConfig); // 키 설정 메뉴 출력 후 메인 메뉴로 돌아갑니다.
				continue;
			}
			if (menu == ConsoleRenderer::MainMenuResult::RANKING) {
				PlaySound(TEXT("right.wav"), NULL, SND_FILENAME | SND_ASYNC);
				renderer.clearScreen();
				renderer.show_ranking(); // 랭킹 출력 후 메인 메뉴로 돌아갑니다.
				continue;
			}
			// START를 선택한 경우, 메인 메뉴로 돌아가지 않고 레벨 선택 화면에 진입합니다.
		}
		PlaySound(TEXT("right.wav"), NULL, SND_FILENAME | SND_ASYNC);
		skipToStart = false; // 매번 메뉴를 통해 재시작을 한 경우에만 메인 메뉴를 건너뛰도록 합니다.

		// 사용자로부터 시작 레벨을 입력받고, 현재 블록과 다음 내려올 블록을 가져온 후
		// 현재 블록의 유령 블록 위치를 미리 구합니다.
		level = renderer.input_data(keyConfig);
		currentBlock = makeNewBlock();
		nextBlock = makeNewBlock();
		ghostY = getGhostPos(*currentBlock);

		// 게임판과 모든 블록들, 게임 정보를 화면에 전부 출력합니다.
		renderer.show_total_block(board.getGrid(), level, true);
		renderer.show_next_block(*nextBlock, level, true);
		renderer.show_hold_block(holdBlock, level, true);
		drawActiveBlock();
		renderer.show_gamestat(true, level, score, clearedLines, stageData[level]);

		// 게임 시작과 함께 경과한 시간을 실시간으로 기록하여 MM:SS.xx 형태로 출력합니다.
		startTime = std::chrono::steady_clock::now();
		for (int i = 1; ; i++) {
			renderer.show_time(std::chrono::steady_clock::now() - startTime);
			if (_kbhit()) {
				Command key = inputHandler.getCommand(); // 입력한 키를 명령어로 변환하여 가져옵니다.

				// 입력한 키에 따라 그에 대한 동작을 할 수 있는지 검사하고,
				// 가능하다면 수행합니다.
				switch (key) {
				case Command::ROTATE_CW:	// 시계 방향 회전
					update(currentBlock->getX(), currentBlock->getAngle() + 1);
					PlaySound(TEXT("se_game_rotate.wav"), NULL, SND_FILENAME | SND_ASYNC);
					break;
				case Command::ROTATE_CCW:	// 반시계 방향 회전
					update(currentBlock->getX(), currentBlock->getAngle() - 1);
					PlaySound(TEXT("se_game_rotate.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
					PlaySound(TEXT("se_game_fixa.wav"), NULL, SND_FILENAME | SND_ASYNC);
					renderer.erase_cur_block(*currentBlock, currentBlock->getY());
					renderer.erase_cur_block(*currentBlock, ghostY);

					// 유령 블록이 있는 위치로 블록을 이동시킨 후 게임판에 고정합니다.
					currentBlock->move(0, ghostY - currentBlock->getY());
					lockCurrentBlock();
					break;
				}
				case Command::EXIT:		// 일시정지
				{
					// 일시정지 시작 시간 기록
					auto pauseStartTime = std::chrono::steady_clock::now();

					// 일시정지 메뉴를 출력한 후 선택한 결과를 가져옵니다.
					ConsoleRenderer::MenuResult result = renderer.show_pause_menu();
					PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);

					if (result == ConsoleRenderer::MenuResult::QUIT) {
						return; // 프로그램 종료
					}
					else if (result == ConsoleRenderer::MenuResult::RESTART) {
						renderer.clearScreen();

						// 메뉴를 통해 재시작했으므로 메인 메뉴는 띄우지 않고 레벨 선택으로 넘어갑니다.
						isRestarting = true;
					}
					else {
						// 게임판과 현재 블록을 다시 그려 출력된 메뉴를 지웁니다.
						renderer.show_total_block(board.getGrid(), level, true);
						drawActiveBlock();

						// 일시정지 메뉴에서 흐른 시간을 기록한 후, 
						// 그 시간만큼 startTime을 지연시켜 시간이 멈췄던 것처럼 보이게 합니다.
						auto pauseEndTime = std::chrono::steady_clock::now();
						startTime += (pauseEndTime - pauseStartTime);
					}
					break;
				}
				case Command::HOLD:		// 홀드
					hold();
					PlaySound(TEXT("se_game_hold.wav"), NULL, SND_FILENAME | SND_ASYNC);
					break;
				};
			}

			// 루프 횟수가 현재 스테이지의 speed의 배수일 때 블록을 아래로 한 칸 이동시킵니다.
			if (i % stageData[level].getSpeed() == 0) {
				down();
				drawActiveBlock();
			}

			// 블록이 아래로 더이상 이동할 수 없어도 즉시 고정되지 않고,
			// 500ms 동안 블록을 조작할 수 있으며 이 시간 내에 블록을 이동시키거나 회전시키면
			// 타이머가 초기화됩니다.
			if (isCurrentBlockGrounded()) {
				if (!lockDelayActive) {
					// 블록을 이동시키거나 회전시키면 lockDelayActive가 다시 false가 되어 새로 시간을 잽니다.
					lockDelayActive = true;
					lockDelayStartTick = GetTickCount64();
				}
				else if (GetTickCount64() - lockDelayStartTick >= LOCK_DELAY_MS) {
					// LOCK_DELAY_MS(500ms) 이상 블록을 이동시키지 않으면 블록을 그 위치에 고정합니다.
					lockCurrentBlock();
				}
			}
			else {
				lockDelayActive = false;
			}

			// 줄을 제거한 개수가 현재 스테이지의 목표에 도달하면 다음 스테이지로 넘어갑니다.
			if (stageData[level].getClearLine() <= clearedLines)
			{
				level++;
				clearedLines = 0;

				// 레벨에 따라 테두리의 색이 변하기 때문에 새로 출력합니다.
				renderer.show_total_block(board.getGrid(), level, true);
				renderer.show_gamestat(false, level, score, clearedLines, stageData[level]);
				renderer.show_next_block(*nextBlock, level, true);
			}
			if (isGameOver || isRestarting) // 게임 오버 또는 메뉴에서 재시작 시 게임을 새로 시작합니다.
			{
				PlaySound(TEXT("lose.wav"), NULL, SND_FILENAME | SND_ASYNC);
				if (isGameOver) renderer.show_gameover();
				break;
			}

			// 커서가 자주 깜빡이는 것을 방지하기 위해 커서를 우측 하단에 고정시킵니다.
			renderer.fixCursor();
			Sleep(15);
		}

		// 메뉴를 통해 재시작한 경우 메인 메뉴를 건너뛰고 바로 레벨 선택으로 갑니다.
		bool wasRestarting = isRestarting;
		
		// 획득한 총 점수를 rank.txt 파일에 저장합니다.
		ofstream outFile("rank.txt", ios::app);
		if (outFile.is_open()) {
			outFile << score << "\n"; // 파일에 점수 쓰고 줄바꿈
			outFile.close();
		}

		initGame(); // 게임을 재시작하기 전에 데이터를 초기화합니다.

		if (wasRestarting) {
			skipToStart = true;
		}
	}
}

// 게임 데이터를 초기 상태로 되돌립니다.
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

// 블록의 회전, 왼쪽 이동, 오른쪽 이동 및 그로 인한 상태 변화를 갱신합니다.
void GameManager::update(int x, int angle) {
	const int currentX = currentBlock->getX();
	const int currentY = currentBlock->getY();
	const int currentAngle = currentBlock->getAngle();
	const int normalizedAngle = ((angle % 4) + 4) % 4; // 회전 각도 범위 보정(0~3)
	const bool rotating = (normalizedAngle != currentAngle);

	int targetX = x;
	int targetY = currentY;
	bool canApply = false;

	if (rotating) { 
		// 블록이 벽이나 바닥에 닿아 회전이 불가능한 경우, 5개의 오프셋을 시도해 보며
		// 회전이 가능한 위치를 찾습니다.
		const bool isIBlock = dynamic_cast<const IBlock*>(currentBlock) != nullptr;
		const int rotationDiff = (normalizedAngle - currentAngle + 4) % 4;

		// J,L,S,T,Z 블록 / I 블록 / 시계방향 회전(CW) / 반시계방향 회전(CCW)
		// 모든 경우에 대해서 서로 다른 오프셋 후보들이 주어집니다.
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

		// I블록 여부 / 회전 방향 / 현재 각도에 따라 5개의 오프셋들을 가져옵니다.
		const int (*kicks)[2] = nullptr;
		if (rotationDiff == 1) {
			kicks = isIBlock ? I_CW[currentAngle] : JLSTZ_CW[currentAngle];
		}
		else if (rotationDiff == 3) {
			kicks = isIBlock ? I_CCW[currentAngle] : JLSTZ_CCW[currentAngle];
		}

		if (kicks != nullptr) {
			// 만약 해당 오프셋을 적용했을 때 충돌이 발생하지 않는다면,
			// 해당 위치로 블록을 이동시킬 수 있도록 targetX, targetY를 갱신합니다.
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
		// 회전이 아닌, 좌우로 한 칸 이동하는 동작의 경우
		// 단순히 이동할 위치에서 충돌이 발생하는지만 검사합니다.
		if (!board.checkCollision(*currentBlock, x, currentY, currentAngle)) {
			targetX = x;
			targetY = currentY;
			canApply = true;
		}
	}

	if (canApply) { // 이동 또는 회전이 가능한 경우
		renderer.erase_cur_block(*currentBlock, currentY);
		renderer.erase_cur_block(*currentBlock, ghostY);

		// 현재 블록의 변경된 상태를 갱신하고, 
		// 유령 블록의 위치를 새로 구한 후 현재 블록을 새로 출력합니다.
		currentBlock->rotate(normalizedAngle - currentAngle);
		currentBlock->move(targetX - currentX, targetY - currentY);
		lockDelayActive = false; // 지연 시간 타이머가 활성화된 상태에서 이동한 경우 다시 지연 시간을 초기화합니다.
		ghostY = getGhostPos(*currentBlock);
		drawActiveBlock();
	}
}

// 블록의 낙하 및 그로 인한 상태 변화를 갱신합니다.
bool GameManager::down(bool forceLock)
{
	renderer.erase_cur_block(*currentBlock, currentBlock->getY());
	currentBlock->move(0, 1);

	// 블록을 아래로 한 칸 내렸을 때 충돌이 발생하면
	// 다시 위로 올리면 충돌하지 않고 고정시킬 수 있으므로 위로 한 칸 올립니다.
	if (board.checkCollision(*currentBlock, currentBlock->getX(), currentBlock->getY(),
		currentBlock->getAngle())) {

		currentBlock->move(0, -1);

		if (forceLock) {
			return lockCurrentBlock();
		}

		// 블록이 이미 바닥에 닿아 지연 시간 타이머가 활성화된 상태라면
		// 함수 실행을 바로 종료하여 타이머가 초기화되는 것을 막습니다.
		return false;
	}
	lockDelayActive = false;
	ghostY = getGhostPos(*currentBlock);
	return false;
}

// 8종류의 블록들 중 하나를 무작위로 뽑아 반환합니다.
Block* GameManager::makeNewBlock()
{
	// 폭탄 블록 등장 확률 5%
	if (rand() % 100 < 5) {
		bombBlock->reset();
		return bombBlock;
	}

	// 폭탄 블록을 뽑지 못했다면, 
	// 새로 생성한 난수와 현재 스테이지의 stick_rate를 비교해 I 블록이 등장했는지 확인합니다.
	if (rand() % 100 <= stageData[level].getRate()) {
		randomBlocks[6]->reset();
		return randomBlocks[6];
	}

	// I 블록도 뽑지 못했다면, J, L, S, T, Z, O 블록 중 하나를 뽑아 반환합니다.
	int idx = rand() % 6;
	randomBlocks[idx]->reset();
	return randomBlocks[idx];
}

// 유령 블록의 위치를 반환합니다.
int GameManager::getGhostPos(const Block& block)
{
	// 블록을 더이상 내릴 수 없을 때까지 내린 후, 그때의 y좌표가 유령 블록의 위치입니다.
	int x = block.getX(), y = block.getY();
	while (!board.checkCollision(block, x, y + 1, block.getAngle())) y++;
	return y;
}

// 현재 블록을 홀드 슬롯에 저장합니다.
void GameManager::hold() {
	if (!canHold) return; // 하나의 블록당 한 번만 홀드가 가능합니다.

	renderer.erase_cur_block(*currentBlock, currentBlock->getY());
	renderer.erase_cur_block(*currentBlock, ghostY);

	if (holdBlock == nullptr) { 
		// 홀드 슬롯이 비어있다면, 현재 블록을 홀드 슬롯에 저장한 후 
		// 현재 블록과 다음 블록을 갱신합니다.
		currentBlock->reset(); nextBlock->reset();
		holdBlock = currentBlock;
		currentBlock = nextBlock;
		nextBlock = makeNewBlock();
	}
	else {
		// 홀드 슬롯이 비어있지 않다면, 현재 블록과 홀드 슬롯에 있는 블록을 교환합니다.
		swap(holdBlock, currentBlock);
		currentBlock->reset();
		canHold = false;
	}

	// 블록이 새로 갱신되었으므로 유령 블록의 좌표를 다시 구한 후 
	// 현재 블록과 홀드 슬롯을 새로 출력합니다.
	ghostY = getGhostPos(*currentBlock);
	lockDelayActive = false;
	drawActiveBlock();
	renderer.show_hold_block(holdBlock, level);
}

// 현재 블록과 유령 블록을 게임판에 출력합니다.
void GameManager::drawActiveBlock()
{
	// 유령 블록을 현재 블록 위에 겹치게 그리는 것을 방지하기 위하여,
	// 지연 고정 상태에서는 이미 블록이 유령 블록의 위치에 있으므로 유령 블록을 출력하지 않고
	// 평상시에도 유령 블록을 먼저 출력하도록 합니다.
	if (!lockDelayActive) {
		renderer.show_cur_block(*currentBlock, currentBlock->getX(), ghostY, true);
	}
	renderer.show_cur_block(*currentBlock, currentBlock->getX(), currentBlock->getY());
}

// 현재 블록이 바닥/고정 블록 위에 닿아 있는지 확인합니다.
bool GameManager::isCurrentBlockGrounded() const
{
	return board.checkCollision(*currentBlock, currentBlock->getX(), currentBlock->getY() + 1,
		currentBlock->getAngle());
}

// 현재 블록을 보드에 고정하고 다음 블록으로 교체합니다.
bool GameManager::lockCurrentBlock()
{
	// 충돌했을 때 블록의 일부가 게임판 위로 벗어난 경우(게임 오버)
	if (currentBlock->getY() < 0) {
		isGameOver = true;
		return true;
	}

	bool cleared = false;
	// 현재 블록이 폭탄 블록인 경우엔 예외적으로
	// 그 행이 꽉 찼는지와 상관 없이 그 행을 제거합니다.
	if (currentBlock->isBomb()) {
		int targetRow = currentBlock->getY(); // 폭탄이 멈춘 Y축 행 위치

		// 범위를 벗어나지 않도록 조건문
		if (targetRow+1>= 0 && targetRow+1 < 20) {
			clearedLines++;
			score += 150; // 폭탄 제거 보너스 점수
			cleared = true;

			renderer.show_total_block(board.getGrid(), level, true);
			renderer.show_clear_animation(targetRow+1);
			board.clearRow(targetRow+1);

			renderer.show_gamestat(false, level, score, clearedLines, stageData[level]);
		}
	}
	// 일반 블록인 경우, 현재 블록을 게임판에 고정시킨 후
	// 위에서부터 한 줄씩 검사하여 꽉 찬 행들을 제거합니다.
	else {
		board.mergeBlock(*currentBlock);
		for (int row = 0; row < 20; row++) {
			if (board.checkRow(row)) {
				clearedLines++; cleared = true;
				score += 100 + level * 10 + rand() % 10;
				renderer.show_total_block(board.getGrid(), level, true);
				renderer.show_clear_animation(row);
				board.clearRow(row);
				renderer.show_gamestat(false, level, score, clearedLines, stageData[level]);
			}
		}
	}

	// 줄을 하나도 제거하지 못했다면 고정된 블록에 대해서만 새로 출력을 하고, 
	// 줄을 하나라도 제거했다면 아예 전체 게임판을 새로 출력합니다.
	renderer.show_total_block(board.getGrid(), level, cleared);

	// 현재 블록과 다음 나올 블록을 갱신합니다.
	nextBlock->reset();
	currentBlock = nextBlock;
	nextBlock = makeNewBlock();

	canHold = true; // 블록이 갱신되었으므로 다시 홀드가 가능해집니다.
	lockDelayActive = false;
	ghostY = getGhostPos(*currentBlock);
	renderer.show_next_block(*nextBlock, level);
	renderer.show_cur_block(*currentBlock, currentBlock->getX(), ghostY, true);
	return true;
}
