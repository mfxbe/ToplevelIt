#include <glib-object.h>
#include "toplevelit_window.h"
#include "internal.h"
#include "../build/extra/foreign-toplevel.h"

//Possible actions ----------------------------------------------------

enum {
  CHANGED,
  OPENED,
  CLOSED,
  LAST_SIGNAL
};
static guint toplevelit_window_signals[LAST_SIGNAL] = { 0 };

//GObject stuff ----------------------------------------------------

struct _ToplevelItWindow{
    GObject parent_instance;
	struct zwlr_foreign_toplevel_handle_v1* toplevel;
    char* app_id;
    int status;
    gboolean active;
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
														G_SIGNAL_RUN_LAST,
														NULL,
														NULL,
														NULL,
														NULL,
														G_TYPE_NONE,
														0,
														NULL);
	toplevelit_window_signals[OPENED] = g_signal_newv("opened",
														G_TYPE_FROM_CLASS(object_class), 
														G_SIGNAL_RUN_LAST,
														NULL,
														NULL,
														NULL,
														NULL,
														G_TYPE_NONE,
														0,
														NULL);
	toplevelit_window_signals[CLOSED] = g_signal_newv("closed",
														G_TYPE_FROM_CLASS(object_class), 
														G_SIGNAL_RUN_LAST,
														NULL,
														NULL,
														NULL,
														NULL,
														G_TYPE_NONE,
														0,
														NULL);
}

static void toplevelit_window_init(ToplevelItWindow*){
	//Init
}

//Functions etc. ----------------------------------------------------
gchar *toplevel_window_get_app_id(ToplevelItWindow *self){
		return(self->app_id);
}

void toplevel_window_set_state(ToplevelItWindow *self, int state){
	toplevel_window_set_state_only(self, state);
	internal_set_state(self->toplevel, state);
}
void toplevel_window_set_active(ToplevelItWindow *self, gboolean active){
	toplevel_window_set_active_only(self, active);
	if(active == TRUE){
		internal_set_active(self->toplevel);
	}
}

//internal functions etc. ----------------------------------------------------
ToplevelItWindow *toplevelit_window_new(void){
	return g_object_new(TOPLEVELIT_TYPE_WINDOW, 0);
}

void toplevel_window_closed(ToplevelItWindow *win){
	g_signal_emit(win, toplevelit_window_signals[CHANGED], 0);
}

void toplevel_window_opened(ToplevelItWindow *win){
	g_signal_emit(win, toplevelit_window_signals[CHANGED], 0);
}

//Setting the app id, as this is the first thing of a new toplevel we also do some other stuff here as well
void toplevel_window_set_app_id(ToplevelItWindow *self, struct zwlr_foreign_toplevel_handle_v1 *toplevel, const gchar* data){
	if(self->app_id == NULL){
		self->app_id = g_strdup_printf("%s", data);
		self->toplevel = toplevel;
	}
}

void toplevel_window_set_state_only(ToplevelItWindow *self, int state){
	self->status = state;
}
void toplevel_window_set_active_only(ToplevelItWindow *self, gboolean active){
	self->active = active;
}