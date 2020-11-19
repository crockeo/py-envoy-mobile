#pragma once

#include "Python.h"
#include "py_envoy_data.h"
#include "py_envoy_headers.h"
#include "library/common/types/c_types.h"


struct PyHttpCallbacksObject {
  PyObject_HEAD
  envoy_http_callbacks http_callbacks;

  PyObject *on_headers;
  PyObject *on_data;
  PyObject *on_metadata;
  PyObject *on_trailerrs;
  PyObject *on_error;
  PyObject *on_complete;
  PyObject *on_cancel;
};

PyObject *PyHttpCallbacksObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs);

PyObject *PyHttpCallbacksObject_init(PyHttpCallbacksObject *self, PyObject *args, PyObject *kwargs);

PyObject *PyHttpCallbacksObject_set_on_headers(PyHttpCallbacksObject *self, PyObject *args);

PyObject *PyHttpCallbacksObject_set_on_data(PyHttpCallbacksObject *self, PyObject *args);

PyObject *PyHttpCallbacksObject_set_on_metadata(PyHttpCallbacksObject *self, PyObject *args);

PyObject *PyHttpCallbacksObject_set_on_trailers(PyHttpCallbacksObject *self, PyObject *args);

PyObject *PyHttpCallbacksObject_set_on_error(PyHttpCallbacksObject *self, PyObject *args);

PyObject *PyHttpCallbacksObject_set_on_complete(PyHttpCallbacksObject *self, PyObject *args);

PyObject *PyHttpCallbacksObject_set_on_cancel(PyHttpCallbacksObject *self, PyObject *args);

static PyMethodDef PyHttpCallbacksObject_methods[] = {
  {
    "set_on_headers",
    (PyCFunction)PyHttpCallbacksObject_set_on_headers,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_data",
    (PyCFunction)PyHttpCallbacksObject_set_on_data,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_metadata",
    (PyCFunction)PyHttpCallbacksObject_set_on_metadata,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_trailers",
    (PyCFunction)PyHttpCallbacksObject_set_on_trailers,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_error",
    (PyCFunction)PyHttpCallbacksObject_set_on_error,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_complete",
    (PyCFunction)PyHttpCallbacksObject_set_on_complete,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_cancel",
    (PyCFunction)PyHttpCallbacksObject_set_on_cancel,
    METH_VARARGS,
    nullptr,
  },
  {nullptr},
};

static PyTypeObject PyHttpCallbacksType = {
  PyVarObject_HEAD_INIT(nullptr, 0)

  .tp_name = "c_types_wrapper.HttpCallbacks",
  .tp_doc = "",
  .tp_basicsize = sizeof(PyHttpCallbacksObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyHttpCallbacksObject_new,
  .tp_init = (initproc)PyHttpCallbacksObject_init,
  .tp_methods = PyHttpCallbacksObject_methods,
};
