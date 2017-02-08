#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//soft.artery
int arteryprompt();
int arterycommand(void*);
void motion_explain(void*);
void network_explain(void*);
void sound_explain(void*);
void vision_explain(void*);
//boot
void printmemory(char*, int);
void say(char*, ...);
//event
u64* eventread();
void eventwrite(u64 why, u64 what, u64 where, u64 when);
int birth();
int death();




int main(int argc,char* argv[])
{
	int ret;
	u64* addr;

	//before
	birth();

	//config
	for(ret=1;ret<argc;ret++)
	{
		arterycommand( argv[ret] );
		arterycommand( "\n" );
	}

	//forever
	while(1)
	{
/*
                //1.display
                //[+00,+07]addr         pointer to actual memory
                //[+08,+0f]fmt          rgba, text, html, ...
                //[+10,+17]width
                //[+18,+1f]height
                for(ret=0;ret<max;ret++)
                {
                        characterread(list + ret*0x20);
                        windowwrite(  list + ret*0x20);
                }
                fps++;
*/
		arteryprompt();

again:
                //2.event
                //[+00,+07]why
                //[+08,+0f]what
                //[+10,+17]where
                //[+18,+1f]when
		addr = eventread();
		if(addr == 0)break;		//error
		if(addr[1] == 0)break;		//exit


		//3.pre change
		if((addr[1]&0xff) == 'p')	//touch
		{
			motion_explain(addr);
		}
		else if((addr[1]&0xff) == 'n')	//net
		{
			network_explain(addr);
		}
		else if((addr[1]&0xff) == 's')	//sound
		{
			sound_explain(addr);
		}
		else if((addr[1]&0xff) == 'v')	//vision
		{
			vision_explain(addr);
		}
		if(addr[1] != 0x64626b)goto again;


		//4.real change
		arterycommand(addr);
	}

	//after
	death();
	return 0;
}
