#ifndef INTERNAL_H
#define INTERNAL_H

#include "toplevelit_manager.h"
#include "foreign-toplevel.h"

extern int lastWinIDCounter;
extern struct wl_display *wlDisplay;
extern struct wl_seat *wlSeat;
extern struct wl_registry *wlRegistry;
extern struct zwlr_foreign_toplevel_manager_v1 *toplevel_manager;
extern const struct wl_registry_listener wlRegistryListener;
extern ToplevelItManager *tplManager;

G_BEGIN_DECLS
typedef struct _RoundtripSource RoundtripSource;
G_END_DECLS

void toplevel_manager_start(ToplevelItManager *);

void internal_set_state(struct zwlr_foreign_toplevel_handle_v1 *, int);

void internal_set_active(struct zwlr_foreign_toplevel_handle_v1 *);

void internal_close(struct zwlr_foreign_toplevel_handle_v1 *);

gboolean toplevel_manager_runner(void*);


#endif
