#ifndef INTERNAL_H
#define INTERNAL_H

#include "toplevelit_manager.h"
#include "foreign-toplevel.h"

extern int lastWinIDCounter;

void toplevel_manager_start(ToplevelItManager *);

void internal_set_state(struct zwlr_foreign_toplevel_handle_v1 *, int);

void internal_set_active(struct zwlr_foreign_toplevel_handle_v1 *);

void internal_close(struct zwlr_foreign_toplevel_handle_v1 *);

#endif
