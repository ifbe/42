#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//event
int uievent(u64* first,u64* second);
int softevent(u64* first,u64* second);
int hardevent(u64* first,u64* second);
int bootevent(u64* first,u64* second);
//
void say(char*,...);




int stillalive=1;




int eventwrite()
{
	stillalive=0;
	return 0;
}
int eventread(u64* first,u64* second)
{
        int ret;
        //say("here\n");
        if(stillalive==0)
        {
                first[0]=0;
                return 0;
        }

        //调试端口有没有消息
        ret=bootevent(first,second);
        if(ret>0)return 11;

        //硬件中断完成状态报告
        ret=hardevent(first,second);
        if(ret>0)return 22;

        //输入/网络/系统事件
        ret=softevent(first,second);
        if(ret>0)return 33;

        //窗口关闭,窗口大小变化等情况
        ret=uievent(first,second);
        if(ret>0)return 44;
}
