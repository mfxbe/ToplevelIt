#ifndef TOPLEVELIT_H
#define TOPLEVELIT_H
#include <glib-object.h>
#include "toplevelit_window.h"

//Define our main GObject
G_DECLARE_FINAL_TYPE (ToplevelItManager, toplevelit_manager, TOPLEVELIT, MANAGER, GObject)
#define TOPLEVELIT_TYPE_MANAGER toplevelit_manager_get_type()

//Functions
ToplevelItManager *toplevelit_manager_new (void);
GList* toplevelit_manager_get_windows(ToplevelItManager*);

//internal functions
void toplevelit_manager_add_window(ToplevelItManager*, ToplevelItWindow*);
void toplevelit_manager_remove_window(ToplevelItManager*, ToplevelItWindow*);

#endif
