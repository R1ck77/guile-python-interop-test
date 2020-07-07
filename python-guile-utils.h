#ifndef PYTHON_GUILE_UTILS_H
#define PYTHON_GUILE_UTILS_H
#include <Python.h>
#include <libguile.h>
#include "pyobject-data.h"
#include "python-locking.h"

void finalize_PyObject (SCM pyobject);

SCM raise_error(const char *subroutine, const char *message);

SCM create_python_scm(PyObject *py_object, const char *object_name);

int get_optional_int(SCM optional_int, int default_value);

char *get_optional_allocated_string(SCM optional_string, char *default_string);

SCM create_empty_list();

long long convert_to_longlong(SCM value);

SCM pyobject_type_p(SCM object);

PyObject* convert_to_pyobject(SCM object);

SCM checked_pyobject_to_scheme(PyObject* pyObject);

#endif
