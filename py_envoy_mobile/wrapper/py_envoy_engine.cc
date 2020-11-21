#include "py_envoy_engine.h"

#include "library/common/main_interface.h"
#include "library/common/types/c_types.h"
#include "py_envoy_engine_callbacks.h"


PyObject *PyEngineObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  PyEngineObject *self;
  self = (PyEngineObject *)type->tp_alloc(type, 0);
  if (self != nullptr) {
    self->engine = 0;
  }
  return (PyObject *)self;
}

int PyEngineObject_init(PyEngineObject *self, PyObject *args, PyObject *kwargs) {
  envoy_engine_t engine = init_engine();
  if (engine == 0) {
    return -1;
  }

  self->engine = engine;
  return 0;
}

PyObject *PyEngineObject_run(PyEngineObject *self, PyObject *args) {
  PyEngineCallbacksObject *callbacks;
  char *config;
  char *log_level;
  if (!PyArg_ParseTuple(args, "Oss", &callbacks, &config, &log_level)) {
    return nullptr;
  }

  envoy_status_t status = run_engine(self->engine, callbacks->engine_callbacks, config, log_level);
  if (status == ENVOY_FAILURE) {
    PyErr_SetString(PyExc_RuntimeError, "failed to run engine");
    return nullptr;
  }

  Py_INCREF(Py_None);
  return Py_None;
}

PyObject *PyEngineObject_terminate(PyEngineObject *self) {
  terminate_engine(self->engine);
  Py_INCREF(Py_None);
  return Py_None;
}

PyObject *PyEngineObject_record_counter(PyEngineObject *self, PyObject *args) {
  PyUnicodeObject *py_str;
  uint64_t count;
  if (!PyArg_ParseTuple(args, "sI", &py_str, &count)) {
    return nullptr;
  }

  const char *str = PyUnicode_As_DATA(py_str);
  if (str == nullptr) {
    return nullptr;
  }

  auto status = record_counter(self->engine, str, count);
  if (status == ENVOY_FAILURE) {
    PyErr_SetStr(PyExc_RuntimeError, "failed to record counter");
    return nullptr;
  }

  Py_INCREF(Py_None);
  return Py_None;
}

PyObject *PyEngineObject_gauge_set(PyEngineObject *self, PyObject *args) {
  PyUnicodeObject *py_str;
  uint64_t value;
  if (!PyArg_ParseTuple(args, "sI", &py_str, &value)) {
    return nullptr;
  }

  const char *str = PyUnicode_As_DATA(py_str);
  if (str == nullptr) {
    return nullptr;
  }

  auto status = gauge_set(self->engine, str, value);
  if (status == ENVOY_FAILURE) {
    PyErr_SetStr(PyExc_RuntimeError, "failed to set gauge");
    return nullptr;
  }

  Py_INCREF(Py_None);
  return Py_None;
}

PyObject *PyEngineObject_gauge_add(PyEngineObject *self, PyObject *args) {
  PyUnicodeObject *py_str;
  uint64_t amount;
  if (!PyArg_ParseTuple(args, "sI", &py_str, &amount)) {
    return nullptr;
  }

  const char *str = PyUnicode_As_DATA(py_str);
  if (str == nullptr) {
    return nullptr;
  }

  auto status = gauge_add(self->engine, str, amount);
  if (status == ENVOY_FAILURE) {
    PyErr_SetStr(PyExc_RuntimeError, "failed to add to gauge");
    return nullptr;
  }

  Py_INCREF(Py_None);
  return Py_None;
}

PyObject *PyEngineObject_gauge_sub(PyEngineObject *self, PyObject *args) {
  PyUnicodeObject *py_str;
  uint64_t amount;
  if (!PyArg_ParseTuple(args, "sI", &py_str, &amount)) {
    return nullptr;
  }

  const char *str = PyUnicode_As_DATA(py_str);
  if (str == nullptr) {
    return nullptr;
  }

  auto status = gauge_sub(self->engine, str, amount);
  if (status == ENVOY_FAILURE) {
    PyErr_SetStr(PyExc_RuntimeError, "failed to sub from gauge");
    return nullptr;
  }

  Py_INCREF(Py_None);
  return Py_None;
}
