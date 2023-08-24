#ifndef INTERNAL_H
#define INTERNAL_H

#include "toplevelit_manager.h"
#include "../build/extra/foreign-toplevel.h"

void toplevel_manager_start(ToplevelItManager*);

void internal_set_state(struct zwlr_foreign_toplevel_handle_v1*, int);
void internal_set_active(struct zwlr_foreign_toplevel_handle_v1*, gboolean);

#endif
