#include <glib-object.h>
#include "toplevelit_window.h"
#include "internal.h"

//Possible actions ----------------------------------------------------

enum {
	CHANGED,
	OPENED,
	CLOSED,
	TITLE_CHANGE,
	LAST_SIGNAL
};
static guint toplevelit_window_signals[LAST_SIGNAL] = {0};

//GObject stuff ----------------------------------------------------

struct _ToplevelItWindow {
	GObject parent_instance;
	ToplevelItManager *manager;
	struct zwlr_foreign_toplevel_handle_v1 *toplevel;
	char *app_id;
	char *title;
	int state;
	int win_id;
	gboolean active;
	void *data;
};

G_DEFINE_TYPE(ToplevelItWindow, toplevelit_window, G_TYPE_OBJECT)

static void toplevelit_window_finalize(GObject *obj) {
	//ToplevelItWindow *self = TOPLEVELIT_WINDOW(obj);
	G_OBJECT_CLASS(toplevelit_window_parent_class)->finalize(obj);
}

static void toplevelit_window_class_init(ToplevelItWindowClass *klass) {
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
	toplevelit_window_signals[TITLE_CHANGE] = g_signal_newv("title-change",
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

static void toplevelit_window_init(ToplevelItWindow *self) {
	//Init
	self->win_id = lastWinIDCounter;
	lastWinIDCounter++;
}

//Functions etc. ----------------------------------------------------
gchar *toplevelit_window_get_app_id(ToplevelItWindow *self) {
	return (g_strdup_printf(self->app_id));
}

int toplevelit_window_get_win_id(ToplevelItWindow *self) {
	return (self->win_id);
}

gchar *toplevelit_window_get_title(ToplevelItWindow *self) {
	return (g_strdup_printf(self->title));
}

gboolean toplevelit_window_get_active(ToplevelItWindow *self) {
	return (self->active);
}

int toplevelit_window_get_state(ToplevelItWindow *self) {
	return (self->state);
}

void toplevelit_window_set_state(ToplevelItWindow *self, int state) {
	if(state == TOPLEVELIT_WINDOW_STATUS_MINIMIZED){
		toplevelit_window_set_active(self, FALSE);
	}
	toplevelit_window_set_state_only(self, state);
	internal_set_state(self->toplevel, state);
}

void toplevelit_window_set_active(ToplevelItWindow *self, gboolean active) {
	if (active == TRUE) {
		internal_set_active(self->toplevel);
		
		//Set all to not active
		GList *l = toplevelit_manager_get_windows(toplevelit_window_get_manager(self));
		GList *e;
		ToplevelItWindow *w;
		for(e = l; e; e = e->next) {
			w = e->data;
			toplevelit_window_set_active_only(w, FALSE);
		}
	}
	toplevelit_window_set_active_only(self, active);
}

void toplevelit_window_close(ToplevelItWindow *self){
	internal_close(self->toplevel);
}

void toplevelit_window_set_data(ToplevelItWindow *self, void *data){
	self->data = data;
}

void *toplevelit_window_get_data(ToplevelItWindow *self){
	return(self->data);
}

//internal functions etc. ----------------------------------------------------
ToplevelItWindow *toplevelit_window_new(void) {
	return g_object_new(TOPLEVELIT_TYPE_WINDOW, 0);
}

void toplevelit_window_closed(ToplevelItWindow *win) {
	g_signal_emit(win, toplevelit_window_signals[CLOSED], 0);
}

void toplevelit_window_opened(ToplevelItWindow *win) {
	g_signal_emit(win, toplevelit_window_signals[OPENED], 0);
}

//Setting the app id, as this is the first thing of a new toplevel we also do some other stuff here as well
void toplevelit_window_set_app_id(ToplevelItWindow *self,
								  struct zwlr_foreign_toplevel_handle_v1 *toplevel,
								  const gchar *data) {
	if (self->app_id == NULL) {
		self->app_id = g_strdup_printf("%s", data);
		self->toplevel = toplevel;
	}
}

void toplevelit_window_set_title(ToplevelItWindow *self, gchar *title) {
	self->title = title;
	g_signal_emit(self, toplevelit_window_signals[TITLE_CHANGE], 0);
}

void toplevelit_window_set_state_only(ToplevelItWindow *self, int state) {
	self->state = state;
	g_signal_emit(self, toplevelit_window_signals[CHANGED], 0);
}

void toplevelit_window_set_active_only(ToplevelItWindow *self, gboolean active) {
	self->active = active;
	g_signal_emit(self, toplevelit_window_signals[CHANGED], 0);
}

void toplevelit_window_set_manager(ToplevelItWindow *self, ToplevelItManager *mng){
	self->manager = mng;
}

ToplevelItManager *toplevelit_window_get_manager(ToplevelItWindow *self){
	return self->manager;
}