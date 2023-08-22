#include <stdio.h>
#include <unistd.h>
#include <glib-object.h>
#include "toplevelit.h"

int checker(gpointer data){
	ToplevelItManager* myobj = (ToplevelItManager*)data;
	GList* list = toplevelit_manager_get_windows(myobj);
	GList* l;
	printf("\r");
	for (l = list; l != NULL; l = l->next){
		ToplevelItWindow* win = l->data;
		printf("%s, ", toplevel_window_get_app_id(win));
	}
	printf("                                                          ");
	return(TRUE);
}

void changed_eventt(ToplevelItManager*, gpointer test, gpointer tdest, ToplevelItWindow *win){
	g_print("wirklich bei ");
	g_print(toplevel_window_get_app_id(win));
}

int main(){
	GMainLoop* loop = g_main_loop_new(NULL, TRUE);
	printf("\nCurrent toplevels: \n");
	ToplevelItManager *myobj = toplevelit_manager_new();
	
	g_signal_connect(myobj, "window-changed", (GCallback) changed_eventt, NULL);
	
	//g_timeout_add(100, G_SOURCE_FUNC(checker), myobj);
	
	g_main_loop_run(loop);	
	return 0;
}
