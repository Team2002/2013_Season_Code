#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include "WPILib.h"
#include "math.h"

#define pi 3.14159
#define Rw  3.0
#define	Dw  26.0
#define	Tr  360.0
#define toFeet 12.0

#define TickstoFeet 0.00455

class DriveTrain
{
	Jaguar *driveLeft;
	Jaguar *driveRight;
	
	Jaguar *LEDs;

	Encoder *encLeft;
	Encoder *encRight;
	DriverStationLCD *m_LCD;

	PIDController *pidLeft;
	PIDController *pidRight;
	PIDController *pidTL;
	PIDController *pidTR;
	
	Timer *breath;
public:
	int E1;
	int E2;
	float theta;
	float DistX;
	float DistY;

	DriveTrain();
	~DriveTrain();

	void OpStart();						//Sets up variables for operator control
	void AutoStart();					//Sets up variables for autonomous control
	void OpControlARC(float x, float y);//Controls for operator control
	void OpControlTANK(float y,float x);
	void OpControlTHIAS(float x, float y, float z, float slow);
	void AutoStraight(float target);	//Autonomous command to go straight for a set length
	void AutoTurn(float target);		//Autonomous command to turn a certain number of degrees
	void OpDisp();
	void LED();

	void DeadStart();					//Sets up dead reckoning variables
	void DeadReset();					//Resets the encoders on the drivetrain
	void DeadStop();					//Stops tracking encoder movement
	void DeadUpdate();					//Calculates displacement and angle from encoder movement
	void DeadDisplay();					//Displays displacement and angle to driverstation
	void AutoStop();
private:
	float targetX;
	float targetY;
	float a;

	float Dt;
	float Dy;
	float Dx;
	float T1;
	float T2;
	
	float Lx;
	float Rx;
};
#endif
