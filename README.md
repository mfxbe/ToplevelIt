# ToplevelIT

ToplevelIt is a small library, built with C and GObject, that makes working with Wayland's foreign-toplevel-protocol easy. It simplifies tasks like controlling and managing windows, making it a great fit for projects like panels, docks, and more.

You can use ToplevelIt in C, and it should also play nicely with any programming language that supports GObject introspection. Whether you prefer C  or another language, ToplevelIt is here to simplify your interactions with Wayland's foreign-toplevel-protocol.
## Build

To build the library, run the following commands.

```
make wlscan
make
```

You can also install the library system-wide.

```
sudo make install
```

## Example

A very basic example can be found below. For all of this to work, the `.h` and `.so` files need to be in the right place on your system.

example.c:

```
#include "toplevelit.h"
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

Compile and run it with these commands.

```
gcc example.c `pkg-config --libs --cflags gobject-2.0` -ltoplevelit  -o example
./example
```

## Usage
See [`toplevelit.h`](https://www.feritale.eu/git/mfxbe/ToplevelIt/src/branch/master/src/toplevelit.h) or the [documentation](https://www.feritale.eu/docs/toplevelit/) for what can be done.

## FAQ

**Why does this have significantly fewer features than, for instance, WNCK?**    
ToplevelIt is currently only using one wayland protocol to have as few dependencies as possible. This might or might not change in future.

**Where can I get help?**    
If you found any errors, feel free to create an issue. Other than that, you are welcome to contact me in the [Feritale Matrix](https://matrix.to/#/#feritale:tchncs.de) chat.