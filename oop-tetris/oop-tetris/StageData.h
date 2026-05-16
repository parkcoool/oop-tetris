#pragma once
class StageData
{
private:
	// speed : 낮을수록 블록이 빠르게 내려온다.
	// stick_rate : 높을수록 I자 모양 블록이 나올 확률이 높다.
	// clear_line : 다음 레벨로 넘어가기 위해 제거해야 하는 줄의 개수이다.
	int speed, stick_rate, clear_line;
public:
	StageData(int speed, int stick_rate, int clear_line);

	int getSpeed() const;
	int getRate() const;
	int getClearLine() const;
};

