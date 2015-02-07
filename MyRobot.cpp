#include "WPILib.h"
#include "math.h"
#include "DriveTrain.h"
#include "Shooter.h"
#include "Climber.h"
#include "Dumper.h"

#include "math.h"


class RobotDemo : public SimpleRobot
{
	Joystick *driveJoy;
	Joystick *elseJoy;
	
	DriveTrain *drive;
	Shooter *shoot;
	Climber *climber;
	Dumper *dumper;
	
	DriverStation *ds;
	Relay *relay;
	Servo *servo;
	
	Encoder *enl;
public:
	RobotDemo(void)
	{
		driveJoy = new Joystick(1); //Joystick with driving controls
		elseJoy = new Joystick(2);  //Other Joystick
		
		drive = new DriveTrain();   //All the drive related stuff including pids and dead reckoning
		shoot = new Shooter();      //All the shooter related stuff
		climber = new Climber();	//All the climbing stuff
		dumper = new Dumper();		//All the dumper/hopper stuff
		
		ds = DriverStation::GetInstance(); //Switchboard
		enl = new Encoder(1, 2, false);
	}
	

	void Autonomous(void)
	{
		//Set everything up//
		drive->AutoStart();
		drive->DeadStart();
		enl->Start();
		
		drive->AutoTurn(180);
		while(abs(enl->Get()) < 800 && IsAutonomous())
		{
			drive->LED();
		}
		
		drive->AutoStraight(10);
		while(abs(enl->Get()) < 2000 && IsAutonomous())
		{
			drive->LED();
		}
		//Stop Dead reckoning tracking//
		drive->AutoStop();
	}

	void OperatorControl(void)
	{
		//Set everything up//
		drive->OpStart();
		shoot->Start();
		climber->Start();
		drive->DeadStart();
		drive->DeadReset();

		while (IsOperatorControl())
		{
			//Controls//
			drive->OpControlTHIAS(elseJoy->GetX(), elseJoy->GetY(), elseJoy->GetZ(), elseJoy->GetRawButton(2));
			drive->DeadUpdate();
			drive->LED();
			//drive->OpDisp();
			shoot->OpControl(elseJoy->GetRawButton(1), driveJoy->GetY(), driveJoy->GetRawButton(3));
			climber->AutoClimb(elseJoy->GetRawButton(3), elseJoy->GetRawButton(4), driveJoy->GetRawAxis(4));
			climber->RegularOp();
			dumper->OpControl(elseJoy->GetRawButton(6), elseJoy->GetRawButton(7));
			
			//Updating the dead reckoning display//
			//drive->DeadUpdate();
			//drive->DeadDisplay();

			Wait(.005);
		}
	}
	
	
};

START_ROBOT_CLASS(RobotDemo);

