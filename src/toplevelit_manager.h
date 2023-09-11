#ifndef TOPLEVELIT_H
#define TOPLEVELIT_H

#include <glib-object.h>
#include "toplevelit_window.h"
#include "toplevelit.h"

// FOR PUBLIC FUNCTIONS AND TYPE DECLARATION SEE toplevelit.h

//internal functions
void toplevelit_manager_add_window(ToplevelItManager *, ToplevelItWindow *);

void toplevelit_manager_remove_window(ToplevelItManager *, ToplevelItWindow *);

#endif
