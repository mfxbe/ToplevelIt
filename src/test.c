#include <stdio.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <assert.h>
#include "toplevelit.h"

//Test functions ---------------------------------------------------------------
void test_open(gpointer, ToplevelItWindow *object){
	assert(TOPLEVELIT_IS_WINDOW(object));
}

void test_close(gpointer, ToplevelItWindow *object){
	assert(TOPLEVELIT_IS_WINDOW(object));
	assert(strcmp(toplevelit_window_get_title(object), "ToplevelItTestWindow") == 0);
}

//Setup and starting tests ------------------------------------------------------
void do_m_loop(){
	for(int i=0; i<5; i++){
		g_main_context_iteration(g_main_context_default(), FALSE);
	}
	sleep(1);
}

static void activate(GtkApplication *app){
	GtkWidget *window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "ToplevelItTestWindow");
	gtk_window_present(GTK_WINDOW(window));
	do_m_loop();

	//Testing
	ToplevelItManager *tliM = toplevelit_manager_new();
	do_m_loop();

	//Get signal of window
	g_signal_connect(tliM, "window-closed", G_CALLBACK(test_open), NULL);
	do_m_loop();

	//Get a list of windows
	GList* ws = toplevelit_manager_get_windows(tliM);
	ToplevelItWindow *ownWin = NULL;
	for(uint i=0; i<g_list_length(ws); i++){
		ToplevelItWindow *w = (ToplevelItWindow*) g_list_nth_data(ws, i);
		assert(TOPLEVELIT_IS_WINDOW(w));
		if (strcmp(toplevelit_window_get_title(w), "ToplevelItTestWindow") == 0){
			ownWin = w;
		}
	}
	assert(ownWin);
	do_m_loop();

	//Test minimizing
	toplevelit_window_set_state(ownWin, TOPLEVELIT_WINDOW_STATUS_MINIMIZED);
	do_m_loop();
	assert(toplevelit_window_get_state(ownWin) == TOPLEVELIT_WINDOW_STATUS_MINIMIZED);


	//Get close signal of window
	g_signal_connect(tliM, "window-closed", G_CALLBACK(test_close), NULL);
	gtk_window_close(GTK_WINDOW(window));
	do_m_loop();

	//Open another window
	GtkWidget *window2 = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window2), "ToplevelItTestWindow");
	gtk_window_present(GTK_WINDOW(window2));
	do_m_loop();

	//Check if existent now
	GList* ws2 = toplevelit_manager_get_windows(tliM);
	do_m_loop();
	do_m_loop();
	ToplevelItWindow *ownWin2 = NULL;
	for(uint i=0; i<g_list_length(ws2); i++){
		ToplevelItWindow *w2 = (ToplevelItWindow*) g_list_nth_data(ws2, i);
		assert(TOPLEVELIT_IS_WINDOW(w2));
		if (strcmp(toplevelit_window_get_title(w2), "ToplevelItTestWindow") == 0){
			ownWin2 = w2;
		}
	}
	assert(ownWin2);
	do_m_loop();

	//Close from library and check if closed
	toplevelit_window_close(ownWin2);
	do_m_loop();
	assert(GTK_IS_WIDGET(window2) == FALSE);


	g_application_quit(G_APPLICATION(app));
	printf("Tests worked");
}

int main(int argc, char **argv){
	GtkApplication *app = gtk_application_new("eu.feritale.ToplevelItTest", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	g_application_run(G_APPLICATION (app), argc, argv);
	g_object_unref(app);
	return 0;
}