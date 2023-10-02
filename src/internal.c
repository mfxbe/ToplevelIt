#include <glib.h>
#include <wayland-client.h>
#include "toplevelit_manager.h"
#include "toplevelit_window.h"
#include "toplevelit.h"
#include "internal.h"
#include "foreign-toplevel.h"

struct wl_display *wlDisplay = NULL;
struct wl_seat *wlSeat;
struct wl_registry *wlRegistry;
static struct zwlr_foreign_toplevel_manager_v1 *toplevel_manager;
ToplevelItManager *tplManager;
int lastWinIDCounter;

void internal_set_state(struct zwlr_foreign_toplevel_handle_v1 *toplevel, int state) {
	switch (state) {
		case TOPLEVELIT_WINDOW_STATUS_MINIMIZED:
			zwlr_foreign_toplevel_handle_v1_set_minimized(toplevel);
			break;
		case TOPLEVELIT_WINDOW_STATUS_MAXIMIZED:
			zwlr_foreign_toplevel_handle_v1_set_maximized(toplevel);
			break;
		case TOPLEVELIT_WINDOW_STATUS_FULLSCREEN:
			zwlr_foreign_toplevel_handle_v1_set_fullscreen(toplevel, NULL);
			break;
		case TOPLEVELIT_WINDOW_STATUS_DEFAULT:
		default:
			zwlr_foreign_toplevel_handle_v1_unset_minimized(toplevel);
			zwlr_foreign_toplevel_handle_v1_unset_maximized(toplevel);
			zwlr_foreign_toplevel_handle_v1_unset_fullscreen(toplevel);
			break;
	}
}

void internal_set_active(struct zwlr_foreign_toplevel_handle_v1 *toplevel) {
	if (wlSeat != NULL) {
		zwlr_foreign_toplevel_handle_v1_activate(toplevel, wlSeat);
	}
}

void internal_close(struct zwlr_foreign_toplevel_handle_v1 *toplevel){
	zwlr_foreign_toplevel_handle_v1_close(toplevel);
}

//there are some unimportant signals which must be connected this function is used for those
static void not_care() {}

//handle closing of toplevel
static void z_toplevel_handle_close(void *data, struct zwlr_foreign_toplevel_handle_v1 *) {
	ToplevelItWindow *win = (ToplevelItWindow *) data;
    toplevelit_window_closed(win);
	toplevelit_manager_remove_window(tplManager, win);
}

//handling a newly opened toplevel
static void z_toplevel_handle_app(void *data, struct zwlr_foreign_toplevel_handle_v1 *toplevel, const char *id) {
	ToplevelItWindow *win = (ToplevelItWindow *) data;
    toplevelit_window_set_app_id(win, toplevel, id);
    toplevelit_window_opened(win);
}

//handle state change of window
static void z_toplevel_handle_state(void *data, struct zwlr_foreign_toplevel_handle_v1 *, struct wl_array *state) {
	ToplevelItWindow *win = (ToplevelItWindow *) data;

	uint32_t *entry;
	gboolean isActive = FALSE;
	wl_array_for_each(entry, state) {
		switch (*entry) {
			case ZWLR_FOREIGN_TOPLEVEL_HANDLE_V1_STATE_ACTIVATED:
				isActive = TRUE;
				toplevelit_window_set_active(win, TRUE);
				break;
			case ZWLR_FOREIGN_TOPLEVEL_HANDLE_V1_STATE_FULLSCREEN:
                toplevelit_window_set_state(win, TOPLEVELIT_WINDOW_STATUS_FULLSCREEN);
				break;
			case ZWLR_FOREIGN_TOPLEVEL_HANDLE_V1_STATE_MAXIMIZED:
                toplevelit_window_set_state(win, TOPLEVELIT_WINDOW_STATUS_MAXIMIZED);
				break;
			case ZWLR_FOREIGN_TOPLEVEL_HANDLE_V1_STATE_MINIMIZED:
                toplevelit_window_set_state(win, TOPLEVELIT_WINDOW_STATUS_MINIMIZED);
				break;
			default:
                toplevelit_window_set_state(win, TOPLEVELIT_WINDOW_STATUS_DEFAULT);
				break;
		}
		if (isActive == FALSE) {
			toplevelit_window_set_active(win, FALSE);
		}
	}
}

void z_toplevel_handle_title(void *data, struct zwlr_foreign_toplevel_handle_v1 *, const char *title) {
    ToplevelItWindow *win = (ToplevelItWindow *) data;
    toplevelit_window_set_title(win, g_strdup_printf(title));
}

void z_toplevel_handle_output_enter(void *, struct zwlr_foreign_toplevel_handle_v1 *, struct wl_output *) {}

void z_toplevel_handle_output_leave(void *, struct zwlr_foreign_toplevel_handle_v1 *, struct wl_output *) {}

void z_toplevel_handle_done(void *, struct zwlr_foreign_toplevel_handle_v1 *) {}

void
z_toplevel_handle_parent(void *, struct zwlr_foreign_toplevel_handle_v1 *, struct zwlr_foreign_toplevel_handle_v1 *) {}

//struct for ft_handle_toplevel (possible actions of/on windows)
static const struct zwlr_foreign_toplevel_handle_v1_listener z_toplevel_listener = {
		.title = z_toplevel_handle_title,
		.app_id = z_toplevel_handle_app,
		.output_enter = z_toplevel_handle_output_enter,
		.output_leave = z_toplevel_handle_output_leave,
		.done =  z_toplevel_handle_done,
		.state = z_toplevel_handle_state,
		.closed = z_toplevel_handle_close,
		.parent = z_toplevel_handle_parent
};

//handle if new toplevel is opened
static void toplevel_manager_handle_toplevel(void *, struct zwlr_foreign_toplevel_manager_v1 *,
											 struct zwlr_foreign_toplevel_handle_v1 *ztoplevel) {
	ToplevelItWindow *win = toplevelit_window_new();
	toplevelit_manager_add_window(tplManager, win);
	void *data = (void *) win;
	zwlr_foreign_toplevel_handle_v1_add_listener(ztoplevel, &z_toplevel_listener, data);
}

//struct
static const struct zwlr_foreign_toplevel_manager_v1_listener toplevel_listener = {
		.toplevel = toplevel_manager_handle_toplevel,
		.finished = (void (*)(void *, struct zwlr_foreign_toplevel_manager_v1 *)) not_care,
};

//connecting from global wayland listener to foreign toplevel listener
struct wl_seat *seat = NULL;

static void wl_registry_handle_global(void *, struct wl_registry *wlRegistryL, uint32_t id, const char *interface,
									  uint32_t version) {
	if (strcmp(interface, zwlr_foreign_toplevel_manager_v1_interface.name) == 0) {
		toplevel_manager = wl_registry_bind(wlRegistryL, id, &zwlr_foreign_toplevel_manager_v1_interface, version);
		zwlr_foreign_toplevel_manager_v1_add_listener(toplevel_manager, &toplevel_listener, NULL);
	} else if (strcmp(interface, wl_seat_interface.name) == 0 && seat == NULL) {
		seat = wl_registry_bind(wlRegistryL, id, &wl_seat_interface, version);
	}
}

//struct for toplevel_manager_start/global listener
static const struct wl_registry_listener wlRegistryListener = {
		.global = wl_registry_handle_global,
		.global_remove = (void (*)(void *, struct wl_registry *, uint32_t)) not_care,
};

int toplevel_manager_runner() {
	wl_display_roundtrip(wlDisplay);
	wl_display_flush(wlDisplay);
	return (TRUE);
}

void toplevel_manager_start(ToplevelItManager *d) {
	tplManager = d;

	wlDisplay = wl_display_connect(NULL);
	wlRegistry = wl_display_get_registry(wlDisplay);
	wl_registry_add_listener(wlRegistry, &wlRegistryListener, NULL);
	wl_display_roundtrip(wlDisplay);
	wl_display_flush(wlDisplay);

	g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, G_SOURCE_FUNC(toplevel_manager_runner), NULL, NULL);
}
