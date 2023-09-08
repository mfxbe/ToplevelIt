#include <glib-object.h>
#include "toplevelit_manager.h"
#include "toplevelit_window.h"
#include "internal.h"

//Possible actions ----------------------------------------------------

enum {
	WINDOW_OPENED,
	WINDOW_CLOSED,
	WINDOW_CHANGED,
	LAST_SIGNAL
};
static guint toplevelit_manager_signals[LAST_SIGNAL] = {0};

//GObject stuff ----------------------------------------------------

struct _ToplevelItManager {
	GObject parent_instance;
	GList *children;
};

G_DEFINE_TYPE(ToplevelItManager, toplevelit_manager, G_TYPE_OBJECT)

static void toplevelit_manager_finalize(GObject *obj) {
	ToplevelItManager *self = TOPLEVELIT_MANAGER(obj);

	self->children = g_list_remove_all(self->children, NULL);
	g_free(self->children);

	G_OBJECT_CLASS(toplevelit_manager_parent_class)->finalize(obj);
}

static void toplevelit_manager_class_init(ToplevelItManagerClass *klass) {
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = toplevelit_manager_finalize;

	//Adding signals
	GType arg_types[1] = {TOPLEVELIT_TYPE_WINDOW};
	toplevelit_manager_signals[WINDOW_CHANGED] = g_signal_newv("window-changed",
															   G_TYPE_FROM_CLASS(object_class),
															   G_SIGNAL_RUN_LAST,
															   NULL,
															   NULL,
															   NULL,
															   NULL,
															   G_TYPE_NONE,
															   1,
															   arg_types);
	toplevelit_manager_signals[WINDOW_OPENED] = g_signal_newv("window-opened",
															  G_TYPE_FROM_CLASS(object_class),
															  G_SIGNAL_RUN_LAST,
															  NULL,
															  NULL,
															  NULL,
															  NULL,
															  G_TYPE_NONE,
															  1,
															  arg_types);
	toplevelit_manager_signals[WINDOW_CLOSED] = g_signal_newv("window-closed",
															  G_TYPE_FROM_CLASS(object_class),
															  G_SIGNAL_RUN_LAST,
															  NULL,
															  NULL,
															  NULL,
															  NULL,
															  G_TYPE_NONE,
															  1,
															  arg_types);
}

static void toplevelit_manager_init(ToplevelItManager *self) {
	toplevel_manager_start(self);

	/*ToplevelItWindow *win = toplevelit_window_new();
	self->children = g_list_append(self->children, win);*/
}

//Functions etc. ----------------------------------------------------

/**
 * toplevelit_manager_new:
 *
 * Get a manager for handling hte toplevels. This usually is the first
 * step that needs to be done.
 *
 * Returns: @ToplevelItManager
 **/
ToplevelItManager *toplevelit_manager_new(void) {
	return g_object_new(TOPLEVELIT_TYPE_MANAGER, 0);
}

/**
 * toplevelit_manager_get_windows:
 * @self ToplevelItManager
 *
 * Returns all windows that are currently opened on all views.
 *
 * Returns: @GList
 **/
GList *toplevelit_manager_get_windows(ToplevelItManager *self) {
	return (self->children);
}

//internal functions etc. ----------------------------------------------------

//helper functions to emit the changed signal when any of the windows changed
void ce_changed(ToplevelItWindow *win, gpointer self) {
	g_signal_emit((ToplevelItManager *) self, toplevelit_manager_signals[WINDOW_CHANGED], 0, win);
}

void ce_opened(ToplevelItWindow *win, gpointer self) {
	g_signal_emit((ToplevelItManager *) self, toplevelit_manager_signals[WINDOW_OPENED], 0, win);
}

void ce_closed(ToplevelItWindow *win, gpointer self) {
	g_signal_emit((ToplevelItManager *) self, toplevelit_manager_signals[WINDOW_CLOSED], 0, win);
}

//adding a view to the list and connect signals
void toplevelit_manager_add_window(ToplevelItManager *self, ToplevelItWindow *win) {
	self->children = g_list_append(self->children, win);
	g_signal_connect(win, "changed", (GCallback) ce_changed, self);
	g_signal_connect(win, "opened", (GCallback) ce_opened, self);
	g_signal_connect(win, "closed", (GCallback) ce_closed, self);
}

//internal
void toplevelit_manager_remove_window(ToplevelItManager *self, ToplevelItWindow *win) {
	self->children = g_list_remove(self->children, win);
}
