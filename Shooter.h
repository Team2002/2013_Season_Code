#ifndef SHOOTER_H
#define SHOOTER_H

#include "WPILib.h"
#include "math.h"

class Shooter
{
	Jaguar *Spin1;
	Jaguar *Spin2;
	CANJaguar *Aim;
	DoubleSolenoid *Loader;
	Timer *loadTimer;
public:
	Shooter();
	~Shooter();

	void Start();										//Sets up the variables for operator control
	void OpControl(bool spin, float angle, bool fire);	//Controls for operator control
	void AutoAim(float angle);							//Autonomous command to aim the shooter
	void SpinOn();										//Autonomous command to turn shooter on
	void SpinOff();										//Autonomous command to turn shooter off
private:
	bool isFiring;
	int step;

	bool isSpinning;
	bool pressed;
};
#endif
