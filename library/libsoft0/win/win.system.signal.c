#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define u64 unsigned long long
void eventwrite(u64,u64);




BOOL CtrlHandler( DWORD fdwCtrlType ) 
{
	switch( fdwCtrlType ) 
	{ 
		// Handle the CTRL-C signal. 
		case CTRL_C_EVENT: 
			//printf( "Ctrl-C event\n\n" );
			eventwrite(0x3, 0x64626b);
			return( TRUE );
 
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
