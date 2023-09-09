#ifndef TOPLEVELIT_WINDOW_H
#define TOPLEVELIT_WINDOW_H

#include <glib-object.h>
#include "foreign-toplevel.h"

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

gchar *toplevelit_window_get_app_id(ToplevelItWindow *);

int toplevelit_window_get_win_id(ToplevelItWindow *);

gchar *toplevelit_window_get_icon_name(ToplevelItWindow *);

int toplevelit_window_get_state(ToplevelItWindow *);

gchar *toplevelit_window_get_title(ToplevelItWindow*);

gboolean toplevelit_window_get_active(ToplevelItWindow*);

void toplevelit_window_set_state(ToplevelItWindow *, int);

void toplevelit_window_set_active(ToplevelItWindow *, gboolean);

//internal functions
void toplevelit_window_set_app_id(ToplevelItWindow *, struct zwlr_foreign_toplevel_handle_v1 *, const gchar *);

void toplevelit_window_opened(ToplevelItWindow *);

void toplevelit_window_closed(ToplevelItWindow *);

void toplevelit_window_set_state_only(ToplevelItWindow *, int);

void toplevelit_window_set_active_only(ToplevelItWindow *, gboolean);

void toplevelit_window_set_title(ToplevelItWindow *, gchar*);

#endif
