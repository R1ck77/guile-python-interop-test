#define Py_LIMITED_API 0x03020000
#include <Python.h>
#include <libguile.h>
#include <stdlib.h>
#include <stdio.h>

static SCM initialize_python_wrapper(SCM unused) {
  Py_Initialize();
  return SCM_UNSPECIFIED;
}

static SCM finalize_python_wrapper(SCM unused) {
  Py_Finalize();
  return SCM_UNSPECIFIED;
}

void init_python()
{
  scm_c_define_gsubr("py-initialize", 0, 0, 0, initialize_python_wrapper);
  scm_c_define_gsubr("py-finalize", 0, 0, 0, initialize_python_wrapper);
}
