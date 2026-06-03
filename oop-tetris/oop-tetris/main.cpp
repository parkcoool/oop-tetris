#include "GameManager.h"
#include <ctime>

// main 함수가 가장 먼저 호출되므로, 여기서 GameManager 객체를 생성하여 게임을 시작합니다.
int main() {

	// seed 값을 현재 시간으로 설정하여 블록이 매번 다른 패턴으로 등장할 수 있도록 합니다.
	srand((unsigned)time(NULL));

	GameManager mgr;
	return 0;
}