//listen,say,diary,and
void initlisten(char*);
void initsay(char*);
void initdiary(char*);
void inithistory(char*);

void initstd(char* p)
{
	initlisten(	p+0x0);
	initsay	(	p+0x100000); 
	initdiary(	p+0x200000);
	inithistory(	p+0x300000);
}
void killstd()
{
}
