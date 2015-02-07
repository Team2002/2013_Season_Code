#include "Climber.h"

Climber::Climber():
Bleft (14),
Bright (13),
Tleft1 (12),
Tright1 (11),
Tleft2 (10),
Tright2 (9),
Top (8),
Bottom (7)
{
	elevator = new Jaguar(10);
	Pull = new DoubleSolenoid(1,2);
	
	c_LCD = DriverStationLCD::GetInstance();
	timer = new Timer;
	
	elevator->SetExpiration(.1);
}

void Climber::Start()
{
	elevator->SetSafetyEnabled(false);
	Pull->Set(DoubleSolenoid::kOff);
	step = 0;
	isClimbing = false;
}

void Climber::AutoClimb(bool climb, bool kill, float joy)
{
	if (!isClimbing)
	{
		if(!Top.Get() && joy < 0)
		{
			elevator->Set(0.0);
		}
		else if(!Bottom.Get() && joy > 0)
		{
			elevator->Set(0.0);
		}
		else
		{
			elevator->Set(joy * .5);
		}
	}
	if (climb)
	{
		isClimbing = true;
	}

	if (isClimbing)
	{
		switch (step)
		{
			case 0:
			{
				if(Top.Get())
				{
					elevator->Set(-.5);
				}
				else
				{
					step += 1;
				}
			}
			break;
			case 1:
			{
				if(Bottom.Get())
				{
					elevator->Set(.5);
				}
				else
				{
					step += 1;
				}
			}
			break;
			case 2:
			{
				if(Top.Get())
				{
					elevator->Set(-.5);
				}
				else
				{
					step += 1;
				}
			}
			break;
			case 3:
			{
				if(Bottom.Get())
				{
					elevator->Set(.5);
				}
				else
				{
					step += 1;
				}
			}
			break;
			case 4:
			{
				if(Top.Get())
				{
					elevator->Set(-.5);
				}
				else
				{
					step += 1;
				}
			}
			break;
			case 5:
			{
				if(Bottom.Get())
				{
					elevator->Set(.5);
				}
				else
				{
					step += 1;
				}
			}
			break;
			case 6:
			{
				if(((!Tright1.Get() || !Tright2.Get()) && (!Tleft1.Get() || !Tleft2.Get())) && (!Bright.Get() && !Bleft.Get()))
				{
					step += 1;
				}
				else
				{
					elevator->Set(-.2);
				}
			}
			break;
			case 7:
			{
				elevator->Set(0);
				step = 0;
				isClimbing = false;
			}
			break;
		}
	}
	
	if(kill && isClimbing)
	{
		isClimbing = false;
	}
	Climber::Safe();
}

void Climber::RegularOp()
{
	
}

void Climber::ClimbUp ()
{
	if(Top.Get() && isSafe)
	{
		elevator->Set(-.5);
	}
	else if(!Top.Get() && isSafe)
	{
		step += 1;
	}
	else
	{
		elevator->Set(0);
		isClimbing = false;
	}
}

void Climber::ClimbDown()
{
	if(Bottom.Get() && isSafe)
	{
		elevator->Set(-.5);
	}
	else if(!Bottom.Get() && isSafe)
	{
		step += 1;
	}
	else
	{
		elevator->Set(0);
		isClimbing = false;
	}
}

void Climber::Safe()
{
	if(((!Tright1.Get() || !Tright2.Get()) && (!Tleft1.Get() || !Tleft2.Get())) || (!Bright.Get() && !Bleft.Get()))
	{
		isSafe = true;
	}
	else
	{
		isSafe = false;
	}
}

void Climber::SmallClimb(bool up, bool down)
{
	if(up)
	{
		Pull->Set(DoubleSolenoid::kForward);
	}
	else if(down)
	{
		Pull->Set(DoubleSolenoid::kReverse);
	}
	else
	{
		Pull->Set(DoubleSolenoid::kOff);
	}
}
