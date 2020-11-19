#pragma once

#include "library/common/types/c_types.h"
#include "Python.h"


struct PyEngineCallbacksObject {
  PyObject_HEAD
  envoy_engine_callbacks engine_callbacks;

  PyObject *on_engine_running;
  PyObject *on_exit;
};

PyObject *PyEngineCallbacks_new(PyTypeObject *type, PyObject *args, PyObject *kwargs);
int PyEngineCallbacks_init(PyEngineCallbacksObject *self, PyObject *args, PyObject *kwargs);

PyObject *PyEngineCallbacks_set_on_engine_running(PyEngineCallbacksObject *self, PyObject *args);
PyObject *PyEngineCallbacks_set_on_exit(PyEngineCallbacksObject *self, PyObject *args);

static PyMethodDef PyEngineCallbacks_methods[] = {
  {
    "set_on_engine_running",
    (PyCFunction)PyEngineCallbacks_set_on_engine_running,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_exit",
    (PyCFunction)PyEngineCallbacks_set_on_exit,
    METH_VARARGS,
    nullptr,
  },
  {nullptr},
};

static PyTypeObject PyEngineCallbacksType = {
  PyVarObject_HEAD_INIT(nullptr, 0)

  .tp_name = "c_types_wrapper.EngineCallbacks",
  .tp_doc = "envoy-mobile engine lifecycle callbacks",
  .tp_basicsize = sizeof(PyEngineCallbacksObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyEngineCallbacks_new,
  .tp_init = (initproc)PyEngineCallbacks_init,
  .tp_methods = PyEngineCallbacks_methods,
};
