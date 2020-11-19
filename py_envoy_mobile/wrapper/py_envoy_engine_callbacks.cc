#include "py_envoy_engine_callbacks.h"

#include <exception>


void dispatchOnEngineRunning(void *context) {
  auto self = (PyEngineCallbacksObject *)context;
  if (self->on_engine_running == Py_None) {
    return;
  }

  auto result = PyObject_CallObject(self->on_engine_running, nullptr);
  if (result == nullptr) {
    // TODO: figure out how to throw python error in this thread
    throw std::runtime_error("failed to call on_engine_running");
  }
}

void dispatchOnExit(void *context) {
  auto self = (PyEngineCallbacksObject *)context;
  if (self->on_exit == Py_None) {
    return;
  }

  auto result = PyObject_CallObject(self->on_exit, nullptr);
  if (result == nullptr) {
    // TODO: figure out how to throw error in this thread
    throw std::runtime_error("failed to call on_exit");
  }
}


PyObject *PyEngineCallbacks_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  PyEngineCallbacksObject *self;
  self = (PyEngineCallbacksObject *)type->tp_alloc(type, 0);
  if (self != nullptr) {
    self->engine_callbacks.on_engine_running = &dispatchOnEngineRunning;
    self->engine_callbacks.on_exit = &dispatchOnExit;
    self->engine_callbacks.context = self;

    self->on_engine_running = nullptr;
    self->on_exit = nullptr;
  }
  return (PyObject *)self;
}

int PyEngineCallbacks_init(PyEngineCallbacksObject *self, PyObject *args, PyObject *kwargs) {
  for (int i = 0; i < 2; i++) {
    Py_INCREF(Py_None);
  }
  self->on_engine_running = Py_None;
  self->on_exit = Py_None;

  return 0;
}

PyObject *PyEngineCallbacks_set_on_engine_running(PyEngineCallbacksObject *self, PyObject *args) {
  PyObject *temp;
  if (!PyArg_ParseTuple(args, "O", &temp)) {
    return nullptr;
  }

  if (!PyCallable_Check(temp)) {
    PyErr_SetString(PyExc_TypeError, "parameter must be callable");
    return nullptr;
  }

  Py_XINCREF(temp);
  Py_XDECREF(self->on_engine_running);
  self->on_engine_running = temp;

  Py_INCREF(self);
  return (PyObject *)self;
}

PyObject *PyEngineCallbacks_set_on_exit(PyEngineCallbacksObject *self, PyObject *args) {
  PyObject *temp;
  if (!PyArg_ParseTuple(args, "O", &temp)) {
    return nullptr;
  }

  if (!PyCallable_Check(temp)) {
    PyErr_SetString(PyExc_TypeError, "parameter must be callable");
    return nullptr;
  }

  Py_XINCREF(temp);
  Py_XDECREF(self->on_exit);
  self->on_exit = temp;

  Py_INCREF(self);
  return (PyObject *)self;
}
