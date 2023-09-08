all:
	mkdir -p build/

	gcc $(filter-out src/main.c, $(wildcard src/*.c)) -g -Wall -Wextra -rdynamic -std=gnu2x $(shell pkg-config --libs --cflags gobject-2.0 wayland-client) -shared -fPIC -o build/libtoplevelit.so -lm
	cp src/toplevelit_manager.h build/toplevelit_manager.h
	cp src/toplevelit_window.h build/toplevelit_window.h
	cp src/foreign-toplevel.h build/foreign-toplevel.h

test:
	mkdir -p build/tests
	gcc $(wildcard src/*.c) -g -Wall -Wextra -rdynamic -std=gnu2x $(shell pkg-config --libs --cflags gobject-2.0 wayland-client) -o build/tests/ToplevelIt
	$(warning WARNING: Testing system not yet ready)

wlscan:
	wayland-scanner private-code data/wlr-foreign-toplevel-management-unstable-v1.xml src/foreign-toplevel.c
	wayland-scanner client-header data/wlr-foreign-toplevel-management-unstable-v1.xml src/foreign-toplevel.h

clean:
	rm -rf build
	rm src/foreign-toplevel.c
	rm src/foreign-toplevel.h

run:
	build/ToplevelIt

