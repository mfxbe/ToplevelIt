all:
	mkdir -p build/

	gcc $(filter-out src/test.c, $(wildcard src/*.c)) -g -Wall -Wextra -rdynamic -std=gnu2x $(shell pkg-config --libs --cflags gobject-2.0 wayland-client) -shared -fPIC -o build/libtoplevelit.so -lm
	cp src/toplevelit.h build/toplevelit.h
	g-ir-scanner --namespace=ToplevelIt --symbol-prefix=toplevelit --nsversion=1.0 --library=toplevelit --include=GObject-2.0 --library-path=./build  -I./build --no-libtool src/toplevelit.h -o build/ToplevelIt-1.0.gir
	g-ir-compiler build/ToplevelIt-1.0.gir -o build/ToplevelIt-1.0.typelib


test:
	mkdir -p build/tests
	gcc src/test.c -g -Wall -Wextra -rdynamic -std=gnu2x $(shell pkg-config --libs --cflags gtk4) -L build/ -ltoplevelit -o build/tests/ToplevelIt
	$(warning WARNING: Testing system not yet fully ready.)
	LD_LIBRARY_PATH=build/ ./build/tests/ToplevelIt

wlscan:
	wayland-scanner private-code data/wlr-foreign-toplevel-management-unstable-v1.xml src/foreign-toplevel.c
	wayland-scanner client-header data/wlr-foreign-toplevel-management-unstable-v1.xml src/foreign-toplevel.h

docs:
	mkdir -p build/docs
	g-ir-doc-tool --language C -o build/docs build/ToplevelIt-1.0.gir
	yelp-build html build/docs -o build/docs

clean:
	rm -rf build
	rm src/foreign-toplevel.c
	rm src/foreign-toplevel.h

run:
	build/ToplevelIt

install:
	cp build/libtoplevelit.so /usr/lib/x86_64-linux-gnu/libtoplevelit.so
	cp build/toplevelit.h /usr/include/toplevelit.h
	cp build/ToplevelIt-1.0.gir /usr/share/gir-1.0/ToplevelIt-1.0.gir
	cp build/ToplevelIt-1.0.typelib /usr/lib/x86_64-linux-gnu/girepository-1.0/ToplevelIt-1.0.typelib
	cp toplevelit.pc  /usr/share/pkgconfig/toplevelit.pc

uninstall:
	rm /usr/lib/x86_64-linux-gnu/libtoplevelit.so
	rm /usr/include/toplevelit.h
	rm /usr/share/gir-1.0/ToplevelIt-1.0.gir
	rm /usr/lib/x86_64-linux-gnu/girepository-1.0/ToplevelIt-1.0.typelib
	rm /usr/share/pkgconfig/toplevelit.pc
