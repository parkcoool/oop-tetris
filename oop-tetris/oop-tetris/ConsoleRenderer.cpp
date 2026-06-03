#include "ConsoleRenderer.h"
#include "Blocks.h"

#include <consoleapi2.h> // SetConsoleCursorPosition
#include <processenv.h> // GetStdHandle
#include <minwindef.h> // DWORD
#include <wincon.h> // console clear APIs
#include <synchapi.h> // Sleep
#include <stdlib.h> // rand, system
#include <conio.h> // _kbhit, _getche

#include <iostream> // cin, cout
#include <iomanip> // setw
#include <limits> // numeric_limits
#include <iosfwd> // streamsize

#include <fstream> // .txt 파일 읽기
#include <vector> // 점수 저장 vector
#include <algorithm> // 점수 내림차순 정렬

using namespace std;

// 게임 화면이 출력되는 절대 좌표입니다.
const int ConsoleRenderer::ab_x = 15;
const int ConsoleRenderer::ab_y = 1;

const HANDLE ConsoleRenderer::hConsole = GetStdHandle((DWORD)-11);

// 시작 화면에 출력되는 TETRIS 로고 문자열입니다.
const string ConsoleRenderer::logoString[7] = {
	"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓",
	"┃ ◆◆◆◆◆ ◆◆◆◆◆ ◆◆◆◆◆ ◆◆◆◆   ◆◆◆   ◆◆◆◆ ┃",
	"┃   ◆   ◆       ◆   ◆   ◆   ◆   ◆     ┃",
	"┃   ◆   ◆◆◆◆◆   ◆   ◆◆◆◆    ◆    ◆◆◆  ┃",
	"┃   ◆   ◆       ◆   ◆   ◆   ◆       ◆ ┃",
	"┃   ◆   ◆◆◆◆◆   ◆   ◆    ◆ ◆◆◆  ◆◆◆◆  ┃",
	"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"
};

// 게임 오버 시 출력되는 GAME OVER 로고입니다.
const string ConsoleRenderer::gameoverString[5] = {
	"┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓",
	"┃**************************┃",
	"┃*        GAME OVER       *┃",
	"┃**************************┃",
	"┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛"
};

void ConsoleRenderer::gotoxy(int x, int y) const {
	SetConsoleCursorPosition(hConsole, COORD(x, y));
}

void ConsoleRenderer::SetColor(BlockColor color) {
	SetConsoleTextAttribute(hConsole, static_cast<int>(color));
}

ConsoleRenderer::ConsoleRenderer() : randomBlocks(
		new IBlock(0, 0), new OBlock(0, 0), new ZBlock(0, 0),
		new SBlock(0, 0), new JBlock(0, 0), new LBlock(0, 0), new TBlock(0, 0)
	) { }

ConsoleRenderer::~ConsoleRenderer() {
	for (int i = 0; i < 7; i++)
		delete randomBlocks[i];
}

// 현재 내려오고 있는 블록과, 조작의 편의성을 위해 그 블록의 유령 블록을 출력합니다.
void ConsoleRenderer::show_cur_block(const Block& block, int x, int y, bool ghost) {
	SetColor(static_cast<BlockColor>(block.getColor()));

	for (int i = 0; i < 4; i++)	{
		for (int j = 0; j < 4; j++)	{
			if ((j + y) >= 0) {
				if (block.getShapeData()[j][i] == 1)	{
					gotoxy((i + x) * 2 + ab_x, j + y + ab_y);
					cout << (ghost ? "□" : "■"); // 실제 블록은 ■, 유령 블록은 □을 출력합니다.
				}
			}
		}
	}
	fixCursor();
}

// 이전 좌표에 공백을 출력하여 잔상을 지웁니다.
void ConsoleRenderer::erase_cur_block(const Block& block, int y) {
	for (int i = 0; i < 4; i++)	{
		for (int j = 0; j < 4; j++) {
			if (block.getShapeData()[j][i] == 1) {
				gotoxy((i + block.getX()) * 2 + ab_x, j + y + ab_y);
				cout << "  ";
			}
		}
	}
}

// 블록들이 쌓여있는 게임판을 화면에 출력합니다.
// 불필요한 렌더링을 최소화하기 위해, showAll 플래그를 추가해 필요한 경우에만 전체를 출력합니다.
void ConsoleRenderer::show_total_block(const int grid[21][14], int level, bool showAll) {
	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 14; j++) {
			if(showAll || prevGrid[i][j] != grid[i][j]) { // 전체를 출력하지 않는 경우엔 바뀐 부분만 새로 출력합니다.
				if (j == 0 || j == 13 || i == 20) {
					SetColor(static_cast<BlockColor>((level % 6) + 1)); // 레벨에 따라 게임판 벽의 색이 변합니다.
				}
				else {
					SetColor(BlockColor::DARK_GRAY); // 게임판에 고정된 블록은 회색으로 출력합니다.
				}
				gotoxy((j * 2) + ab_x, i + ab_y);
				if (grid[i][j] != 0) {
					cout << "■";
				}
				else {
					cout << "  ";
				}
				prevGrid[i][j] = grid[i][j]; // prevGrid에 현재 게임판의 상태를 기록합니다.
			}
		}
	}
	fixCursor();
}

// 게임을 처음 시작할 때 TETRIS 로고와 함께 시작 화면을 출력합니다.
void ConsoleRenderer::initScreen() {
	for (int i = 0; i < 7; i++) {
		gotoxy(12, 3 + i);
		cout << logoString[i];
		Sleep(100);
	}

	gotoxy(21, 17);
	cout << "Please Press Any Key~!";

	for (int i = 0; true; i++) {
		if (i % 40 == 0) {
			// 일정 간격마다 새로운 블록을 출력해야 하므로 그 전에 공백을 출력하여 싹 지웁니다.
			for (int j = 0; j < 5; j++)	{
				gotoxy(4, 10 + j);
				cout << "                                                          ";
			}

			for (int i = 0; i < 4; i++)
				show_random_block(-1 + 5 * i, 10); // 랜덤한 블록을 화면에 출력합니다.
		}
		if (_kbhit()) break; // 사용자가 아무 키나 입력하면 루프에서 탈출하여 시작 화면을 지웁니다.
		Sleep(30);
	}
	_getche(); // 키를 입력하여 값이 들어있는 입력 버퍼를 비웁니다.
	clearScreen();
}

// 게임을 시작하기 전 동작에 바인딩된 키를 알려준 후,
// 사용자로부터 시작 레벨을 입력받습니다.
int ConsoleRenderer::input_data(const KeyConfig& config) {
	clearScreen();
	int level = 0;
	SetColor(BlockColor::GRAY);

	// 현재 동작들에 지정되어 있는 키들을 가져와 출력합니다.
	const char* actionNames[8] = {
		"Rotate CW", "Rotate CCW", "Move Down", "Hard Drop", "Move Left", "Move Right", "Hold", "Pause"
	};
	const KeyBinding* keyPtrs[8] = {
		&config.rotateCW, &config.rotateCCW, &config.moveDown, &config.hardDrop,
		&config.moveLeft, &config.moveRight, &config.hold, &config.pause
	};

	gotoxy(10, 7);
	cout << "┏━━━━━━━━━━<GAME KEY>━━━━━━━━━┓";
	for (int i = 0; i < 8; i++) {
		gotoxy(10, 8 + i);
		cout << "┃ ";
		cout << left << setw(11) << actionNames[i];
		cout << ": ";
		string kname = KeyConfig::bindingName(*keyPtrs[i]); // 지정된 키를 문자열로 변환한 후 하나로 조합하여 출력합니다.
		cout << left << setw(14) << kname;
		cout << " ┃";
		Sleep(10);
	}
	gotoxy(10, 16);
	cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";


	while (level < 1 || level > 8) { // 유효한 레벨이 입력될 때까지 무한히 입력을 받습니다.
		gotoxy(12, 5);
		cout << "Select Start level[1-8]:       \b\b\b\b\b\b\b";
		cin >> level; 
		if (cin.fail()) { // 입력 오류가 발생하여도 상태를 초기화하여 다시 입력을 받도록 합니다.
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			continue;
		}
	}

	clearScreen();
	return level - 1; // 배열의 인덱스에 맞춰 1을 뺀 값을 반환합니다.
}

// 다음으로 내려올 블록을 우측 상단 슬롯에 출력하여 미리 알려줍니다.
void ConsoleRenderer::show_next_block(const Block& block, int level, bool showAll) {
	if (showAll) {
		SetColor(BlockColor::GRAY);
		gotoxy(45, 1);
		cout << "N E X T";
	}
	SetColor(static_cast<BlockColor>((level + 1) % 6 + 1)); // 레벨에 따라 테두리의 색이 변합니다.
	for (int i = 1; i < 7; i++) {
		for (int j = 0; j < 6; j++) {
			bool border = (i == 1 || i == 6 || j == 0 || j == 5);
			if (!border || showAll) { // 전체를 출력하지 않는 경우엔 불필요한 테두리 출력을 막습니다.
				gotoxy(43 + 2 * j, 1 + i);
				cout << (border ? "■" : "  ");
			}
		}
	}
	show_cur_block(block, 15, 2); // 다음으로 내려올 블록을 슬롯에 출력합니다.
}

// 현재 점수, 제거해야 하는 줄의 개수, 경과한 시간 등의 정보를 화면에 출력합니다.
void ConsoleRenderer::show_gamestat(bool printed_text, int level, int score, int clearedLines, StageData& stage_data) {
	SetColor(BlockColor::GRAY);
	if (printed_text) // 게임이 새로 시작될 때 한 번만 출력하도록 하여 불필요한 렌더링을 줄입니다.
	{
		gotoxy(45, 8);
		cout << "STAGE";

		gotoxy(45, 10);
		cout << "SCORE";

		gotoxy(45, 13);
		cout << "LINES";

		gotoxy(45, 16);
		cout << "TIME";
	}
	gotoxy(51, 8);
	cout << level + 1; // 현재 스테이지 레벨
	gotoxy(45, 11);
	cout << setw(10) << score; // 지금까지 획득한 점수
	gotoxy(45, 14);
	cout << setw(10) << stage_data.getClearLine() - clearedLines; // 다음 레벨로 넘어가기 위해 추가로 제거해야 하는 줄의 개수
}

// 새 게임을 시작하고 지금까지 경과한 시간을 출력합니다.
void ConsoleRenderer::show_time(std::chrono::duration<double> elapsedTime) {
	// 경과 시간을 밀리초 단위 정수로 변환합니다.
	long long total_ms = static_cast<long long>(elapsedTime.count() * 1000);

	long long minutes = (total_ms / 1000) / 60; // 분
	long long seconds = (total_ms / 1000) % 60; // 초
	long long centiseconds = (total_ms % 1000) / 10; // 100분의 1초 (경과한 시간을 소수점 2자리까지 출력합니다.)

	SetColor(BlockColor::GRAY);
	gotoxy(45, 17);

	// MM:SS.xx 형태로 출력 (깜빡임 최소화)
	cout << right << setfill('0')
		<< setw(2) << minutes << ":"
		<< setw(2) << seconds << "."
		<< setw(2) << centiseconds;

	cout << setfill(' '); // 공백 채우기로 원상 복구
}

// 게임 오버 시 GAME OVER 로고를 출력합니다.
void ConsoleRenderer::show_gameover() {
	SetColor(BlockColor::RED);
	for (int i = 0; i < 5; i++) {
		gotoxy(15, 8 + i);
		cout << gameoverString[i];
	}
	cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	Sleep(1000); // 로고를 띄우고 1초 후 사용자로부터 아무 키나 입력받으면 시작 메뉴로 돌아갑니다.
	_getche();
	clearScreen();
}

// 맨 처음 시작 화면에서 블록을 무작위로 뽑아 출력합니다.
void ConsoleRenderer::show_random_block(int x, int y)
{
	Block* block = randomBlocks[rand() % 7]; // BombBlock을 제외한 7종류의 블록들 중 랜덤하게 하나를 가져옵니다.
	block->rotate(rand() % 4); // 랜덤한 각도로 블록을 회전시킨 후 출력합니다.
	show_cur_block(*block, x, y);
}

// 일시정지 메뉴를 출력하고 선택 결과를 반환합니다.
ConsoleRenderer::MenuResult ConsoleRenderer::show_pause_menu() {
	const int menuX = 17, menuY = 7;

	// 메뉴 외곽 프레임 (6행 × 24열)
	static const string frame[] = {
		"┏━━━━━━━━━━━━━━━━━━━━━━┓",
		"┃      PAUSE MENU      ┃",
		"┣━━━━━━━━━━━━━━━━━━━━━━┫",
		"┃                      ┃",
		"┃                      ┃",
		"┃                      ┃",
		"┗━━━━━━━━━━━━━━━━━━━━━━┛"
	};

	SetColor(BlockColor::YELLOW);
	for (int i = 0; i < 7; i++) {
		gotoxy(menuX, menuY + i);
		cout << frame[i];
	}

	static const char* options[] = { "Resume", "Restart", "Quit" };
	int selected = 0;

	// 현재 지정된 옵션을 '>' 접두사로 강조하고 하얀색으로 출력해주는 람다식
	auto drawOptions = [&]() {
		for (int i = 0; i < 3; i++) {
			SetColor(i == selected ? BlockColor::WHITE : BlockColor::GRAY);
			gotoxy(menuX + 2, menuY + 3 + i);
			cout << (i == selected ? "> " : "  ") << options[i] << "          ";
		}
		fixCursor();
	};

	drawOptions();

	while (true) { // 옵션을 선택할 때까지 무한 반복합니다.
		int key = _getch();
		if (key == 0 || key == 224) {
			key = _getch();
			if (key == 72 && selected > 0) selected--;        // 위 방향키
			else if (key == 80 && selected < 2) selected++;   // 아래 방향키
		}
		else if (key == 13) break; // 엔터를 누르면 해당 옵션이 선택되어 반환됩니다.
		drawOptions();
	}

	return static_cast<MenuResult>(selected);
}

// 메인 메뉴를 출력하고 선택 결과를 반환합니다.
ConsoleRenderer::MainMenuResult ConsoleRenderer::show_main_menu() {
	PlaySound(TEXT("right.wav"), NULL, SND_FILENAME | SND_ASYNC); // 효과음을 비동기 방식으로 재생합니다.
	const int menuX = 15, menuY = 10;

	SetColor(BlockColor::YELLOW);
	for (int i = 0; i < 7; i++) {
		gotoxy(12, 2 + i);
		cout << logoString[i]; // TETRIS 로고를 콘솔 위에 고정시킵니다.
	}

	static const string frame[8] = {
		"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓",
		"┃             MENU                ┃",
		"┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫",
		"┃                                 ┃",
		"┃                                 ┃",
		"┃                                 ┃",
		"┃                                 ┃",
		"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"
	};

	SetColor(BlockColor::GRAY);
	for (int i = 0; i < 8; i++) {
		gotoxy(menuX, menuY + i);
		cout << frame[i];
	}

	static const char* options[4] = { "Start Game", "Key Settings", "Ranking", "Quit" };
	int selected = 0;

	// 현재 지정된 옵션을 '>' 접두사로 강조하고 하얀색으로 출력해주는 람다식
	auto drawOptions = [&]() {
		for (int i = 0; i < 4; i++) {
			SetColor(i == selected ? BlockColor::WHITE : BlockColor::GRAY);
			gotoxy(menuX + 1, menuY + 3 + i);
			cout << (i == selected ? "> " : "  ") << options[i];
			int written = 2 + (int)strlen(options[i]);
			for (int j = written; j < 33; j++) cout << ' ';
		}
		fixCursor();
	};

	drawOptions();

	while (true) { // 옵션을 선택할 때까지 무한 반복합니다.
		int key = _getch();
		if (key == 0 || key == 224) {
			key = _getch();
			if (key == 72 && selected > 0) { // 위 방향키
				PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
				selected--;
			}
			else if (key == 80 && selected < 3) { // 아래 방향키
				PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
				selected++;
			}
		}
		else if (key == 13) break; // 엔터를 누르면 해당 옵션이 선택되어 반환됩니다.
		drawOptions();
	}

	return static_cast<MainMenuResult>(selected);
}

// 커스텀 키 설정 메뉴를 출력하고 config를 대화식으로 수정합니다.
void ConsoleRenderer::show_key_settings(KeyConfig& config) {
	const char* actionNames[8] = {
		"Rotate CW", "Rotate CCW", "Move Down", "Move Left", "Move Right", "Hard Drop", "Hold", "Pause"
	};
	KeyBinding* keyRefs[8] = {
		&config.rotateCW, &config.rotateCCW, &config.moveDown, &config.moveLeft,
		&config.moveRight, &config.hardDrop, &config.hold, &config.pause
	};
	const int startX = 10, startY = 3;
	int selected = 0;

	// 키 설정 메뉴의 프레임을 출력하는 람다식
	auto drawFrame = [&]() {
		SetColor(BlockColor::YELLOW);
		gotoxy(startX, startY);
		cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
		gotoxy(startX, startY + 1);
		cout << "┃              KEY SETTINGS              ┃";
		gotoxy(startX, startY + 2);
		cout << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫";
		for (int i = 0; i < 8; i++) {
			gotoxy(startX, startY + 3 + i);
			cout << "┃                                        ┃";
		}
		gotoxy(startX, startY + 11);
		cout << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫";
		gotoxy(startX, startY + 12);
		cout << "┃ Up/Dn: select  1:key1  2:key2  ESC:back┃";
		gotoxy(startX, startY + 13);
		cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
	};

	// 현재 동작들에 바인딩된 키의 이름을 출력하는 람다식
	auto drawKeys = [&]() {
		for (int i = 0; i < 8; i++) {
			SetColor(i == selected ? BlockColor::WHITE : BlockColor::GRAY);
			gotoxy(startX + 1, startY + 3 + i);
			cout << (i == selected ? "> " : "  ");
			cout << left << setw(12) << actionNames[i];
			cout << ": ";
			cout << left << setw(16) << KeyConfig::bindingName(*keyRefs[i]); // 하나의 문자열로 조합하여 출력합니다.
			cout << "  ";
		}
		fixCursor();
	};

	// 동작을 선택했을 때 프레임 하단에 띄운 메시지를 지우는 람다식
	auto clearStatus = [&]() {
		gotoxy(startX + 1, startY + 14);
		cout << "                                                ";
		fixCursor();
	};

	drawFrame();
	drawKeys();

	while (true) { // 옵션을 선택할 때까지 무한 반복합니다.
		int key = _getch();
		if (key == 0 || key == 224) {
			key = _getch();
			if (key == 72 && selected > 0) selected--; // 위 방향키
			else if (key == 80 && selected < 7) selected++; // 아래 방향키
		}
		else if (key == 27) { // ESC 키를 누르면 메인 메뉴로 돌아갑니다.
			break;
		}
		else if (key == 13) { // 엔터를 누르면 선택한 동작에 대해 지정할 커스텀 키를 물어보는 메시지를 띄웁니다.
			SetColor(BlockColor::YELLOW);
			gotoxy(startX + 1, startY + 14);
			cout << "Slot: 1=primary  2=secondary  ESC=cancel  ";
			fixCursor();

			int slotKey = _getch();
			int slot = -1;
			if (slotKey == '1') slot = 0;
			else if (slotKey == '2') slot = 1;
			clearStatus();

			// 1 또는 2 이외의 키를 누르면 되돌아가 다시 옵션을 선택하도록 합니다.
			if (slot != -1) {
				SetColor(BlockColor::YELLOW);
				gotoxy(startX + 1, startY + 14);
				if (slot == 0)
					cout << "New primary key for [" << actionNames[selected] << "]... (ESC=cancel)  ";
				else
					cout << "New 2nd key for [" << actionNames[selected] << "]... (ESC=clear)  ";
				fixCursor();

				int newKey = KeyConfig::readKey();
				clearStatus();

				if (newKey == 27) {
					// 2 키를 누른 상태에서 ESC 키를 누르면 해당 동작에 바인딩된 보조 키를 지웁니다. 
					// 1 키를 누른 상태에서 ESC 키를 누르면 다시 옵션을 선택하도록 합니다.
					if (slot == 1) keyRefs[selected]->key2 = -1;
				}
				else {
					// 다른 동작에 이미 해당 키가 지정되어 겹치는지 검사합니다.
					int conflictIdx = -1;
					for (int j = 0; j < 8; j++) {
						if (j == selected) continue; // 현재 동작은 비교에서 제외합니다. 
						if (keyRefs[j]->key1 == newKey || keyRefs[j]->key2 == newKey) { // 충돌 발생
							conflictIdx = j;
							break;
						}
					}
					// 해당 동작의 기본 키와 보조 키가 겹치는지 검사합니다.
					bool sameAction = false;
					if (conflictIdx == -1) {
						int otherKey = (slot == 0) ? keyRefs[selected]->key2 : keyRefs[selected]->key1;
						if (otherKey != -1 && otherKey == newKey) sameAction = true;
					}

					// 충돌이 발생했다면, 오류 메시지를 1.5초간 출력합니다. 해당 키로는 변경할 수 없습니다.
					// 이후 다시 옵션을 선택하도록 합니다.
					if (conflictIdx >= 0) {
						SetColor(BlockColor::RED);
						gotoxy(startX + 1, startY + 14);
						cout << "Already used by [" << actionNames[conflictIdx] << "]! Not changed.  ";
						fixCursor();
						Sleep(1500);
						clearStatus();
					}
					else if (sameAction) {
						SetColor(BlockColor::RED);
						gotoxy(startX + 1, startY + 14);
						cout << "Already assigned to this action! Not changed.     ";
						fixCursor();
						Sleep(1500);
						clearStatus();
					}
					else { // 충돌 검사에 걸리지 않았다면 해당 동작에 키를 새롭게 지정합니다.
						if (slot == 0) keyRefs[selected]->key1 = newKey;
						else keyRefs[selected]->key2 = newKey;
					}
				}
			}
		}
		drawKeys();
	}
}

// 콘솔 전체를 빈칸으로 채워 화면을 청소합니다.
void ConsoleRenderer::clearScreen() {
	COORD topLeft = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		return;
	}
	DWORD cellCount = static_cast<DWORD>(csbi.dwSize.X) * static_cast<DWORD>(csbi.dwSize.Y);
	DWORD count = 0;
	FillConsoleOutputCharacter(hConsole, ' ', cellCount, topLeft, &count);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, topLeft, &count);
	SetConsoleCursorPosition(hConsole, topLeft);
}

// 꽉 찬 행을 제거할 때 파란색 빈 블록이 번쩍이며 해당 행을 청소하는 애니메이션을 출력합니다.
void ConsoleRenderer::show_clear_animation(int row)
{
	SetColor(BlockColor::BLUE);
	// 10ms 간격으로 파란색 블록이 출력되어 해당 행을 청소한 후 사라집니다.
	for (int j = 1; j < 13; j++)
	{
		gotoxy(2 * j + ab_x, row + ab_y);
		cout << "□";
		Sleep(10);
	}
	for (int j = 1; j < 13; j++)
	{
		gotoxy(2 * j + ab_x, row + ab_y);
		cout << "  ";
		Sleep(10);
	}
}

// 커서가 자주 깜빡이는 것을 방지하기 위해 커서를 우측 하단에 고정시킵니다.
void ConsoleRenderer::fixCursor() {
	gotoxy(77, 23);
}

// 게임 종료 시의 점수를 기록해 상위 5개 점수를 내림차순으로 출력합니다.
void ConsoleRenderer::show_ranking()
{
	const int startX = 12, startY = 4;

	// 파일에서 데이터 읽기 준비
	vector<int> rankings;
	ifstream inFile("rank.txt");

	// ofstream은 파일이 존재하지 않으면 해당 이름으로 새로운 파일을 생성하므로,
	// 파일이 존재하지 않아 열리지 않았다면 ofstream 객체를 생성하여 txt 파일을 생성해 줍니다.
	if (!inFile.is_open()) {
		ofstream outFile("rank.txt");
		if (outFile.is_open()) {
			outFile.close();
		}
		// 기본 데이터를 썼으니 다시 열어서 읽기
		inFile.open("rank.txt");
	}

	// 파일에 저장된 점수를 전부 읽어와서 벡터에 저장합니다.
	int score;
	while (inFile >> score) {
		rankings.push_back(score);
	}
	inFile.close();

	// 람다식을 이용하여 점수 벡터를 내림차순으로 정렬합니다 (더 높은 점수가 앞으로)
	sort(rankings.begin(), rankings.end(), [](int a, int b) {
		return a > b;
		});

	// --- UI 출력 부분 ---
	auto drawFrame = [&]() {
		SetColor(BlockColor::YELLOW);
		gotoxy(startX, startY);
		cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
		gotoxy(startX, startY + 1);
		cout << "┃              HALL OF FAME              ┃";
		gotoxy(startX, startY + 2);
		cout << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫";
		for (int i = 0; i < 5; i++) {
			gotoxy(startX, startY + 3 + i);
			cout << "┃                                        ┃";
		}
		gotoxy(startX, startY + 8);
		cout << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫";
		gotoxy(startX, startY + 9);
		cout << "┃        Press Any Key to Return...      ┃";
		gotoxy(startX, startY + 10);
		cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
		};

	auto drawScores = [&]() {
		for (int i = 0; i < 5; i++) {
			gotoxy(startX + 14, startY + 3 + i);

			// 1등은 노란색, 나머지는 회색으로 점수 표시
			SetColor(i == 0 ? BlockColor::YELLOW : BlockColor::GRAY);
			cout << i + 1 << ". ";

			SetColor(BlockColor::WHITE);
			// 랭킹 데이터가 5개 미만이라면, 남은 순위는 0점으로 표시합니다.
			if (i < rankings.size()) {
				cout << right << setw(10) << rankings[i] << " pts";
			}
			else {
				cout << right << setw(10) << "0" << " pts";
			}
		}
		fixCursor();
		};

	clearScreen();
	drawFrame();
	drawScores();

	_getch(); // 사용자로부터 아무 키나 입력받으면 메인 메뉴로 돌아갑니다.
	PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
	clearScreen();
}

// 홀드한 블록을 화면 좌측 상단 슬롯에 출력합니다.
void ConsoleRenderer::show_hold_block(const Block* block, int level, bool showAll) {
	if (showAll) {
		SetColor(BlockColor::GRAY);
		gotoxy(5, 1);
		cout << "H O L D";
	}
	SetColor(static_cast<BlockColor>((2 * level + 1) % 6 + 1)); // 레벨에 따라 테두리의 색이 변합니다.
	for (int i = 1; i < 7; i++) {
		for (int j = 0; j < 6; j++) {
			bool border = (i == 1 || i == 6 || j == 0 || j == 5);
			if (!border || showAll) { // 전체를 출력하지 않는 경우엔 불필요한 테두리 출력을 막습니다.
				gotoxy(3 + 2 * j, 1 + i);
				cout << (border ? "■" : "  ");
			}
		}
	}
	if(block != nullptr)
		show_cur_block(*block, -5, 2); // 다음으로 내려올 블록을 슬롯에 출력합니다.
}
