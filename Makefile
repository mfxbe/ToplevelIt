all:
	mkdir -p build/extra
	
	wayland-scanner private-code data/wlr-foreign-toplevel-management-unstable-v1.xml build/extra/foreign-toplevel.c
	wayland-scanner client-header data/wlr-foreign-toplevel-management-unstable-v1.xml build/extra/foreign-toplevel.h
	
	gcc $(wildcard src/*.c) build/extra/foreign-toplevel.c -g -Wall -Wextra -rdynamic -std=gnu2x $(shell pkg-config --libs --cflags gobject-2.0 wayland-client) -o build/ToplevelIt
	
clean:
	rm -rf build
	
run:
	build/ToplevelIt

