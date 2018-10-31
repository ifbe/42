#include "libsoft.h"




#define hole 0x656c6f68
#define HOLE 0x454c4f48
int hole_client(struct object* obj, int fd, u8* buf, int len)
{
	u64* aa;
	u64* bb;
	if((buf[0] == 2) && (buf[1] == 0) )
	{
		aa = (void*)(obj[fd].peer);
		bb = (void*)buf;

		say("target=%llx\n",bb[0]);
		aa[0] = bb[0];

		//writesocket(fd, 0, "hole\n", 5);
		//writesocket(fd, 0, "hole\n", 5);
	}

	printmemory(buf, len);
	return 0;
}
int hole_server(struct object* obj, int fd, u8* buf, int len)
{
	u64* aa;
	u64* bb;
	u8* q = obj[fd].peer;
	say(
		"[%d.%d.%d.%d:%d]%x,%x\n",
		q[4],q[5],q[6],q[7],
		(((int)q[2])<<8) + q[3],
		buf[0], buf[1], buf[2], buf[3]
	);
	if(buf[0] == '1')
	{
		aa = (void*)(obj[fd].data);
		bb = (void*)(obj[fd].peer);
		aa[1] = bb[0];

		if(aa[0] != 0)
		{
			//writesocket(fd, 0, aa, 8);

			bb[0] = aa[0];
			//writesocket(fd, 0, aa+1, 8);

			aa[0] = aa[1] = 0;
		}
	}
	else if(buf[0] == '0')
	{
		aa = (void*)(obj[fd].data);
		bb = (void*)(obj[fd].peer);
		aa[0] = bb[0];

		if(aa[1] != 0)
		{
			//writesocket(fd, 0, aa+1, 8);

			bb[0] = aa[1];
			//writesocket(fd, 0, aa, 8);

			aa[0] = aa[1] = 0;
		}
	}
	return 0;
}
