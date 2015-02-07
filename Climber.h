#ifndef CLIMBER_H
#define CLIMBER_H

#include "WPILib.h"

class Climber
{
	Jaguar *elevator;
	DigitalInput Bleft;
	DigitalInput Bright;
	DigitalInput Tleft1;
	DigitalInput Tright1;
	DigitalInput Tleft2;
	DigitalInput Tright2;
	DigitalInput Top;
	DigitalInput Bottom;
	DoubleSolenoid *Pull;
	
	DriverStationLCD *c_LCD;
	Timer *timer;
public:
	Climber();
	~Climber();
	
	void Start();
	void AutoClimb(bool climb, bool kill, float joy);
	void RegularOp();
	void ClimbUp();
	void ClimbDown();
	void Safe();
	void SmallClimb(bool up, bool down);
private:
	int step;
	bool isClimbing;
	bool isSafe;
	
};
#endif
