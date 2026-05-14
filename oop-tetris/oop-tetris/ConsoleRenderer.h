#pragma once
#include <winnt.h> // HANDLE
#include <string> // string
#include "Block.h"
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

	void show_cur_block(int shape, int angle, int x, int y);
	void erase_cur_block(int shape, int angle, int x, int y);
	void show_total_block();
	void initScreen();
	void input_data();
	void show_next_block(int shape);
	void show_gamestat(bool printed_text);
	void show_gameover();
private:
	int level;
	int score;
	int lines;
	static const int ab_x, ab_y;
	static const HANDLE hConsole;
	static const string logoString[];
	static const string informationString[];
	static const string gameoverString[];
	void gotoxy(int x, int y) const;
	void SetColor(BlockColor color);
};

