#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<gtk/gtk.h>
#include"libuser.h"




static u64 thread;




void* uievent(void* p)
{
	GtkWidget *window;
	GtkWidget *button;
	int argc = 1;
	char* argv[2];
	void* haha=argv;
	argv[0] = "./a.out";
	argv[1] = 0;
	gtk_init (&argc, (void*)&haha);

	//
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 512, 512);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_show (window);

	//
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	//
	gtk_main();
	eventwrite(0, 0, 0, 0);
	return 0;
}




void windowchange()
{
}
void windowlist()
{
}
void windowread()
{
}
void windowwrite()
{
}
void windowstop()
{
}
void windowstart(char* addr, char* pixfmt, int x, int y)
{
}
void windowdelete()
{
}
void windowcreate()
{
}




void initwindow()
{
	thread = startthread(uievent, 0);
}
void freewindow()
{
}
