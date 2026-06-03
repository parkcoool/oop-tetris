#pragma once
#include <Windows.h>
#include <winnt.h> // HANDLE
#include <string> // string
#include <chrono> // 경과한 시간 측정
#include "Block.h"
#include "StageData.h"
#include "KeyConfig.h"
using namespace std;

/*
 * 메뉴 화면, 게임판 등을 콘솔 화면에 직접 출력하는 함수들이 정의된 클래스입니다.
 */
class ConsoleRenderer
{
public:
	// 블록의 색상 값들을 열거형으로 정의합니다.
	enum class BlockColor {
		BLACK, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE,
		DARK_RED, DARK_VOILET, DARK_YELLOW, GRAY,
		DARK_GRAY, BLUE, GREEN, SKY_BLUE,
		RED, VOILET, YELLOW, WHITE
	};

	// 일시 정지 메뉴에서 선택한 결과의 상태를 정의합니다.
	enum class MenuResult { RESUME = 0, RESTART = 1, QUIT = 2 };

	// 게임 시작 전 메인 메뉴에서 선택한 결과의 상태를 정의합니다.
	enum class MainMenuResult { START = 0, SETTINGS = 1, RANKING = 2 , QUIT = 3};

	ConsoleRenderer();
	~ConsoleRenderer();

	// 현재 내려오고 있는 블록과, 조작의 편의성을 위해 그 블록의 유령 블록을 출력합니다.
	void show_cur_block(const Block& block, int x, int y, bool ghost = false);

	// 이전 좌표에 공백을 출력하여 잔상을 지웁니다.
	void erase_cur_block(const Block& block, int y);

	// 블록들이 쌓여있는 게임판을 화면에 출력합니다.
	// 불필요한 렌더링을 최소화하기 위해, showAll 플래그를 추가해 필요한 경우에만 전체를 출력합니다.
	void show_total_block(const int grid[21][14], int level, bool showAll = false);

	// 게임을 처음 시작할 때 TETRIS 로고와 함께 시작 화면을 출력합니다.
	void initScreen();

	// 게임을 시작하기 전 동작에 바인딩된 키를 알려준 후,
	// 사용자로부터 시작 레벨을 입력받습니다.
	int input_data(const KeyConfig& config);

	// 다음으로 내려올 블록을 우측 상단 슬롯에 출력하여 미리 알려줍니다.
	void show_next_block(const Block& block, int level, bool showAll = false);

	// 현재 점수, 제거해야 하는 줄의 개수, 경과한 시간 등의 정보를 화면에 출력합니다.
	void show_gamestat(bool printed_text, int level, int score, int clearedLines, StageData& stage_data);

	// 새 게임을 시작하고 지금까지 경과한 시간을 출력합니다.
	void show_time(std::chrono::duration<double> elapsedTime);

	// 게임 오버 시 GAME OVER 로고를 출력합니다.
	void show_gameover();

	// 맨 처음 시작 화면에서 블록을 무작위로 뽑아 출력합니다.
	void show_random_block(int x, int y);

	// 일시정지 메뉴를 출력하고 선택 결과를 반환합니다.
	MenuResult show_pause_menu();

	// 메인 메뉴를 출력하고 선택 결과를 반환합니다.
	MainMenuResult show_main_menu();

	// 커스텀 키 설정 메뉴를 출력하고 config를 대화식으로 수정합니다.
	void show_key_settings(KeyConfig& config);

	// 콘솔 전체를 빈칸으로 채워 화면을 청소합니다.
	void clearScreen();

	// 꽉 찬 행을 제거할 때 파란색 빈 블록이 번쩍이며 해당 행을 청소하는 애니메이션을 출력합니다.
	void show_clear_animation(int row);

	// 커서가 자주 깜빡이는 것을 방지하기 위해 커서를 우측 하단에 고정시킵니다.
	void fixCursor();

	// 게임 종료 시의 점수를 기록해 상위 5개 점수를 내림차순으로 출력합니다.
	void show_ranking();

	// 홀드한 블록을 화면 좌측 상단 슬롯에 출력합니다.
	void show_hold_block(const Block* block, int level, bool showAll = false);

private:
	static const int ab_x, ab_y;
	static const HANDLE hConsole;
	static const string logoString[];
	static const string gameoverString[];

	// 시작 화면에 랜덤하게 출력되는 블록들입니다.
	Block* randomBlocks[7];

	// 이전 프레임의 게임판 상태를 기록해두어 변경된 부분만 출력해 불필요한 출력을 줄입니다.
	int prevGrid[21][14] = {};

	void gotoxy(int x, int y) const;
	void SetColor(BlockColor color);
};

