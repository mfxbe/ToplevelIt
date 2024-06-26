#ifndef TOPLEVELIT_TOPLEVELIT_H
#define TOPLEVELIT_TOPLEVELIT_H

//includes
#include <glib-object.h>

G_BEGIN_DECLS

//some constants
/**
 * TOPLEVELIT_WINDOW_STATUS_DEFAULT: (value 0)
 * The default state for toplevel windows.
 */
#define TOPLEVELIT_WINDOW_STATUS_DEFAULT 0

/**
 * TOPLEVELIT_WINDOW_STATUS_MINIMIZED: (value 1)
 * The minimized state for toplevel windows (e.g. only visible in taskbar).
 */
#define TOPLEVELIT_WINDOW_STATUS_MINIMIZED 1

/**
 * TOPLEVELIT_WINDOW_STATUS_MAXIMIZED: (value 3)
 * The maximized state for toplevel windows. Uses all space but not the full screen.
 */
#define TOPLEVELIT_WINDOW_STATUS_MAXIMIZED 3

/**
 * TOPLEVELIT_WINDOW_STATUS_FULLSCREEN: (value 4)
 * The fullscreen state for toplevel windows.
 */
#define TOPLEVELIT_WINDOW_STATUS_FULLSCREEN 4

//Define our main TopLevelItManager GObject
G_DECLARE_FINAL_TYPE (ToplevelItManager, toplevelit_manager, TOPLEVELIT, MANAGER, GObject)
#define TOPLEVELIT_TYPE_MANAGER toplevelit_manager_get_type()

//Define the toplevel window GObject
G_DECLARE_FINAL_TYPE (ToplevelItWindow, toplevelit_window, TOPLEVELIT, WINDOW, GObject)
#define TOPLEVELIT_TYPE_WINDOW toplevelit_window_get_type()

//Public functions of Manager (internal see toplevelit_manager.h) -----------------

/**
 * toplevelit_manager_new:(constructor):
 *
 * Create a manager for handling the toplevels. This usually is the first
 * step that needs to be done.
 *
 * Returns: (transfer full): #ToplevelItManager
 **/
ToplevelItManager *toplevelit_manager_new(void);

/**
 * toplevelit_manager_get_windows:
 * @self A #ToplevelItManager
 *
 * Returns all windows that are currently opened on all views.
 *
 * Returns: #GList of #ToplevelItWindow
 **/
GList *toplevelit_manager_get_windows(ToplevelItManager *self);

/**
* toplevelit_manager_do_trip:
 * @self A #ToplevelItManager
 *
 * Make the wl_roundtrip manually. Most of the times this is not needed ToplevelIt does the roundtrip regularly.
 *
 **/
void toplevelit_manager_do_trip(ToplevelItManager *self);

//Public functions of Window (internal see toplevelit_window.h) -----------------

/**
 * toplevelit_window_get_app_id:
 * @self ToplevelItManager
 *
 * Returns the app id of the this window
 *
 * Returns: Application ID as GChar
 **/
gchar *toplevelit_window_get_app_id(ToplevelItWindow *self);

/**
 * toplevelit_window_get_win_id:
 * @self ToplevelItManager
 *
 * Returns the app win of the this window. This is not per application.
 *
 * Returns: Window ID as Int
 **/
int toplevelit_window_get_win_id(ToplevelItWindow *self);

/**
 * toplevelit_window_get_state:
 * @self ToplevelItManager
 *
 * Returns the state.
 *
 * Returns: int
 **/
int toplevelit_window_get_state(ToplevelItWindow *self);

/**
 * toplevelit_window_get_title:
 * @self ToplevelItManager
 *
 * Returns the title of the window
 *
 * Returns: (transfer full): gchar
 **/
gchar *toplevelit_window_get_title(ToplevelItWindow *self);

/**
 * toplevelit_window_get_active:
 * @self ToplevelItWindow
 *
 * Returns if win is active
 *
 * Returns: TRUE or FALSE
 **/
gboolean toplevelit_window_get_active(ToplevelItWindow *self);

/**
 * toplevelit_window_set_state:
 * @self ToplevelItWindow
 * @state TOPLEVELIT_WINDOW_STATUS
 *
 * Change the status of a window
 *
 **/
void toplevelit_window_set_state(ToplevelItWindow *self, int state);

/**
 * toplevelit_window_set_active:
 * @self ToplevelItWindow
 * @active TRUE or FALSE
 *
 * Set if win is active
 *
 **/
void toplevelit_window_set_active(ToplevelItWindow *self, gboolean active);

/**
 * toplevelit_window_close:
 * @self ToplevelItWindow
 *
 * Ask the window to close itself.
 *
 **/
void toplevelit_window_close(ToplevelItWindow *self);

/**
 * toplevelit_window_set_data:
 * @self ToplevelItWindow
 * @data Pointer to data
 *
 * Allows to add custom data to the window object.
 *
 **/
void toplevelit_window_set_data(ToplevelItWindow *self, void *data);

/**
 * toplevelit_window_get_data:
 * @self ToplevelItWindow
 *
 * Allows to get stored custom data from the window object.
 *
 **/
void *toplevelit_window_get_data(ToplevelItWindow *self);

G_END_DECLS

#endif //TOPLEVELIT_TOPLEVELIT_H
