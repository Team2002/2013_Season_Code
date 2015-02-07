#include "DriveTrain.h"

DriveTrain::DriveTrain()
{
	driveLeft = new Jaguar(1);
	driveRight = new Jaguar(2);
	LEDs = new Jaguar(5);

	encLeft = new Encoder(1, 2, false);
	encRight = new Encoder(3, 4, false);
	m_LCD = DriverStationLCD::GetInstance();

	pidLeft = new PIDController(.0001, 0.00000005, 0.00001, encLeft, driveLeft);
	pidRight = new PIDController(.0001, 0.000000006, 0.00001, encRight, driveRight);
	
	pidTL = new PIDController(.0001, 0.00000007, 0.00001, encLeft, driveLeft);
	pidTR = new PIDController(.0001, 0.00000007, 0.00001, encRight, driveRight);
	
	breath = new Timer;
	breath->Start();

	driveLeft->SetExpiration(.1);
	driveRight->SetExpiration(.1);
}

void DriveTrain::OpStart()
{
	driveLeft->SetSafetyEnabled(true);
	driveRight->SetSafetyEnabled(true);
	breath->Start();
	
	a = 0.5;
	Lx = 0;
	Rx = 0;
}

void DriveTrain::AutoStart()
{
	driveLeft->SetSafetyEnabled(false);
	driveRight->SetSafetyEnabled(false);
	pidLeft->Enable();
	pidRight->Enable();
	pidTL->Enable();
	pidTR->Enable();
}

void DriveTrain::OpControlARC(float x, float y)
{
	targetY = a * (y * y * y) + (1 - a) * y;
	targetX = a * (x * x * x) + (1 - a) * x;
	driveLeft->Set((targetY - targetX));
	driveRight->Set((targetY + targetX));
}

void DriveTrain::OpControlTANK(float y, float x)
{
	targetY = a * (x * y * y) + (1 - a) * y;
	targetX = a * (x * x * x) + (1 - a) * x;
	driveLeft->Set(targetY);
	driveRight->Set(targetX);
}

void DriveTrain::OpControlTHIAS(float x, float y, float z, float slow)
{
	targetY = a * (x * y * y) + (1 - a) * y;
	targetX = a * (x * x * x) + (1 - a) * x;
	
	if(targetX > 0)
	{
		Lx = targetX;
		Rx = 0;
	}
	else if(targetX <  0)
	{
		Rx = targetX;
		Lx = 0;
	}
	else
	{
		Rx = 0;
		Lx = 0;
	}
	
	z *= .5;
	Lx *= .6;
	Rx *= .6;
	
	if(slow)
	{
		targetY *= .4;
		Lx *= .4;
		Rx *= .4;
		z *=.6;
	}
	
	driveLeft->Set((targetY - Lx) - z);
	driveRight->Set((targetY + Rx) + z);
	
	
}

void DriveTrain::AutoStraight(float target)
{
	pidLeft->Enable();
	pidRight->Enable();
	pidTL->Disable();
	pidTR->Disable();
	encLeft->Reset();
	encRight->Reset();
	pidLeft->SetSetpoint((target / TickstoFeet) * -1);
	pidRight->SetSetpoint((target / TickstoFeet) * -1);
}

void DriveTrain::AutoTurn(float target)
{
	pidLeft->Disable();
	pidRight->Disable();
	pidTL->Enable();
	pidTR->Enable();
	encLeft->Reset();
	encRight->Reset();
	pidTL->SetSetpoint(pi * Dw * (target / 360) / Rw / TickstoFeet);
	pidTR->SetSetpoint(pi * Dw * (target / 360) / Rw * -1 / TickstoFeet);
}

void DriveTrain::OpDisp()
{
	m_LCD->Printf(DriverStationLCD::kUser_Line1, 2, "Left: %f", (driveLeft->Get()));
	m_LCD->Printf(DriverStationLCD::kUser_Line2, 2, "Right: %f", (driveRight->Get()));
	m_LCD->UpdateLCD();
}

void DriveTrain::LED()
{
	LEDs->Set(.6 + cos( breath->Get()) *-.45);
}

void DriveTrain::DeadStart()
{
	encLeft->Start();
	encRight->Start();
	encLeft->Reset();
	encRight->Reset();

	E1 = 0;
	E2 = 0;
	DistX = 0;
	DistY = 0;
	theta = 0;

	Dt = 0;
	Dy = 0;
	Dx = 0;
	T1 = 0;
	T2 = 0;
}

void DriveTrain::DeadReset()
{
	encLeft->Reset();
	encRight->Reset();

	E1 = 0;
	E2 = 0;
	DistX = 0;
	DistY = 0;
	theta = 0;
}

void DriveTrain::DeadStop()
{
	encLeft->Stop();
	encRight->Stop();
}

void DriveTrain::DeadUpdate()
{
	T1 = (encLeft->Get() * -1) - E1;
	T2 = (encRight->Get() * -1) - E2;

	Dt = 2 * pi * (Rw / Dw) * ((T1 - T2) / Tr);
	theta += Dt;

	if (theta > pi * 2)
	{
		theta -= pi * 2;
	}
	else if (theta < 0)
	{
		theta += pi * 2;
	}
	Dx = Rw * cos(theta) * (T1 + T2) * (pi / Tr);
	Dy = Rw * sin(theta) * (T1 + T2) * (pi / Tr);

	DistX += Dx;
	DistY += Dy;

	E1 = (encLeft->Get()*-1);
	E2 = (encRight->Get()*-1);
}

void DriveTrain::DeadDisplay()
{
	m_LCD->Printf(DriverStationLCD::kUser_Line1, 2, "Left: %5i     ", encLeft->Get());
	m_LCD->Printf(DriverStationLCD::kUser_Line2, 2, "Right: %5i    ", encRight->Get());

	m_LCD->Printf(DriverStationLCD::kUser_Line3, 2, "X: %f feet     ", (DistX
			/ toFeet));
	m_LCD->Printf(DriverStationLCD::kUser_Line4, 2, "Y: %f feet     ", (DistY
			/ toFeet));
	m_LCD->Printf(DriverStationLCD::kUser_Line5, 2, "Angle: %i degrees   ", (int) (theta
			* (180 / pi)));
	m_LCD->UpdateLCD();
}

void DriveTrain::AutoStop()
{
	pidLeft->Disable();
	pidRight->Disable();
	pidTL->Disable();
	pidTR->Disable();
}
