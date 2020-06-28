#define Py_LIMITED_API 0x03020000
#include <Python.h>
#include <libguile.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <pthread.h>

#define ACQUIRE_PYTHON_LOCK() pthread_mutex_lock(&python_lock)
#define RELEASE_PYTHON_LOCK() pthread_mutex_unlock(&python_lock)
#define WITH_PYTHON_LOCK(code)       \
  pthread_mutex_lock(&python_lock);  \
  {                                  \
     code;                           \
  }                                  \
  pthread_mutex_unlock(&python_lock);

static pthread_mutex_t python_lock;

static SCM PyObject_type;

struct PyObject_data {
  PyObject *object;
  SCM name;
  SCM update_func;
};

static void finalize_PyObject (SCM pyobject)
{
  struct PyObject_data *pyobject_data = scm_foreign_object_ref(pyobject, 0);
  ACQUIRE_PYTHON_LOCK();
  PyObject *object = pyobject_data->object;
  //  fprintf(stderr, "Internal finalization of object with %ld ref counts\n",  object->ob_refcnt);
  Py_XDECREF(object);
  RELEASE_PYTHON_LOCK();
} 

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
    // TODO/FIXME extract, generalize
    scm_error_scm (scm_from_locale_symbol ("value error"),
                   scm_from_utf8_string ("PyLong_FromLongLong"),
                   scm_from_utf8_string ("Invalid value provided"),
                   SCM_UNSPECIFIED,
                   scm_list_1 (scm_from_int (1)));
  } else {
    long long int_value = scm_to_signed_integer(value, LONG_MIN, LONG_MAX);
    PyObject *py_value;
    WITH_PYTHON_LOCK(py_value = PyLong_FromLongLong(int_value));

    struct PyObject_data *pyobject_data  = (struct PyObject_data *) scm_gc_malloc (sizeof (struct PyObject_data), "PyLong");
    pyobject_data->object = py_value;
    
    // TODO/FIXME Python garbage collection completely missing! I need probably some kind of thread guard
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

void init_python()
{
  pthread_mutex_init(&python_lock, NULL);
  PyObject_type = scm_make_foreign_object_type(scm_from_utf8_symbol("PyObject"),
                                               scm_list_1(scm_from_utf8_symbol("pointer")),
                                               finalize_PyObject);
  scm_c_define_gsubr("py-initialize", 0, 0, 0, Py_Initialize_wrapper);
  scm_c_define_gsubr("pylong-from-long", 1, 0, 0, PyLong_FromLongLong_wrapper);
  scm_c_define_gsubr("pylong-as-long", 1, 0, 0, PyLong_AsLongLong_wrapper);
  scm_c_define_gsubr("py-finalize", 0, 0, 0, Py_Finalize_wrapper);
}
