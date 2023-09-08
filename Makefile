all:
	mkdir -p build/extra
	
	wayland-scanner private-code data/wlr-foreign-toplevel-management-unstable-v1.xml build/extra/foreign-toplevel.c
	wayland-scanner client-header data/wlr-foreign-toplevel-management-unstable-v1.xml build/extra/foreign-toplevel.h
	
	gcc $(wildcard src/*.c) build/extra/foreign-toplevel.c -g -Wall -Wextra -rdynamic -std=gnu2x $(shell pkg-config --libs --cflags gobject-2.0 wayland-client) -o build/ToplevelIt

lib:
	mkdir -p build/extra
	mkdir -p build/result

	wayland-scanner private-code data/wlr-foreign-toplevel-management-unstable-v1.xml build/extra/foreign-toplevel.c
	wayland-scanner client-header data/wlr-foreign-toplevel-management-unstable-v1.xml build/extra/foreign-toplevel.h

	gcc $(filter-out src/main.c, $(wildcard src/*.c)) build/extra/foreign-toplevel.c -g -Wall -Wextra -rdynamic -std=gnu2x $(shell pkg-config --libs --cflags gobject-2.0 wayland-client) -shared -fPIC -o build/result/toplevelit.so -lm
	cp src/toplevelit_manager.h build/result/toplevel_manager.h
	cp src/toplevelit_window.h build/result/toplevel_window.h



clean:
	rm -rf build
	
run:
	build/ToplevelIt

