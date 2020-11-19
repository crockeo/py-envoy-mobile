#pragma once

#include "Python.h"
#include "library/common/types/c_types.h"


struct PyEnvoyDataObject {
  PyObject_HEAD
  envoy_data data;
};

PyObject *PyEnvoyData_new(PyTypeObject *type, PyObject *args, PyObject *kwargs);
int PyEnvoyData_init(PyEnvoyDataObject *self, PyObject *args, PyObject *kwargs);
void PyEnvoyData_dealloc(PyEnvoyDataObject *self);

static PyTypeObject PyEnvoyDataType = {
  PyVarObject_HEAD_INIT(nullptr, 0)

  .tp_name = "c_types_wrapper.Data",
  .tp_doc = "",
  .tp_basicsize = sizeof(PyEnvoyDataObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyEnvoyData_new,
  .tp_init = (initproc)PyEnvoyData_init,
  .tp_dealloc = (destructor)PyEnvoyData_dealloc,
};
