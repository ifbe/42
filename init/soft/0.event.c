#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
static unsigned char* inbuf;




//void waitevent(QWORD* type,QWORD* value)
//{
	//优先级
	//while(1)
	//{
		//上下，esc，enter......
		//if(kbd)
			//return "kbd",value

		//"张"，"x"，" "......
		//if(char)
			//reurn "char",value

		//(w<<48) + (z<<32) + (y<<16) + x
		//if(mouse)
			//return "mouse",value

		//上滚，下滚，按下，侧键
		//if(wheel)
			//return "wheel",value

		//tick,tock,tick,tock,tick......
		//if(time)
			//return "ticktock",value

		//message form 192.168.1.99:tcp1357:hahahaha
		//if(net)
			//return "net",value

		//screen resolution change
		//if(change)
			//return "screen",value
	//}
//}




void initin(unsigned long long addr)
{
	inbuf=(void*)addr;
}
