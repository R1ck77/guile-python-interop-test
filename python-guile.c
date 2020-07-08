#define Py_LIMITED_API 0x03020000
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "python-guile-utils.h"

#include "auto-wrappers.c"

void export_constants()
{
  scm_c_define("py-file-input", scm_from_signed_integer(Py_file_input));
  scm_c_define("py-eval-input", scm_from_signed_integer(Py_eval_input));
  scm_c_define("py-single-input", scm_from_signed_integer(Py_single_input));
}

void export_types()
{
  PyObject_type = scm_make_foreign_object_type(scm_from_utf8_symbol("PyObject"),
                                               scm_list_1(scm_from_utf8_symbol("pointer")),
                                               finalize_PyObject);
}

void export_functions()
{
#include "auto-define-gsubr.c"
}
                      

void init_python()
{
  init_global_lock();
  export_types();
  export_constants();
  export_functions();
}
