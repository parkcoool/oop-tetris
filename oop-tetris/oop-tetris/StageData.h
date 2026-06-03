#pragma once
/*
 * 레벨 별로 난이도를 다르게 하기 위해,
 * 각 스테이지의 정보를 모아놓은 클래스입니다.
 */
class StageData
{
private:
	int speed, stick_rate, clear_line;
public:
	StageData(int speed, int stick_rate, int clear_line);

	// 블록이 자동으로 낙하하기 위해 필요한 루프 횟수인 speed를 반환합니다.
	// 약 (speed * 15) ms의 간격으로 블록이 낙하합니다.
	int getSpeed() const;

	// 다음 블록으로 폭탄 블록이 등장하지 않았을 때
	// 7종류의 블록 중 I 블록이 등장하는 확률을 의미하는 stick_rate를 반환합니다.
	int getRate() const;

	// 다음 레벨로 넘어가기 위해 제거해야 하는 줄의 개수인 clear_line을 반환합니다.
	int getClearLine() const;
};

