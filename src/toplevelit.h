#ifndef TOPLEVELIT_TOPLEVELIT_H
#define TOPLEVELIT_TOPLEVELIT_H

//some constants
#define TOPLEVELIT_WINDOW_STATUS_DEFAULT 0
#define TOPLEVELIT_WINDOW_STATUS_MINIMIZED 1
#define TOPLEVELIT_WINDOW_STATUS_MAXIMIZED 3
#define TOPLEVELIT_WINDOW_STATUS_FULLSCREEN 4

//Define our main TopLevelItManager GObject
G_DECLARE_FINAL_TYPE (ToplevelItManager, toplevelit_manager, TOPLEVELIT, MANAGER, GObject)
#define TOPLEVELIT_TYPE_MANAGER toplevelit_manager_get_type()

//Define the toplevel window GObject
G_DECLARE_FINAL_TYPE (ToplevelItWindow, toplevelit_window, TOPLEVELIT, WINDOW, GObject)


//Public functions of Manager (internal see toplevelit_manager.h)
ToplevelItManager *toplevelit_manager_new(void);

GList *toplevelit_manager_get_windows(ToplevelItManager *);

//Public functions of Window (internal see toplevelit_window.h)
ToplevelItWindow *toplevelit_window_new(void);

gchar *toplevelit_window_get_app_id(ToplevelItWindow *);

int toplevelit_window_get_win_id(ToplevelItWindow *);

gchar *toplevelit_window_get_icon_name(ToplevelItWindow *);

int toplevelit_window_get_state(ToplevelItWindow *);

gchar *toplevelit_window_get_title(ToplevelItWindow *);

gboolean toplevelit_window_get_active(ToplevelItWindow *);

void toplevelit_window_set_state(ToplevelItWindow *, int);

void toplevelit_window_set_active(ToplevelItWindow *, gboolean);

#endif //TOPLEVELIT_TOPLEVELIT_H
