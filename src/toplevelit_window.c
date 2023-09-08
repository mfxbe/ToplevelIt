#include <glib-object.h>
#include "toplevelit_window.h"
#include "internal.h"

//Possible actions ----------------------------------------------------

enum {
	CHANGED,
	OPENED,
	CLOSED,
	LAST_SIGNAL
};
static guint toplevelit_window_signals[LAST_SIGNAL] = {0};

//GObject stuff ----------------------------------------------------

struct _ToplevelItWindow {
	GObject parent_instance;
	struct zwlr_foreign_toplevel_handle_v1 *toplevel;
	char *app_id;
	char *title;
	int state;
	gboolean active;
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
}

static void toplevelit_window_init(ToplevelItWindow *) {
	//Init
}

//Functions etc. ----------------------------------------------------
/**
 * toplevelit_window_get_app_id:
 * @self ToplevelItManager
 *
 * Returns the app id of the this window
 *
 * Returns: @gchar
 **/
gchar *toplevelit_window_get_app_id(ToplevelItWindow *self) {
	return (self->app_id);
}

/**
 * toplevelit_window_get_app_id:
 * @self ToplevelItManager
 *
 * Returns the app id of the this window
 *
 * Returns: @gchar
 **/
gchar *toplevelit_window_get_title(ToplevelItWindow *self) {
	return (self->title);
}

/**
 * toplevelit_window_get_state:
 * @self ToplevelItManager
 *
 * Returns the state of the this window
 *
 * Returns: @int TOPLEVELIT_WINDOW_STATUS
 **/
int toplevelit_window_get_state(ToplevelItWindow *self) {
	return (self->state);
}

/**
 * toplevelit_window_set_state:
 * @self ToplevelItManager
 * @state TOPLEVELIT_WINDOW_STATUS
 *
 * Set the state of the window. Like minimized, maximized, default.
 *
 **/
void toplevelit_window_set_state(ToplevelItWindow *self, int state) {
    toplevelit_window_set_state_only(self, state);
	internal_set_state(self->toplevel, state);
}

/**
 * toplevelit_window_set_active:
 * @self ToplevelItManager
 * @active gboolean
 *
 * Makes the window the active one.
 *
 **/
void toplevelit_window_set_active(ToplevelItWindow *self, gboolean active) {
    toplevelit_window_set_active_only(self, active);
	if (active == TRUE) {
		internal_set_active(self->toplevel);
	}
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

void toplevelit_window_set_title(ToplevelItWindow *self, gchar *title){
    self->title = title;
}

void toplevelit_window_set_state_only(ToplevelItWindow *self, int state) {
	self->state = state;
	g_signal_emit(self, toplevelit_window_signals[CHANGED], 0);
}

void toplevelit_window_set_active_only(ToplevelItWindow *self, gboolean active) {
	self->active = active;
	g_signal_emit(self, toplevelit_window_signals[CHANGED], 0);
}