#include "ConsoleRenderer.h"
#include "Blocks.h"

#include <consoleapi2.h> // SetConsoleCursorPosition
#include <processenv.h> // GetStdHandle
#include <minwindef.h> // DWORD
#include <synchapi.h> // Sleep
#include <stdlib.h> // rand, system
#include <conio.h> // _kbhit, _getche

#include <iostream> // cin, cout
#include <iomanip> // setw
#include <limits> // numeric_limits
#include <iosfwd> // streamsize

#include <fstream> // .txt file
#include <vector> // pair
#include <algorithm> // sort

using namespace std;

/******************** static constants ********************/

const int ConsoleRenderer::ab_x = 5;
const int ConsoleRenderer::ab_y = 1;

const HANDLE ConsoleRenderer::hConsole = GetStdHandle((DWORD)-11);

const string ConsoleRenderer::logoString[7] = {
	"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓",
	"┃ ◆◆◆◆◆ ◆◆◆◆◆ ◆◆◆◆◆ ◆◆◆◆   ◆◆◆   ◆◆◆◆ ┃",
	"┃   ◆   ◆       ◆   ◆   ◆   ◆   ◆     ┃",
	"┃   ◆   ◆◆◆◆◆   ◆   ◆◆◆◆    ◆    ◆◆◆  ┃",
	"┃   ◆   ◆       ◆   ◆   ◆   ◆       ◆ ┃",
	"┃   ◆   ◆◆◆◆◆   ◆   ◆    ◆ ◆◆◆  ◆◆◆◆  ┃",
	"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"
};

const string ConsoleRenderer::gameoverString[5] = {
	"┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓",
	"┃**************************┃",
	"┃*        GAME OVER       *┃",
	"┃**************************┃",
	"┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛"
};

/******************** private functions ********************/

void ConsoleRenderer::gotoxy(int x, int y) const {
	SetConsoleCursorPosition(hConsole, COORD(x, y));
}

void ConsoleRenderer::SetColor(BlockColor color) {
	SetConsoleTextAttribute(hConsole, static_cast<int>(color));
}

/******************** public functions ********************/

ConsoleRenderer::ConsoleRenderer() 
	: randomBlocks(
		new IBlock(0, 0), new OBlock(0, 0), new ZBlock(0, 0),
		new SBlock(0, 0), new JBlock(0, 0), new LBlock(0, 0), new TBlock(0, 0)
	)
{
}

ConsoleRenderer::~ConsoleRenderer() {
	for (int i = 0; i < 7; i++)
		delete randomBlocks[i];
}

void ConsoleRenderer::show_cur_block(const Block& block, int x, int y) {
	SetColor(static_cast<BlockColor>(block.getColor()));

	for (int i = 0; i < 4; i++)	{
		for (int j = 0; j < 4; j++)	{
			if ((j + y) >= 0) {
				if (block.getShapeData()[j][i] == 1)	{
					gotoxy((i + x) * 2 + ab_x, j + y + ab_y);
					cout << "■" ;
				}
			}
		}
	}
	fixCursor();
}

void ConsoleRenderer::erase_cur_block(const Block& block) {
	for (int i = 0; i < 4; i++)	{
		for (int j = 0; j < 4; j++) {
			if (block.getShapeData()[j][i] == 1) {
				gotoxy((i + block.getX()) * 2 + ab_x, j + block.getY() + ab_y);
				cout << "  ";
			}
		}
	}
}

void ConsoleRenderer::show_total_block(const int grid[21][14], int level, bool changeAll) {
	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 14; j++) {
			if(changeAll || prevGrid[i][j] != grid[i][j]) {
				if (j == 0 || j == 13 || i == 20) { //레벨에 따라 외벽 색이 변함
					SetColor(static_cast<BlockColor>((level % 6) + 1));
				}
				else {
					SetColor(BlockColor::DARK_GRAY);
				}
				gotoxy((j * 2) + ab_x, i + ab_y);
				if (grid[i][j] != 0) {
					cout << "■";
				}
				else {
					cout << "  ";
				}
				prevGrid[i][j] = grid[i][j];
			}
		}
	}
	fixCursor();
}

void ConsoleRenderer::initScreen() {
	for (int i = 0; i < 7; i++) {
		gotoxy(12, 3 + i);
		cout << logoString[i];
		Sleep(100);
	}

	gotoxy(20, 15);
	cout << "Please Press Any Key~!";

	for (int i = 0; true; i++) {
		if (i % 40 == 0) {
			for (int j = 0; j < 5; j++)	{
				gotoxy(4, 10 + j);
				cout << "                                                          ";
			}

			for (int i = 0; i < 4; i++)
				show_random_block(4 + 5 * i, 10);
		}
		if (_kbhit()) break;
		Sleep(30);
	}
	_getche();
	system("cls");
}

int ConsoleRenderer::input_data(const KeyConfig& config) {
	system("cls");
	int level = 0;
	SetColor(BlockColor::GRAY);

	// 현재 키 바인딩을 동적으로 출력한다.
	const char* actionNames[6] = {
		"Rotate", "Move Down", "Hard Drop", "Move Left", "Move Right", "Pause"
	};
	const int* keyPtrs[6] = {
		&config.rotate, &config.moveDown, &config.hardDrop,
		&config.moveLeft, &config.moveRight, &config.pause
	};

	gotoxy(10, 7);
	cout << "┏━━━━━━━━━━<GAME KEY>━━━━━━━━━┓";
	for (int i = 0; i < 6; i++) {
		gotoxy(10, 8 + i);
		cout << "┃ ";
		cout << left << setw(11) << actionNames[i];
		cout << ": ";
		string kname = KeyConfig::keyName(*keyPtrs[i]);
		cout << left << setw(13) << kname;
		cout << " ┃";
		Sleep(10);
	}
	gotoxy(10, 14);
	cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";


	while (level < 1 || level > 8) {
		gotoxy(12, 5);
		cout << "Select Start level[1-8]:       \b\b\b\b\b\b\b";
		cin >> level;
		if (cin.fail()) {
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			continue;
		}
	}

	system("cls");
	return level - 1;
}

void ConsoleRenderer::show_next_block(const Block& block, int level) {
	SetColor(static_cast<BlockColor>((level + 1) % 6 + 1));
	for (int i = 1; i < 7; i++) {
		for (int j = 0; j < 6; j++) {
			gotoxy(33 + 2 * j, i);
			if (i == 1 || i == 6 || j == 0 || j == 5) {
				cout << "■";
			} else {
				cout << "  ";
			}
		}
	}
	show_cur_block(block, 15, 1);
}

void ConsoleRenderer::show_gamestat(bool printed_text, int level, int score, int clearedLines, StageData& stage_data) {
	SetColor(BlockColor::GRAY);
	if (printed_text)
	{
		gotoxy(35, 7);
		cout << "STAGE";

		gotoxy(35, 9);
		cout << "SCORE";

		gotoxy(35, 12);
		cout << "LINES";
	}
	gotoxy(41, 7);
	cout << level + 1;
	gotoxy(35, 10);
	cout << setw(10) << score;
	gotoxy(35, 13);
	cout << setw(10) << stage_data.getClearLine() - clearedLines;
}

void ConsoleRenderer::show_gameover() {
	SetColor(BlockColor::RED);
	for (int i = 0; i < 5; i++) {
		gotoxy(15, 8 + i);
		cout << gameoverString[i];
	}
	cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	fflush(stdin);
	Sleep(1000);
	_getche();
	system("cls");
}

// 맨 처음 시작 화면에서 블록을 무작위로 생성해 출력한다.
void ConsoleRenderer::show_random_block(int x, int y)
{
	Block* block = randomBlocks[rand() % 7];
	block->rotate(rand() % 4);
	show_cur_block(*block, x, y);
}

// 일시정지 메뉴를 출력하고 선택 결과를 반환한다.
ConsoleRenderer::MenuResult ConsoleRenderer::show_pause_menu() {
	const int menuX = 7, menuY = 7;

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

	auto drawOptions = [&]() {
		for (int i = 0; i < 3; i++) {
			SetColor(i == selected ? BlockColor::WHITE : BlockColor::GRAY);
			gotoxy(menuX + 2, menuY + 3 + i);
			cout << (i == selected ? "> " : "  ") << options[i] << "          ";
		}
		fixCursor();
	};

	drawOptions();

	while (true) {
		int key = _getch();
		if (key == 0 || key == 224) {
			key = _getch();
			if (key == 72 && selected > 0) selected--;        // Up
			else if (key == 80 && selected < 2) selected++;   // Down
		}
		else if (key == 13) break; // Enter
		drawOptions();
	}

	return static_cast<MenuResult>(selected);
}

void ConsoleRenderer::clearScreen() {
	system("cls");
}

ConsoleRenderer::MainMenuResult ConsoleRenderer::show_main_menu() {
	PlaySound(TEXT("right.wav"), NULL, SND_FILENAME | SND_ASYNC);
	const int menuX = 15, menuY = 10;

	SetColor(BlockColor::YELLOW);
	for (int i = 0; i < 7; i++) {
		gotoxy(12, 2 + i);
		cout << logoString[i];
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

	while (true) {
		int key = _getch();
		if (key == 0 || key == 224) {
			key = _getch();
			if (key == 72 && selected > 0) {
				PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
				selected--;
			}
			else if (key == 80 && selected < 3) {
				PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
				selected++;
			}
		}
		else if (key == 13) break;
		drawOptions();
	}

	return static_cast<MainMenuResult>(selected);
}

void ConsoleRenderer::show_key_settings(KeyConfig& config) {
	const char* actionNames[6] = {
		"Rotate", "Move Down", "Move Left", "Move Right", "Hard Drop", "Pause"
	};
	int* keyRefs[6] = {
		&config.rotate, &config.moveDown, &config.moveLeft,
		&config.moveRight, &config.hardDrop, &config.pause
	};
	const int startX = 10, startY = 3;
	int selected = 0;

	auto drawFrame = [&]() {
		SetColor(BlockColor::YELLOW);
		gotoxy(startX, startY);
		cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
		gotoxy(startX, startY + 1);
		cout << "┃              KEY SETTINGS              ┃";
		gotoxy(startX, startY + 2);
		cout << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫";
		for (int i = 0; i < 6; i++) {
			gotoxy(startX, startY + 3 + i);
			cout << "┃                                        ┃";
		}
		gotoxy(startX, startY + 9);
		cout << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫";
		gotoxy(startX, startY + 10);
		cout << "┃ Up/Dn: select  Enter: rebind  ESC:back ┃";
		gotoxy(startX, startY + 11);
		cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
	};

	auto drawKeys = [&]() {
		for (int i = 0; i < 6; i++) {
			SetColor(i == selected ? BlockColor::WHITE : BlockColor::GRAY);
			gotoxy(startX + 1, startY + 3 + i);
			cout << (i == selected ? "> " : "  ");
			cout << left << setw(12) << actionNames[i];
			cout << ": ";
			cout << left << setw(14) << KeyConfig::keyName(*keyRefs[i]);
			cout << "          ";
		}
		fixCursor();
	};

	auto clearStatus = [&]() {
		gotoxy(startX + 1, startY + 12);
		cout << "                                              ";
		fixCursor();
	};

	drawFrame();
	drawKeys();

	while (true) {
		int key = _getch();
		if (key == 0 || key == 224) {
			key = _getch();
			if (key == 72 && selected > 0) selected--;
			else if (key == 80 && selected < 5) selected++;
		}
		else if (key == 27) {
			break;
		}
		else if (key == 13) {
			// 리바인드 모드
			SetColor(BlockColor::YELLOW);
			gotoxy(startX + 1, startY + 12);
			cout << "Press new key for [" << actionNames[selected] << "]... (ESC=cancel)  ";
			fixCursor();

			int newKey = KeyConfig::readKey();
			clearStatus();

			if (newKey != 27) {
				// 충돌 검사
				int conflictIdx = -1;
				for (int j = 0; j < 6; j++) {
					if (j != selected && *keyRefs[j] == newKey) {
						conflictIdx = j;
						break;
					}
				}

				if (conflictIdx >= 0) {
					SetColor(BlockColor::RED);
					gotoxy(startX + 1, startY + 12);
					cout << "Already used by [" << actionNames[conflictIdx] << "]! Not changed.  ";
					fixCursor();
					Sleep(1500);
					clearStatus();
				}
				else {
					*keyRefs[selected] = newKey;
				}
			}
		}
		drawKeys();
	}
}


void ConsoleRenderer::show_clear_animation(int row)
{
	SetColor(BlockColor::BLUE);
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

// 커서가 계속 움직이는 것을 방지하기 위해 고정시킨다.
void ConsoleRenderer::fixCursor() {
	gotoxy(77, 23);
}

void ConsoleRenderer::show_ranking()
{
	const int startX = 12, startY = 4;

	// 파일에서 데이터 읽기 준비
	vector<int> rankings;
	ifstream inFile("rank.txt");

	// 파일이 없으면 새로 생성하고 기본 데이터 넣기
	if (!inFile.is_open()) {
		ofstream outFile("rank.txt");
		if (outFile.is_open()) {
			outFile.close();
		}
		// 기본 데이터를 썼으니 다시 열어서 읽기
		inFile.open("rank.txt");
	}

	// 파일 내용 읽어와서 백터에 저장
	int score;
	while (inFile >> score) {
		rankings.push_back(score);
	}
	inFile.close();

	// 점수 기준 내림차순 정렬 (높은 점수가 1등)
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

			// 1등은 노란색, 나머지는 회색으로 숫자 표시
			SetColor(i == 0 ? BlockColor::YELLOW : BlockColor::GRAY);
			cout << i + 1 << ". ";

			SetColor(BlockColor::WHITE);
			// 랭킹 데이터가 5개 미만일 때
			if (i < rankings.size()) {
				cout << right << setw(10) << rankings[i] << " pts";
			}
			else {
				cout << right << setw(10) << "0" << " pts";
			}
		}
		fixCursor();
		};

	system("cls");
	drawFrame();
	drawScores();

	_getch();
	PlaySound(TEXT("next.wav"), NULL, SND_FILENAME | SND_ASYNC);
	system("cls");
}
