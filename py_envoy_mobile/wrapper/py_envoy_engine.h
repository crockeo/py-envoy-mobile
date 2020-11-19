#pragma once

#include "Python.h"
#include "library/common/types/c_types.h"


struct PyEngineObject {
  PyObject_HEAD
  envoy_engine_t engine;
};

PyObject *PyEngineObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs);
int PyEngineObject_init(PyEngineObject *self, PyObject *args, PyObject *kwargs);

PyObject *PyEngineObject_terminate(PyEngineObject *self);

static PyMethodDef PyEngineObject_methods[] = {
  {
    "terminate",
    (PyCFunction)PyEngineObject_terminate,
    METH_NOARGS,
    nullptr,
  },
  {nullptr},
};

static PyTypeObject PyEngineType = {
  PyVarObject_HEAD_INIT(nullptr, 0)

  .tp_name = "c_types_wrapper.Engine",
  .tp_doc = "envoy-mobile engine handle",
  .tp_basicsize = sizeof(PyEngineObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyEngineObject_new,
  .tp_init = (initproc)PyEngineObject_init,
  .tp_methods = PyEngineObject_methods,
};
