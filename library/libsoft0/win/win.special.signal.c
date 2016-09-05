#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define u64 unsigned long long
void (*death)();




BOOL CtrlHandler( DWORD fdwCtrlType ) 
{
	switch( fdwCtrlType ) 
	{ 
		// Handle the CTRL-C signal. 
		case CTRL_C_EVENT: 
			printf( "Ctrl-C event\n\n" );
			death();
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
			Beep( 750, 500 ); 
			printf( "Ctrl-Shutdown event\n\n" );
			return FALSE; 
 
		default: 
			return FALSE; 
	}
} 
void signalcreate(char* what, void* handler)
{
	if(what == 0)return;
	if(what[0] != 'c')return;

	printf("ctrl+c=%llx\n",(u64)handler);
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)handler, 1);
}
