#include "py_envoy_stream.h"

#include "library/common/main_interface.h"
#include "py_envoy_engine.h"
#include "py_envoy_http_callbacks.h"


PyObject *PyStreamObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  PyStreamObject *self;
  self = (PyStreamObject *)type->tp_alloc(type, 0);
  if (self != nullptr) {
    self->stream = 0;
  }
  return (PyObject *)self;
}

int PyStreamObject_init(PyStreamObject *self, PyObject *args, PyObject *kwargs) {
  char *kwlist[] = {"engine", nullptr};
  PyEngineObject *engine;

  if (
      !PyArg_ParseTupleAndKeywords(
         args,
         kwargs,
         "O",
         kwlist,
         &engine
      )
  ) {
    return -1;
  }

  self->stream = init_stream(engine->engine);
  return 0;
}

PyObject *PyStreamObject_start(PyStreamObject *self, PyObject *args) {
  PyHttpCallbacksObject *callbacks;
  if (!PyArg_ParseTuple(args, "O", &callbacks)) {
    return nullptr;
  }

  envoy_status_t status = start_stream(self->stream, callbacks->http_callbacks);
  if (status == kEnvoyFailure) {
    PyErr_SetString(PyExc_RuntimeError, "failed to start stream");
    return nullptr;
  }

  Py_INCREF(Py_None);
  return Py_None;
}

PyObject *PyStreamObject_send_headers(PyStreamObject *self, PyObject *args) {
  PyHeadersObject *headers;
  bool close;
  if (!PyArg_ParseTuple(args, "Op:send_headers", &headers, &close)) {
    return nullptr;
  }

  envoy_status_t status = send_headers(self->stream, headers->headers, close);
  if (status == kEnvoyFailure) {
    PyErr_SetString(PyExc_RuntimeError, "failed to send headers");
    return nullptr;
  }

  Py_INCREF(Py_None);
  return Py_None;
}

PyObject *PyStreamObject_send_data(PyStreamObject *self, PyObject *args) {
  return (PyObject *)self;
}

PyObject *PyStreamObject_send_metadata(PyStreamObject *self, PyObject *args) {
  return (PyObject *)self;
}

PyObject *PyStreamObject_send_trailers(PyStreamObject *self, PyObject *args) {
  return (PyObject *)self;
}

PyObject *PyStreamObject_reset(PyStreamObject *self, PyObject *args) {
  return (PyObject *)self;
}

PyObject *PyStreamObject_close(PyStreamObject *self, PyObject *args) {
  return (PyObject *)self;
}
