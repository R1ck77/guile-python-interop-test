.PHONY: clean all run


CFLAGS=`pkg-config --cflags python3 guile-2.2`
LDFLAGS=`pkg-config --libs python3 guile-2.2`

run: all
	GUILE_LOAD_PATH=$GUILE_LOAD_PATH:. LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. guile test-libpython.scm

all: python-guile.so

python-guile.so: python-guile.c
	gcc -fPIC -shared $(CFLAGS) -o $@ $< $(LDFLAGS) -lpthread


clean:
	rm -f *.o *.so
