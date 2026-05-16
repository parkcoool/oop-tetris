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

const string ConsoleRenderer::informationString[7] = {
	"┏━━━━━━━━━<GAME KEY>━━━━━━━━━┓",
	"┃ UP   : Rotate Block        ┃",
	"┃ DOWN : Move One-Step Down  ┃",
	"┃ SPACE: Move Bottom Down    ┃",
	"┃ LEFT : Move Left           ┃",
	"┃ RIGHT: Move Right          ┃",
	"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"
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

void ConsoleRenderer::show_cur_block(const Block& block, const int& x, const int& y) {
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

void ConsoleRenderer::show_total_block(const int grid[21][14], const int& level) {
	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 14; j++) {
			if (j == 0 || j == 13 || i == 20) { //레벨에 따라 외벽 색이 변함
				SetColor(static_cast<BlockColor>((level % 6) + 1));
			} else {
				SetColor(BlockColor::DARK_GRAY);
			}
			gotoxy((j * 2) + ab_x, i + ab_y);
			if (grid[i][j] != 0) {
				cout << "■";
			} else {
				cout << "  ";
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

int ConsoleRenderer::input_data() {
	int level = 0;
	SetColor(BlockColor::GRAY);
	for (int i = 0; i < 7; i++) {
		gotoxy(10, 7 + i);
		cout << informationString[i];
		Sleep(10);
	}


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

void ConsoleRenderer::show_next_block(Block& block, const int& level) {
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

void ConsoleRenderer::show_gamestat(bool printed_text, const int& level, const int& score, const int& clearedLines, StageData& stage_data) {
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
	Block* randomBlock = nullptr;
	switch (rand() % 7) {
		case 0:
			randomBlock = new OBlock(x, y);
			break;
		case 1:
			randomBlock = new IBlock(x, y);
			break;
		case 2:
			randomBlock = new ZBlock(x, y);
			break;
		case 3:
			randomBlock = new SBlock(x, y);
			break;
		case 4:
			randomBlock = new TBlock(x, y);
			break;
		case 5:
			randomBlock = new JBlock(x, y);
			break;
		default:
			randomBlock = new LBlock(x, y);
			break;
	}
	show_cur_block(*randomBlock, x, y);
	delete randomBlock;
}

// 꽉 찬 줄을 제거하는 애니메이션
void ConsoleRenderer::show_clear_animation(const int& row)
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