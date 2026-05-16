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

	ConsoleRenderer() = default;

	void show_cur_block(const Block& block, const int& x, const int& y);
	void erase_cur_block(const Block& block);
	void show_total_block(const int grid[21][14], const int& level);
	void initScreen();
	int input_data();
	void show_next_block(Block& block, const int& level);
	void show_gamestat(bool printed_text, const int& level, const int& score, const int& clearedLines, StageData& stage_data);
	void show_gameover();

	// 맨 처음 시작 화면에서 블록을 무작위로 생성해 출력한다.
	void show_random_block(int x, int y);

	// 꽉 찬 줄을 제거하는 애니메이션
	void show_clear_animation(const int& row);

	// 커서가 계속 움직이는 것을 방지하기 위해 고정시킨다.
	void fixCursor();
private:
	static const int ab_x, ab_y;
	static const HANDLE hConsole;
	static const string logoString[];
	static const string informationString[];
	static const string gameoverString[];
	void gotoxy(int x, int y) const;
	void SetColor(BlockColor color);
};

