#include "StageData.h"

StageData::StageData(int speed, int stick_rate, int clear_line)
	: speed(speed), stick_rate(stick_rate), clear_line(clear_line)
{}

int StageData::getSpeed() const {
	return speed;
}
int StageData::getRate() const {
	return stick_rate;
}
int StageData::getClearLine() const {
	return clear_line;
}