all:
	mkdir -p build/

	gcc $(filter-out src/main.c, $(wildcard src/*.c)) -g -Wall -Wextra -rdynamic -std=gnu2x $(shell pkg-config --libs --cflags gobject-2.0 wayland-client) -shared -fPIC -o build/libtoplevelit.so -lm
	cp src/toplevelit.h build/toplevelit.h
	g-ir-scanner --namespace=ToplevelIt --symbol-prefix=toplevelit --nsversion=1.0 --library=toplevelit --include=GObject-2.0 --library-path=./build  -I./build --no-libtool src/toplevelit.h -o build/ToplevelIt-1.0.gir
	g-ir-compiler build/ToplevelIt-1.0.gir -o build/ToplevelIt-1.0.typelib


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

