#define Py_LIMITED_API 0x03020000
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "python-guile-utils.h"

static SCM Py_Initialize_wrapper(SCM unused)
{
  WITH_PYTHON_LOCK(Py_Initialize());
  return SCM_UNSPECIFIED;
}

static SCM Py_Finalize_wrapper(SCM unused)
{
  WITH_PYTHON_LOCK(Py_Finalize())
  return SCM_UNSPECIFIED;
}

static SCM PyLong_FromLongLong_wrapper(SCM value)
{
  if(!scm_integer_p(value)) {
    return raise_error("PyLong_FromLongLong", "Invalid value provided");
  } else {
    long long int_value = scm_to_signed_integer(value, LONG_MIN, LONG_MAX);
    PyObject *py_value;
    WITH_PYTHON_LOCK(py_value = PyLong_FromLongLong(int_value));

    struct PyObject_data *pyobject_data  = (struct PyObject_data *) scm_gc_malloc (sizeof (struct PyObject_data), "PyLong");
    pyobject_data->object = py_value;
    
    return scm_make_foreign_object_1(PyObject_type, pyobject_data);
  }
}

static SCM PyLong_AsLongLong_wrapper(SCM value)
{
  scm_assert_foreign_object_type(PyObject_type, value);
  struct PyObject_data *pyobject_data = scm_foreign_object_ref(value, 0);
  long long int_value;
  WITH_PYTHON_LOCK(int_value = PyLong_AsLongLong(pyobject_data->object))
  return scm_from_signed_integer(int_value);
}

static SCM PyFloat_FromDouble_wrapper(SCM value)
{
  if(!scm_real_p(value)) {
    return raise_error("PyFloat_FromDouble", "Invalid value provided");
  } else {
    double double_value = scm_to_double(value);
    PyObject *py_value;
    WITH_PYTHON_LOCK(py_value = PyFloat_FromDouble(double_value));

    return create_python_scm(py_value, "PyFloat");
  }
}

static SCM PyFloat_AsDouble_wrapper(SCM value)
{
  scm_assert_foreign_object_type(PyObject_type, value);
  struct PyObject_data *pyobject_data = scm_foreign_object_ref(value, 0);
  double double_value;
  WITH_PYTHON_LOCK(double_value = PyFloat_AsDouble(pyobject_data->object))
  return scm_from_double(double_value);
}

static SCM Py_CompileString_wrapper(SCM scm_script, SCM scm_file, SCM scm_start)
{
  int start = get_optional_int(scm_start, Py_file_input);
  char *file = get_optional_allocated_string(scm_file, "<file>");
  char *script = scm_to_utf8_stringn(scm_script, NULL);

  PyObject *py_object;
  WITH_PYTHON_LOCK(py_object = Py_CompileString(script, file, start));

  if(py_object == NULL) {
    return raise_error("Py_CompileString", "NULL returned");
  } else {
    return create_python_scm(py_object, "PyCompiledCode");
  } 
}

static SCM PyDict_New_wrapper()
{
  PyObject *py_dict;
  WITH_PYTHON_LOCK(py_dict = PyDict_New());

  if(py_dict == NULL) {
    return raise_error("PyDict_New", "NULL returned");
  } else {
    return create_python_scm(py_dict, "PyDict");
  }
}

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
  scm_c_define_gsubr("pydict-new", 0, 0, 0, PyDict_New_wrapper);
  scm_c_define_gsubr("py-finalize", 0, 0, 0, Py_Finalize_wrapper);
}
                      

void init_python()
{
  init_global_lock();
  export_types();
  export_constants();
  export_functions();
}
