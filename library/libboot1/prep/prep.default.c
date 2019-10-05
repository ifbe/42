#include "libuser.h"




void role_default()
{
	say("@role_default\n");

	//+libhard0
	//devicecreate(_ahci_, 0);
	//devicecreate(_xhci_, 0);

	//+libhard1
	//drivercreate(_sata_, 0);
	//drivercreate(_usb_, 0);

	//+libsoft0
	//systemcreate(_uart_, "/dev/ptmx");
	//systemcreate(_uart_, "/dev/ttyACM0");

	//+libsoft1
	arterycreate(0,   "HACK://0.0.0.0:2222", 0, 0);
	arterycreate(0,   "QUIC://0.0.0.0:4444", 0, 0);
	arterycreate(0,    "SSH://0.0.0.0:2022", 0, 0);
	arterycreate(0, "TELNET://0.0.0.0:2023", 0, 0);
	arterycreate(0,   "HTTP://0.0.0.0:2080", 0, 0);
	arterycreate(0,    "TLS://0.0.0.0:2443", 0, 0);
	arterycreate(0,  "SERVE://0.0.0.0:2099", 0, 0);
}