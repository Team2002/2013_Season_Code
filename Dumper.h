#ifndef DUMPER_H
#define DUMPER_H

#include "WPILib.h"
#include "math.h"

class Dumper
{
	DigitalInput *dumpSwitch;
	Servo push;
public:
	Dumper();
	~Dumper();
	
	void OpControl(bool forward, bool reverse);
private:
	
};
#endif
