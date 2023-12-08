#include <glib.h>
#include <gdk/wayland/gdkwayland.h>
#include <wayland-client.h>
#include "toplevelit_manager.h"
#include "toplevelit_window.h"
#include "toplevelit.h"
#include "internal.h"
#include "foreign-toplevel.h"

void toplevel_manager_start(ToplevelItManager *d) {
	tplManager = d;
	
	wlDisplay = gdk_wayland_display_get_wl_display(gdk_display_get_default());
	GdkSeat* gdkSeat = gdk_display_get_default_seat(gdk_display_get_default());
	wlSeat = gdk_wayland_seat_get_wl_seat(gdkSeat);
	wlRegistry = wl_display_get_registry(wlDisplay);
	wl_registry_add_listener(wlRegistry, &wlRegistryListener, NULL);
	wl_display_roundtrip(wlDisplay);
}
