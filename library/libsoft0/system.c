//event
void initevent();
void killevent();
//file
void initfile();
void killfile();
//process
void initprocess();
void killprocess();
//random
void initrandom();
void killrandom();
//socket
void initsocket();
void killsocket();




void initsystem(char* addr)
{
	initevent();		//1
	initfile();			//2
	//initprocess();	//4
	initrandom();		//5
	//initsocket();		//6
}
void killsystem()
{
	//killsocket();		//6
	killrandom();		//5
	//killprocess();	//4
	killfile();			//2
	killevent();		//1
}
