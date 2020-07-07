#include "python-guile-utils.h"
#include "pyobject-data.h"

void finalize_PyObject (SCM pyobject)
{
  struct PyObject_data *pyobject_data = scm_foreign_object_ref(pyobject, 0);
  global_lock();
  PyObject *object = pyobject_data->object;
  Py_XDECREF(object);
  global_unlock();
} 

SCM raise_error(const char *subroutine, const char *message) {
  return scm_error_scm(scm_from_locale_symbol("misc-error"),
                       scm_from_utf8_string(subroutine),
                       scm_from_utf8_string(message),
                       scm_list_1(scm_from_int(1)),
                       SCM_BOOL_F);
}

SCM create_python_scm(PyObject *py_object, const char *object_name)
{
  struct PyObject_data *pyobject_data  = (struct PyObject_data *) scm_gc_malloc (sizeof (struct PyObject_data), object_name);
  pyobject_data->object = py_object;
  return scm_make_foreign_object_1(PyObject_type, pyobject_data);  
}

int get_optional_int(SCM optional_int, int default_value)
{
  return scm_is_eq(optional_int, SCM_UNDEFINED) ? default_value : scm_to_signed_integer(optional_int, LONG_MIN, LONG_MAX);
}

char *get_optional_allocated_string(SCM optional_string, char *default_string)
{
  return scm_is_eq(optional_string, SCM_UNDEFINED) ? strdup(default_string) : scm_to_utf8_stringn(optional_string, NULL);
}

// TODO/FIXME this seems oddly convoluted
SCM create_empty_list()
{
  return scm_list_n(SCM_UNDEFINED);
}

long long convert_to_longlong(SCM value)
{
  return scm_to_long_long(value);
}

SCM pyobject_type_p(SCM object)
{
  // thin ice: this macro is defined in guile.h, but it's not in the infodoc…
  return SCM_IS_A_P(object, PyObject_type)? SCM_BOOL_T : SCM_BOOL_F;
}

PyObject* convert_to_pyobject(SCM object)
{
  struct PyObject_data *pyobject_data = scm_foreign_object_ref(object, 0);
  return pyobject_data->object;
}

SCM checked_pyobject_to_scheme(PyObject* pyObject)
{
  if(pyObject == NULL) {
    return create_empty_list();
  } else {
    return create_python_scm(pyObject, "PyObject");
  }
}
