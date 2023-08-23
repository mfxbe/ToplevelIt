#include <glib.h>
#include <wayland-client.h>
#include "toplevelit.h"
#include "toplevelit_window.h"
#include "internal.h"
#include "../build/extra/foreign-toplevel.h"

struct wl_display *wlDisplay = NULL;
struct wl_seat* wlSeat;
struct wl_registry* wlRegistry;
static struct zwlr_foreign_toplevel_manager_v1 *toplevel_manager;
ToplevelItManager* tplManager;

//there are some unimportant signals which must be connected this function is used for those
static void not_care(){/*nobody cares*/};

//handle closing of toplevel
static void z_toplevel_handle_close(void* data, struct zwlr_foreign_toplevel_handle_v1*){
	ToplevelItWindow* win = (ToplevelItWindow*) data;
	toplevel_window_closed(win);
	toplevelit_manager_remove_window(tplManager, win);
}

//handling a newly opend toplevel
static void z_toplevel_handle_app(void* data, struct zwlr_foreign_toplevel_handle_v1*, const char *id){
	ToplevelItWindow* win = (ToplevelItWindow*) data;
	toplevel_window_set_app_id(win, id);
	toplevel_window_opened(win);
}

//struct for ft_handle_toplevel (possible actions of/on windows)
static const struct zwlr_foreign_toplevel_handle_v1_listener z_toplevel_listener = {
	.title = not_care,
	.app_id = z_toplevel_handle_app,
	.output_enter = not_care,
	.output_leave = not_care,
	.done = not_care,
	.state = not_care,
	.closed = z_toplevel_handle_close,
	.parent = not_care
};

//handle if new toplevel is opend
static void toplevel_manager_handle_toplevel(void*, struct zwlr_foreign_toplevel_manager_v1*,  struct zwlr_foreign_toplevel_handle_v1 *ztoplevel){
	ToplevelItWindow *win = toplevelit_window_new();
	toplevelit_manager_add_window(tplManager, win);
	void* data = (void *) win;
	zwlr_foreign_toplevel_handle_v1_add_listener(ztoplevel, &z_toplevel_listener, data);
}

//struct
static const struct zwlr_foreign_toplevel_manager_v1_listener toplevel_listener = {
	.toplevel = toplevel_manager_handle_toplevel,
	.finished = not_care,
};

//connecting from global wayland listener to foreign toplevel listener
static void wl_registry_handle_global(void*, struct wl_registry *wlRegistry, uint32_t id, const char *interface, uint32_t version){
	if (strcmp (interface, zwlr_foreign_toplevel_manager_v1_interface.name) == 0){
		toplevel_manager = wl_registry_bind(wlRegistry, id, &zwlr_foreign_toplevel_manager_v1_interface, version);
		zwlr_foreign_toplevel_manager_v1_add_listener(toplevel_manager, &toplevel_listener, NULL);
	}
}

//struct for toplevel_manager_start/global listener
static const struct wl_registry_listener wlRegistryListener = {
    .global = wl_registry_handle_global,
    .global_remove = not_care,
};

int toplevel_manager_runner(){
	wl_display_roundtrip(wlDisplay);
	wl_display_flush(wlDisplay);
	return(TRUE);
}

void toplevel_manager_start(ToplevelItManager* d){
	tplManager = d;
	
	wlDisplay = wl_display_connect(NULL);
	wlRegistry = wl_display_get_registry(wlDisplay);
	wl_registry_add_listener(wlRegistry, &wlRegistryListener, NULL);
	wl_display_roundtrip(wlDisplay);
	wl_display_flush(wlDisplay);
	
	g_idle_add_full(G_PRIORITY_HIGH_IDLE, G_SOURCE_FUNC(toplevel_manager_runner), NULL, NULL);
}
