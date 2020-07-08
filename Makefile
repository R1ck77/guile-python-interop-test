.PHONY: clean all run

CFLAGS=`pkg-config --cflags python3 guile-2.2`
PYTHON_EMBED_PRESENT=$(shell pkg-config --exists python3-embed;echo $$?)
ifeq ($(PYTHON_EMBED_PRESENT), 0)
LDFLAGS=`pkg-config --libs python3-embed guile-2.2`
else
LDFLAGS=`pkg-config --libs python3 guile-2.2`
endif

STATIC_SOURCES=python-guile.c python-guile-utils.c pyobject-data.c python-locking.c
GENERATED_SOURCES=automatically-generated.c
ALL_SOURCES=$(STATIC_SOURCES) $(GENERATED_SOURCES)

run: all
	GUILE_LOAD_PATH=$GUILE_LOAD_PATH:. LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. guile test-libpython.scm

all: python-guile.so

python-guile.so: $(STATIC_SOURCES) $(GENERATED_SOURCES)
	gcc -fPIC -shared $(CFLAGS) -o $@ $(STATIC_SOURCES) $(LDFLAGS) -lpthread

automatically-generated.c: template.scm meta-create-c-binding.scm
	guile meta-create-c-binding.scm $< >$@

clean:
	rm -f *.o *.so $(GENERATED_SOURCES)
