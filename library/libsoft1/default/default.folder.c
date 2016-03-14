int openfolder(char* p);
int closefolder(char* p);
int listfolder(char* p);
int switchfolder(char* p);
int readfolder(char* p);
int writefolder(char* p);




void folder_init()
{
}
void folder_kill()
{
}
int folder_open(char* p)
{
	return openfolder(p);
}
void folder_close(char* p)
{
	closefolder(p);
}




void folder_list(char* p)
{
	listfolder(p);
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
