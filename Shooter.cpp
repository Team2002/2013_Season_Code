#include "Shooter.h"

Shooter::Shooter()
{
	Spin1 = new Jaguar(3);
	Spin2 = new Jaguar(4);

	Aim = new CANJaguar(2, CANJaguar::kPosition);
	Loader = new DoubleSolenoid(1, 2);
	loadTimer = new Timer();
	
}

void Shooter::Start()
{
	isFiring = false;
	step = 0;
	isSpinning = false;
	pressed = false;
	

	Aim->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	Aim->SetPID(40, .003, .5);
	Aim->ConfigEncoderCodesPerRev(360);
	Aim->ConfigMaxOutputVoltage(8.0);
	Aim->EnableControl(0);
}

void Shooter::OpControl(bool spin, float angle, bool fire)
{
	Aim->Set(angle * 90);

	if (spin && !isSpinning && !pressed)
	{
		Spin1->Set(-.5);
		Spin2->Set(-.5);
		pressed = true;
		isSpinning = true;
	}
	else if (spin && isSpinning && !pressed)
	{
		Spin1->Set(0);
		Spin2->Set(0);
		pressed = true;
		isSpinning = false;
	}
	else if (!spin && pressed)
	{
		pressed = false;
	}

	if (fire)
	{
		isFiring = true;
	}

	if (isFiring)
	{
		switch (step)
		{
			case 0:
			{
				Loader->Set(DoubleSolenoid::kForward);
				loadTimer->Start();
				step = 1;
			}
			break;
			case 1:
			{
				if(loadTimer->HasPeriodPassed(.5))
				{
					Loader->Set(DoubleSolenoid::kReverse);
					step = 2;
				}
			}
			break;
			case 2:
			{
				if(loadTimer->HasPeriodPassed(1))
				{
					Loader->Set(DoubleSolenoid::kOff);
					loadTimer->Reset();
					loadTimer->Stop();
					step = 0;
					isFiring = false;
				}
			}
			break;
		}
	}
}

void Shooter::AutoAim(float angle)
{
	Aim->Set(angle * 90);
}

void Shooter::SpinOn()
{
	Spin1->Set(-1.0);
	Spin2->Set(-1.0);
}

void Shooter::SpinOff()
{
	Spin1->Set(0);
	Spin2->Set(0);
}
