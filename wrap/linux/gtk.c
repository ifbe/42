#include<gtk/gtk.h>


int main(int argc,char** argv)
{
	gtk_init(&argc,&argv);

	GtkWidget* window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);

	gtk_window_set_title(GTK_WINDOW(window),"hello");
	gtk_window_set_default_size(GTK_WINDOW(window),1024,640);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_widget_set_opacity(window,0.75);
	gtk_widget_show(window);

	gtk_main();
	return 0;
}
