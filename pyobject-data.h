#ifndef PYOBJECT_DATA_H
#define PYOBJECT_DATA_H
#include <Python.h>
#include <libguile.h>

struct PyObject_data {
  PyObject *object;
  SCM name;
  SCM update_func;
};

extern SCM PyObject_type;

#endif
