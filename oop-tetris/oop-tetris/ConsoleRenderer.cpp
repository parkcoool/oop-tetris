#include "ConsoleRenderer.h"

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

const string ConsoleRenderer::logoString[] = {
	"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓",
	"┃ ◆◆◆◆◆ ◆◆◆◆◆ ◆◆◆◆◆ ◆◆◆◆   ◆◆◆   ◆◆◆◆ ┃",
	"┃   ◆   ◆       ◆   ◆   ◆   ◆   ◆     ┃",
	"┃   ◆   ◆◆◆◆◆   ◆   ◆◆◆◆    ◆    ◆◆◆  ┃",
	"┃   ◆   ◆       ◆   ◆   ◆   ◆       ◆ ┃",
	"┃   ◆   ◆◆◆◆◆   ◆   ◆    ◆ ◆◆◆  ◆◆◆◆  ┃",
	"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"
};

const string ConsoleRenderer::informationString[] = {
	"┏━━━━━━━━━<GAME KEY>━━━━━━━━━┓",
	"┃ UP   : Rotate Block        ┃",
	"┃ DOWN : Move One-Step Down  ┃",
	"┃ SPACE: Move Bottom Down    ┃",
	"┃ LEFT : Move Left           ┃",
	"┃ RIGHT: Move Right          ┃",
	"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"
};

const string ConsoleRenderer::gameoverString[] = {
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

void ConsoleRenderer::show_cur_block(int shape, int angle, int x, int y) {
	switch (shape)
	{
	case 0:
		SetColor(BlockColor::RED);
		break;
	case 1:
		SetColor(BlockColor::BLUE);
		break;
	case 2:
		SetColor(BlockColor::SKY_BLUE);
		break;
	case 3:
		SetColor(BlockColor::WHITE);
		break;
	case 4:
		SetColor(BlockColor::YELLOW);
		break;
	case 5:
		SetColor(BlockColor::VOILET);
		break;
	case 6:
		SetColor(BlockColor::GREEN);
		break;
	}

	for (int i = 0; i < 4; i++)	{
		for (int j = 0; j < 4; j++)	{
			if ((j + y) >= 0) {
				if (block[shape][angle][j][i] == 1)	{
					gotoxy((i + x) * 2 + ab_x, j + y + ab_y);
					cout << "■" ;
				}
			}
		}
	}
	SetColor(BlockColor::BLACK);
	gotoxy(77, 23);
}

void ConsoleRenderer::erase_cur_block(int shape, int angle, int x, int y) {
	for (int i = 0; i < 4; i++)	{
		for (int j = 0; j < 4; j++) {
			if (block[shape][angle][j][i] == 1) {
				gotoxy((i + x) * 2 + ab_x, j + y + ab_y);
				cout << "  ";
			}
		}
	}
}

void ConsoleRenderer::show_total_block() {
	SetColor(BlockColor::DARK_GRAY);
	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 14; j++) {
			if (j == 0 || j == 13 || i == 20) { //레벨에 따라 외벽 색이 변함
				SetColor((level % 6) + 1);
			} else {
				SetColor(BlockColor::DARK_GRAY);
			}
			gotoxy((j * 2) + ab_x, i + ab_y);
			if (total_block[i][j] == 1) {
				cout << "■";
			} else {
				cout << "  ";
			}

		}
	}
	SetColor(BlockColor::BLACK);
	gotoxy(77, 23);
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
			for (j = 0; j < 5; j++)	{
				gotoxy(4, 10 + j); // 수정
				cout << "                                                          ";
			}

			show_cur_block(rand() % 7, rand() % 4, 4, 10);
			show_cur_block(rand() % 7, rand() % 4, 9, 10);
			show_cur_block(rand() % 7, rand() % 4, 14, 10);
			show_cur_block(rand() % 7, rand() % 4, 19, 10);
		}
		if (_kbhit()) break;
		Sleep(30);
	}
	_getche();
	system("cls");
}

void ConsoleRenderer::input_data() {
	int i = 0;
	SetColor(BlockColor::GRAY);
	for (int i = 0; i < 7; i++) {
		gotoxy(10, 7 + i);
		cout << informationString[i];
		Sleep(10);
	}


	while (i < 1 || i > 8) {
		gotoxy(12, 5);
		printf("Select Start level[1-8]:       \b\b\b\b\b\b\b");
		cin >> i;
		if (cin.fail()) {
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			continue;
		}
	}

	level = i - 1;
	system("cls");
}

void ConsoleRenderer::show_next_block(int shape) {
	SetColor(BlockColor((level + 1) % 6 + 1));
	for (int i = 1; i < 7; i++) {
		gotoxy(33, i);
		for (int j = 0; j < 6; j++) {
			if (i == 1 || i == 6 || j == 0 || j == 5) {
				cout << "■";
			} else {
				cout << "  ";
			}
		}
	}
	show_cur_block(shape, 0, 15, 1);
}

void ConsoleRenderer::show_gamestat(bool printed_text) {
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
	cout << setw(10) << stage_data[level].clear_line - lines;
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