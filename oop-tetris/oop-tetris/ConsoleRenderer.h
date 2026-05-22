#pragma once
#include <Windows.h>
#include <winnt.h> // HANDLE
#include <string> // string
#include "Block.h"
#include "StageData.h"
using namespace std;

class ConsoleRenderer
{
public:
	enum class BlockColor {
		BLACK, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE,
		DARK_RED, DARK_VOILET, DARK_YELLOW, GRAY,
		DARK_GRAY, BLUE, GREEN, SKY_BLUE,
		RED, VOILET, YELLOW, WHITE
	};

	ConsoleRenderer();
	~ConsoleRenderer();

	void show_cur_block(const Block& block, int x, int y);
	void erase_cur_block(const Block& block);
	void show_total_block(const int grid[21][14], int level, bool changeAll = false);
	void initScreen();
	int input_data();
	void show_next_block(const Block& block, int level);
	void show_gamestat(bool printed_text, int level, int score, int clearedLines, StageData& stage_data);
	void show_gameover();

	// 일시정지 메뉴를 출력하고 선택 결과를 반환한다.
	enum class MenuResult { RESUME = 0, RESTART = 1, QUIT = 2 };
	MenuResult show_pause_menu();

	void clearScreen();

	// 맨 처음 시작 화면에서 블록을 무작위로 생성해 출력한다.
	void show_random_block(int x, int y);

	// 꽉 찬 줄을 제거하는 애니메이션
	void show_clear_animation(int row);

	// 커서가 계속 움직이는 것을 방지하기 위해 고정시킨다.
	void fixCursor();
private:
	static const int ab_x, ab_y;
	static const HANDLE hConsole;
	static const string logoString[];
	static const string informationString[];
	static const string gameoverString[];
	Block* randomBlocks[7];
	int prevGrid[21][14] = {};

	void gotoxy(int x, int y) const;
	void SetColor(BlockColor color);
};

