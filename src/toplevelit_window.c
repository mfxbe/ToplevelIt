#include <glib-object.h>
#include "toplevelit_window.h"
#include "internal.h"

enum {
  CHANGED,
  LAST_SIGNAL
};
static guint toplevelit_window_signals[LAST_SIGNAL] = { 0 };

struct _ToplevelItWindow{
    GObject parent_instance;
    char* app_id;
    int status;
    int active;
};
G_DEFINE_TYPE(ToplevelItWindow, toplevelit_window, G_TYPE_OBJECT)

static void toplevelit_window_finalize(GObject *obj){
  //ToplevelItWindow *self = TOPLEVELIT_WINDOW(obj);
  G_OBJECT_CLASS(toplevelit_window_parent_class)->finalize(obj);
}

static void toplevelit_window_class_init(ToplevelItWindowClass *klass){
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = toplevelit_window_finalize;
	
	toplevelit_window_signals[CHANGED] = g_signal_newv("changed",
		G_TYPE_FROM_CLASS(object_class), 
		G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
		NULL, NULL, NULL, NULL, G_TYPE_NONE, 0, NULL);
}

static void toplevelit_window_init(ToplevelItWindow*){
	//Init
}

ToplevelItWindow *toplevelit_window_new(void){
	return g_object_new(TOPLEVELIT_TYPE_WINDOW, 0);
}

void toplevel_window_changed(ToplevelItWindow *win){
	g_signal_emit(win, toplevelit_window_signals[CHANGED], 0);
}

void toplevel_window_set_app_id(ToplevelItWindow *self, const gchar* data){
	if(self->app_id == NULL){
		self->app_id = g_strdup_printf("%s", data);
	}
}

gchar *toplevel_window_get_app_id(ToplevelItWindow *self){
		return(self->app_id);
}
