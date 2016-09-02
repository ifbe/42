#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//event
int uievent(  u64* who, u64* what, u64* how);
int softevent(u64* who, u64* what, u64* how);
int hardevent(u64* who, u64* what, u64* how);
int bootevent(u64* who, u64* what, u64* how);
//
void say(char*,...);




int stillalive=1;




int eventwrite()
{
	stillalive=0;
	return 0;
}
int eventread(u64* who, u64* what, u64* how)
{
        int ret;
        //say("here\n");
        if(stillalive==0)
        {
                what[0]=0;
                return 0;
        }

        //调试端口有没有消息
        ret=bootevent(who, what, how);
        if(ret>0)return 11;

        //硬件中断完成状态报告
        ret=hardevent(who, what, how);
        if(ret>0)return 22;

        //输入/网络/系统事件
        ret=softevent(who, what, how);
        if(ret>0)return 33;

        //窗口关闭,窗口大小变化等情况
        ret=uievent(  who, what, how);
        if(ret>0)return 44;
}




void eventcreate()
{
}
void eventdelete()
{
}
