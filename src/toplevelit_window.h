#ifndef TOPLEVELIT_WINDOW_H
#define TOPLEVELIT_WINDOW_H

#include <glib-object.h>
#include "foreign-toplevel.h"
#include "toplevelit.h"

#define TOPLEVELIT_TYPE_WINDOW toplevelit_window_get_type()

// FOR PUBLIC FUNCTIONS AND TYPE DECLARATION SEE toplevelit.h

//internal functions
ToplevelItWindow *toplevelit_window_new(void);

void toplevelit_window_set_app_id(ToplevelItWindow *, struct zwlr_foreign_toplevel_handle_v1 *, const gchar *);

void toplevelit_window_opened(ToplevelItWindow *);

void toplevelit_window_closed(ToplevelItWindow *);

void toplevelit_window_set_state_only(ToplevelItWindow *, int);

void toplevelit_window_set_active_only(ToplevelItWindow *, gboolean);

void toplevelit_window_set_title(ToplevelItWindow *, gchar *);

#endif
