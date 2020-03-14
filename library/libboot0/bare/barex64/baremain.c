#include "libboot.h"
u8 in8(u16);




int readkbd_one(u8* buf)
{
	u8 flag = in8(0x64);
	if( (flag&1) == 0)return 0;

	buf[0] = in8(0x60);
	return 1;
}
int readkbd(u8* buf, int len)
{
	int j,ret;
	for(j=0;j<len;j++)
	{
		ret = readkbd_one(buf+j);
		if(ret == 0)break;
	}
	return j;
}



static u8 kbd[9*2] =
{
	0x1,0x1b,       //esc
	0x47,0x47,      //home
	0x4f,0x4f,      //end
	0x49,0x49,      //page up
	0x51,0x51,      //page down
	0x48,0x48,      //up
	0x50,0x50,      //down
	0x4b,0x4b,      //left
	0x4d,0x4d       //right
};
static u8 ch[41*2] =
{
0x02,'1',
0x03,'2',
0x04,'3',
0x05,'4',
0x06,'5',
0x07,'6',
0x08,'7',
0x09,'8',
0x0a,'9',
0x0b,'0',
0x1e,'a',
0x30,'b',
0x2e,'c',
0x20,'d',
0x12,'e',
0x21,'f',
0x22,'g',
0x23,'h',
0x17,'i',
0x24,'j',
0x25,'k',
0x26,'l',
0x32,'m',
0x31,'n',
0x18,'o',
0x19,'p',
0x10,'q',
0x13,'r',
0x1f,'s',
0x14,'t',
0x16,'u',
0x2f,'v',
0x11,'w',
0x2d,'x',
0x15,'y',
0x2c,'z',
0x34,'.',
0x0e,0x7f,      //backspace
0x1c,0xd,       //enter
0x39,0x20,      //space
0x00,0x00
};
static struct event ev[1];
void* pollenv()
{
	int j;
	u8 buf[1];
	j = readkbd(buf, 1);
	if(j == 0)return 0;
	if(buf[0] >= 0x80)return 0;

	//kbd
	ev->what = 0;
	ev->why = buf[0];
	if(ev->what==0)
	{
		for(j=0;j<9;j++)
		{
			if(ev->why == kbd[j*2])
			{
				ev->what = 0x64626b;
				ev->why = kbd[(j*2) + 1];
				break;
			}
		}
	}

	//char
	if(ev->what==0)
	{
		for(j=0;j<41;j++)
		{
			if(ev->why == ch[j*2])
			{
				ev->what = 0x72616863;
				ev->why = ch[(j*2) + 1];
				break;
			}
		}
	}

	return ev;
}




int main()
{
	void* all = pwrclkcreate(_start_, 0, 0, 0);
	void* wrk = workercreate(0, 0, 0, 0);
	workerdelete(wrk);
	pwrclkdelete(all);
	return 0;
}