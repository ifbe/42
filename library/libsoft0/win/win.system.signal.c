#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define u64 unsigned long long
u64 gettime();
void eventwrite(u64,u64,u64,u64);




static u64 die=0,old=0,new=0;




BOOL CtrlHandler( DWORD fdwCtrlType ) 
{
	switch( fdwCtrlType ) 
	{ 
		// Handle the CTRL-C signal. 
		case CTRL_C_EVENT: 
		{
			die = old;
			old = new;
			new = gettime();

			if(new-old < 500*1000)  //0.5 s
			{
				if(new-die < 1000*1000)exit(-1);
				else eventwrite(0, 0, 0, 0);
			}
			else eventwrite(0x3, 0x64626b, 0, 0);
			return( TRUE );
 		}
		// CTRL-CLOSE: confirm that the user wants to exit. 
		case CTRL_CLOSE_EVENT: 
			printf( "Ctrl-Close event\n\n" );
			return( TRUE ); 
 
		// Pass other signals to the next handler. 
		case CTRL_BREAK_EVENT: 
			printf( "Ctrl-Break event\n\n" );
			return FALSE; 
 
		case CTRL_LOGOFF_EVENT: 
			printf( "Ctrl-Logoff event\n\n" );
			return FALSE; 
 
		case CTRL_SHUTDOWN_EVENT: 
			printf( "Ctrl-Shutdown event\n\n" );
			return FALSE; 
 
		default: 
			return FALSE; 
	}
} 
void createsignal()
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, 1);
}
void deletesignal()
{
}
