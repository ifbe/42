#define u64 unsigned long long
//
int startvision();
int stopvision();
//
void printmemory(void*, int);
void say(void*, ...);




static int bb = 0;
int vision_explain(u64* p)
{
	if(bb != 0)say("vision@%x\n",p[0]);
	return 42;
}




int vision_list()
{
	return 0;
}
int vision_choose()
{
	return 0;
}
int vision_read()
{
	return 0;
}
int vision_write()
{
	return 0;
}
int vision_start()
{
	bb = 1;
	return startvision();
}
int vision_stop()
{
	bb = 0;
	return stopvision();
}
void vision_create(void* w)
{
}
void vision_delete()
{
}

