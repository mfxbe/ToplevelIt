# ToplevelIT

A C and GObject based library for the wayland foreign-window-protocol. It gives easier access to window controlling and managing. Might be used in panels, docks, etc...

## Build

To build the library run the following commands.

```
make wlscan
make
```

An automatic installation option is not there at the moment.

## Example

A very basic example can be found below. For all of this to work the `.h` and `.so` file need to be in the right place on your system.

example.c:

```
#include "toplevelit_manager.h"
#include <glib-object.h>

void do_something(){
    g_print("A window has opened.");
}


int main(){
    GMainLoop *loop = g_main_loop_new(NULL, TRUE); //only if there isn't already one

    ToplevelItManager *m = toplevelit_manager_new();
    //toplevelit_manager_get_windows(m);

    g_signal_connect(m, "window-opened", G_CALLBACK(do_something), NULL);

    g_main_loop_run(loop);
    return 0;
}
```

Compile and run it with:

```
gcc example.c `pkg-config --libs --cflags gobject-2.0` -ltoplevelit  -o example
./example
```

## Usage
See `toplevelit_manager.h` and `toplevelit_window.h` for what can be done.