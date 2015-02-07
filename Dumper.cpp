#include "Dumper.h"

Dumper::Dumper():
push (9)
{
	dumpSwitch = new DigitalInput(8);
}

void Dumper::OpControl(bool forward, bool reverse)
{
	if(forward)
	{
		push.Set(1.0);
	}
	else
	{
		push.Set(0);
	}
}
