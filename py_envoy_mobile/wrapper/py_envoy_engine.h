#pragma once

#include "library/common/types/c_types.h"
#include "Python.h"


struct PyEngineObject {
  PyObject_HEAD
  envoy_engine_t engine;
};

PyObject *PyEngineObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs);
int PyEngineObject_init(PyEngineObject *self, PyObject *args, PyObject *kwargs);

PyObject *PyEngineObject_run(PyEngineObject *self, PyObject *args);
PyObject *PyEngineObject_terminate(PyEngineObject *self);

PyObject *PyEngineObject_record_counter(PyEngineObject *self, PyObject *args);
PyObject *PyEngineObject_gauge_set(PyEngineObject *self, PyObject *args);
PyObject *PyEngineObject_gauge_add(PyEngineObject *self, PyObject *args);
PyObject *PyEngineObject_gauge_sub(PyEngineObject *self, PyObject *args);

static PyMethodDef PyEngineObject_methods[] = {
  {
    "run",
    (PyCFunction)PyEngineObject_run,
    METH_VARARGS,
    nullptr,
  },
  {
    "terminate",
    (PyCFunction)PyEngineObject_terminate,
    METH_NOARGS,
    nullptr,
  },
  {
    "record_counter",
    (PyCFunction)PyEngineObject_record_counter,
    METH_VARARGS,
    nullptr,
  },
  {
    "gauge_set",
    (PyCFunction)PyEngineObject_gauge_set,
    METH_VARARGS,
    nullptr,
  },
  {
    "gauge_add",
    (PyCFunction)PyEngineObject_gauge_add,
    METH_VARARGS,
    nullptr,
  },
  {
    "gauge_sub",
    (PyCFunction)PyEngineObject_gauge_sub,
    METH_VARARGS,
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
