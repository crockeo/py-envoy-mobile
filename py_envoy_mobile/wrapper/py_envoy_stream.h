#pragma once

#include "Python.h"
#include "library/common/types/c_types.h"


struct PyStreamObject {
  PyObject_HEAD
  envoy_stream_t stream;
};

PyObject *PyStreamObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs);
int PyStreamObject_init(PyStreamObject *self, PyObject *args, PyObject *kwargs);

PyObject *PyStreamObject_start(PyStreamObject *self, PyObject *args);
PyObject *PyStreamObject_send_headers(PyStreamObject *self, PyObject *args);
PyObject *PyStreamObject_send_data(PyStreamObject *self, PyObject *args);
PyObject *PyStreamObject_send_metadata(PyStreamObject *self, PyObject *args);
PyObject *PyStreamObject_send_trailers(PyStreamObject *self, PyObject *args);
PyObject *PyStreamObject_reset(PyStreamObject *self, PyObject *args);
PyObject *PyStreamObject_close(PyStreamObject *self, PyObject *args);

static PyMethodDef PyStreamObject_methods[] = {
  {
    "start",
    (PyCFunction)PyStreamObject_start,
    METH_VARARGS,
    nullptr,
  },
  {
    "send_headers",
    (PyCFunction)PyStreamObject_send_headers,
    METH_VARARGS,
    nullptr,
  },
  {
    "send_data",
    (PyCFunction)PyStreamObject_send_data,
    METH_VARARGS,
    nullptr,
  },
  {
    "send_metadata",
    (PyCFunction)PyStreamObject_send_metadata,
    METH_VARARGS,
    nullptr,
  },
  {
    "send_trailers",
    (PyCFunction)PyStreamObject_send_trailers,
    METH_VARARGS,
    nullptr,
  },
  {
    "reset",
    (PyCFunction)PyStreamObject_reset,
    METH_VARARGS,
    nullptr,
  },
  {
    "close",
    (PyCFunction)PyStreamObject_close,
    METH_VARARGS,
    nullptr,
  },
  {nullptr},
};

static PyTypeObject PyStreamType = {
  PyVarObject_HEAD_INIT(nullptr, 0)

  .tp_name = "c_types_wrapper.Stream",
  .tp_doc = "envoy-mobile stream handle",
  .tp_basicsize = sizeof(PyStreamObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyStreamObject_new,
  .tp_init = (initproc)PyStreamObject_init,
  .tp_methods = PyStreamObject_methods,
};
