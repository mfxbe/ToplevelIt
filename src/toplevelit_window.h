#ifndef TOPLEVELIT_WINDOW_H
#define TOPLEVELIT_WINDOW_H

#include <glib-object.h>
#include "../build/extra/foreign-toplevel.h"

//some constants
#define TOPLEVELIT_WINDOW_STATUS_DEFAULT 0
#define TOPLEVELIT_WINDOW_STATUS_MINIMIZED 1
#define TOPLEVELIT_WINDOW_STATUS_MAXIMIZED 3
#define TOPLEVELIT_WINDOW_STATUS_FULLSCREEN 4

//Define the toplevel window GObject
G_DECLARE_FINAL_TYPE (ToplevelItWindow, toplevelit_window, TOPLEVELIT, WINDOW, GObject)
#define TOPLEVELIT_TYPE_WINDOW toplevelit_window_get_type()

//Functions
ToplevelItWindow *toplevelit_window_new(void);

gchar *toplevel_window_get_app_id(ToplevelItWindow *);

gchar *toplevel_window_get_icon_name(ToplevelItWindow *);

gchar *toplevel_window_get_status(ToplevelItWindow *);

void toplevel_window_set_state(ToplevelItWindow *, int);

void toplevel_window_set_active(ToplevelItWindow *, gboolean);

//internal functions
void toplevel_window_set_app_id(ToplevelItWindow *, struct zwlr_foreign_toplevel_handle_v1 *, const gchar *);

void toplevel_window_opened(ToplevelItWindow *);

void toplevel_window_closed(ToplevelItWindow *);

void toplevel_window_set_state_only(ToplevelItWindow *, int);

void toplevel_window_set_active_only(ToplevelItWindow *, gboolean);

#endif
