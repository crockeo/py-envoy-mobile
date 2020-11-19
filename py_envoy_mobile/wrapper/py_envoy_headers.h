#pragma once

#include "py_envoy_data.h"
#include "Python.h"
#include "library/common/types/c_types.h"


struct PyHeadersObject {
  PyObject_HEAD
  envoy_headers headers;
  size_t capacity;
};

PyObject *PyHeadersObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs);

void PyHeadersObject_dealloc(PyHeadersObject *self);

PyObject *PyHeadersObject_set_header(PyHeadersObject *self, PyObject *args);

static PyMethodDef PyHeadersObject_methods[] = {
  {
    "set_header",
    (PyCFunction)PyHeadersObject_set_header,
    METH_VARARGS,
    nullptr,
  },
  {nullptr},
};

static PyTypeObject PyHeadersType = {
  PyVarObject_HEAD_INIT(nullptr, 0)

  .tp_name = "c_types_wrapper.Headers",
  .tp_doc = "",
  .tp_basicsize = sizeof(PyHeadersObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyHeadersObject_new,
  .tp_dealloc = (destructor)PyHeadersObject_dealloc,
  .tp_methods = PyHeadersObject_methods,
};
