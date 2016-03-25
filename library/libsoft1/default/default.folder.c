int systemstart(int type,char* p);
int systemstop(char* p);
int systemlist(char* p);
int systemswitch(char* p);
int systemread(char* p);
int systemwrite(char* p);




void folder_init()
{
}
void folder_kill()
{
}
int folder_start(char* p)
{
	return systemstart(0,p);
}
void folder_stop(char* p)
{
	systemstop(p);
}




void folder_list(char* p)
{
	systemlist(p);
}
void folder_switch()
{
}
void folder_read()
{
}
void folder_write()
{
}
