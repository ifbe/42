int systemopen(int type,char* p);
int systemclose(char* p);
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
int folder_open(char* p)
{
	return systemopen(0,p);
}
void folder_close(char* p)
{
	systemclose(p);
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
