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
  char *script = scm_to_utf8_stringn(scm_script, NULL); // TODO/FIXME memory leak galore, everywhere

  PyObject *py_object;
  WITH_PYTHON_LOCK(py_object = Py_CompileString(script, file, start));

  if(py_object == NULL) {
    return create_empty_list();
  } else {
    return create_python_scm(py_object, "PyCompiledCode");
  } 
}

static SCM PyDict_New_wrapper()
{
  PyObject *py_dict;
  WITH_PYTHON_LOCK(py_dict = PyDict_New());

  if(py_dict == NULL) {
    return create_empty_list();
  } else {
    return create_python_scm(py_dict, "PyDict");
  }
}

static SCM PyDict_Copy_wrapper(SCM scm_dict)
{
  scm_assert_foreign_object_type(PyObject_type, scm_dict);
  struct PyObject_data *pyobject_data = scm_foreign_object_ref(scm_dict, 0);

  PyObject *new_py_dict;
  WITH_PYTHON_LOCK(new_py_dict = PyDict_Copy(pyobject_data->object));

  if(new_py_dict == NULL) {
    return create_empty_list();
  } else {
    return create_python_scm(new_py_dict, "PyDict");
  }
}

static SCM PyEval_EvalCode_wrapper(SCM code, SCM globals, SCM locals)
{
  scm_assert_foreign_object_type(PyObject_type, code);
  struct PyObject_data *pycode_data = scm_foreign_object_ref(code, 0);
  scm_assert_foreign_object_type(PyObject_type, globals);
  struct PyObject_data *pyglobals_data = scm_foreign_object_ref(globals, 0);
  scm_assert_foreign_object_type(PyObject_type, locals);
  struct PyObject_data *pylocals_data = scm_foreign_object_ref(locals, 0);
  
  PyObject *result;
  WITH_PYTHON_LOCK(result = PyEval_EvalCode(pycode_data->object, pyglobals_data->object, pylocals_data->object));

  if(result == NULL) {
    return raise_error("PyEval_EvalCode", "NULL retuned");
  } else {
    return create_python_scm(result, "PyDict");
  }
}

static SCM PyEval_GetBuiltins_wrapper()
{
  PyObject *result;
  WITH_PYTHON_LOCK(result = PyEval_GetBuiltins());
  return create_python_scm(result, "PyDict");  
}

static SCM PyDict_SetItemString_wrapper(SCM dict, SCM key, SCM value)
{
  scm_assert_foreign_object_type(PyObject_type, dict);
  struct PyObject_data *pydict_data = scm_foreign_object_ref(dict, 0);
  
  char *c_key = scm_to_utf8_stringn(key, NULL);

  scm_assert_foreign_object_type(PyObject_type, value);
  struct PyObject_data *pyvalue_data = scm_foreign_object_ref(value, 0);

  int result;
  WITH_PYTHON_LOCK(result = PyDict_SetItemString(pydict_data->object, c_key, pyvalue_data->object));

  return scm_from_signed_integer(result);
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
  scm_c_define_gsubr("pyeval-eval-code", 3, 0, 0, PyEval_EvalCode_wrapper);
  scm_c_define_gsubr("pyeval-get-builtins", 0, 0, 0, PyEval_GetBuiltins_wrapper);  
  scm_c_define_gsubr("pydict-new", 0, 0, 0, PyDict_New_wrapper);
  scm_c_define_gsubr("pydict-copy", 1, 0, 0, PyDict_Copy_wrapper);
  scm_c_define_gsubr("pydict-set-item-string", 3, 0, 0, PyDict_SetItemString_wrapper);
  scm_c_define_gsubr("py-finalize", 0, 0, 0, Py_Finalize_wrapper);
}
                      

void init_python()
{
  init_global_lock();
  export_types();
  export_constants();
  export_functions();
}
