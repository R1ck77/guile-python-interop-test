.PHONY: clean all run


CFLAGS=`pkg-config --cflags python3 guile-2.2`
LDFLAGS=`pkg-config --libs python3 guile-2.2`

run: all
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. guile test-libpython.scm

all: libpython-guile.so

libpython-guile.so: python-guile.c
	gcc -fPIC -shared $(CFLAGS) -o $@ $< $(LDFLAGS)


clean:
	rm -f *.o *.so
