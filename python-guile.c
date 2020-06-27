#include <Python.h>
#include <libguile.h>

static void initialize_python() {
  Py_Initialize();
}

static void finalize_python() {
  Py_Finalize();
}
