#include <glib-object.h>
#include "toplevelit.h"
#include "toplevelit_window.h"
#include "internal.h"

enum {
  WINDOW_OPEND,
  WINDOW_CLOSED,
  WINDOW_CHANGED,
  LAST_SIGNAL
};
static guint toplevelit_manager_signals[LAST_SIGNAL] = { 0 };

struct _ToplevelItManager{
    GObject parent_instance;
    char *empty;
    GList *children;
};
G_DEFINE_TYPE(ToplevelItManager, toplevelit_manager, G_TYPE_OBJECT)

static void toplevelit_manager_finalize(GObject *obj){
	ToplevelItManager *self = TOPLEVELIT_MANAGER(obj);

	self->children = g_list_remove_all(self->children, NULL);
	g_free(self->empty);
	g_free(self->children);

  G_OBJECT_CLASS(toplevelit_manager_parent_class)->finalize(obj);
}

static void toplevelit_manager_class_init(ToplevelItManagerClass *klass){
	GObjectClass *object_class = G_OBJECT_CLASS(klass);	
	object_class->finalize = toplevelit_manager_finalize;
	
	GType arg_types[1] = { TOPLEVELIT_TYPE_WINDOW };
	
	toplevelit_manager_signals[WINDOW_CHANGED] = g_signal_newv("window-changed",
		G_TYPE_FROM_CLASS(object_class), 
		G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
		NULL, NULL, NULL, NULL, G_TYPE_NONE, 0, NULL);
		
	toplevelit_manager_signals[WINDOW_OPEND] = g_signal_newv("window-opend",
		G_TYPE_FROM_CLASS(object_class), 
		G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
		NULL, NULL, NULL, NULL, G_TYPE_NONE, 1, arg_types);
		
	toplevelit_manager_signals[WINDOW_CLOSED] = g_signal_newv("window-closed",
		G_TYPE_FROM_CLASS(object_class), 
		G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
		NULL, NULL, NULL, NULL, G_TYPE_NONE, 0, NULL);
}

static void toplevelit_manager_init(ToplevelItManager *self){
	toplevel_manager_start(self);
	
	/*ToplevelItWindow *win = toplevelit_window_new();
	self->children = g_list_append(self->children, win); 
	ToplevelItWindow *win2 = toplevelit_window_new();
	self->children = g_list_append(self->children, win2); */
}

ToplevelItManager *toplevelit_manager_new(void){
	return g_object_new(TOPLEVELIT_TYPE_MANAGER, 0);
}

GList* toplevelit_manager_get_windows(ToplevelItManager *self){
	return(self->children);
}

void changed_event(ToplevelItWindow *win, gpointer self){
  	g_print("-%s-", toplevel_window_get_app_id(win));
	g_signal_emit((ToplevelItManager*) self, toplevelit_manager_signals[WINDOW_CHANGED], 0, win);
}

void toplevelit_manager_add_window(ToplevelItManager *self, ToplevelItWindow *win){
	self->children = g_list_append(self->children, win); 
	g_signal_connect(win, "changed", (GCallback) changed_event, self);
}

void toplevelit_manager_remove_window(ToplevelItManager *self, ToplevelItWindow *win){
	self->children = g_list_remove(self->children, win); 
}
