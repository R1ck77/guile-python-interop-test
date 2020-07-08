#define Py_LIMITED_API 0x03020000
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "python-guile-utils.h"

#include "automatically-generated.c"

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
  scm_c_define_gsubr("py-initialize", 0, 0, 0, Py_Initialize_wrapper);
  scm_c_define_gsubr("pylong-from-long", 1, 0, 0, PyLong_FromLongLong_wrapper);
  scm_c_define_gsubr("pylong-as-long", 1, 0, 0, PyLong_AsLongLong_wrapper);
  scm_c_define_gsubr("pyfloat-from-double", 1, 0, 0, PyFloat_FromDouble_wrapper);
  scm_c_define_gsubr("pyfloat-as-double", 1, 0, 0, PyFloat_AsDouble_wrapper);
  scm_c_define_gsubr("py-compile-string", 1, 2, 0, Py_CompileString_wrapper);
  scm_c_define_gsubr("pyeval-eval-code", 3, 0, 0, PyEval_EvalCode_wrapper);
  scm_c_define_gsubr("pyeval-get-builtins", 0, 0, 0, PyEval_GetBuiltins_wrapper);  
  scm_c_define_gsubr("pydict-new", 0, 0, 0, PyDict_New_wrapper);
  scm_c_define_gsubr("pydict-copy", 1, 0, 0, PyDict_Copy_wrapper);
  scm_c_define_gsubr("pydict-set-item-string", 3, 0, 0, PyDict_SetItemString_wrapper);
  scm_c_define_gsubr("py-finalize", 0, 0, 0, Py_Finalize_wrapper);

  scm_c_define_gsubr("py-incref", 1, 0, 0, Py_INCREF_wrapper);
  scm_c_define_gsubr("pytuple-new", 1, 0, 0, PyTuple_New_wrapper);
  scm_c_define_gsubr("pytuple-set-item", 3, 0, 0, PyTuple_SetItem_wrapper);
  scm_c_define_gsubr("pytuple-get-item", 2, 0, 0, PyTuple_GetItem_wrapper);
}
                      

void init_python()
{
  init_global_lock();
  export_types();
  export_constants();
  export_functions();
}
