#include "py_envoy_http_callbacks.h"

#include "py_envoy_headers.h"


// since we can't do dynamic dispatch natively in C++ (closures can't be function pointers), we need
// these static functions that dispatch on the value contained in the context, which is just a
// refernce to the PyHttpCallbacksObject.
void *dispatchOnHeaders(envoy_headers headers, bool end_stream, void *context) {
  auto self = (PyHttpCallbacksObject *)context;
  if (self->on_headers == Py_None) {
    return context;
  }

  PyHeadersObject py_headers;
  py_headers.headers = headers;
  py_headers.capacity = headers.length;

  PyObject *arglist = Py_BuildValue("(Op)", &py_headers, end_stream);
  PyObject *result;

  result = PyObject_CallObject(self->on_headers, arglist);
  Py_DECREF(arglist);
  if (result == nullptr) {
    // TODO: what happens when we're not directly in a python context?
  }

  return context;
}

void *dispatchOnData(envoy_data data, bool end_stream, void *context) {
  auto self = (PyHttpCallbacksObject *)context;
  if (self->on_data == Py_None) {
    return context;
  }

  return context;
}

void *dispatchOnMetadata(envoy_headers headers, void *context) {
  return context;
}

void *dispatchOnTrailers(envoy_headers trailers, void *context) {
  return context;
}

void *dispatchOnError(envoy_error error, void *context) {
  // TODO: this one has to wait until we have an envoy_error wrapper
  return context;
}

void *dispatchOnComplete(void *context) {
  return context;
}

void *dispatchOnCancel(void *context) {
  return context;
}


PyObject *PyHttpCallbacksObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  PyHttpCallbacksObject *self;
  self = (PyHttpCallbacksObject *)type->tp_alloc(type, 0);
  if (self != nullptr) {
    self->http_callbacks.on_headers = &dispatchOnHeaders;
    self->http_callbacks.on_data = &dispatchOnData;
    self->http_callbacks.on_metadata = &dispatchOnMetadata;
    self->http_callbacks.on_trailers = &dispatchOnTrailers;
    self->http_callbacks.on_error = &dispatchOnError;
    self->http_callbacks.on_complete = &dispatchOnComplete;
    self->http_callbacks.on_cancel = &dispatchOnCancel;
    self->http_callbacks.context = (void *)self;

    self->on_headers = nullptr;
    self->on_data = nullptr;
    self->on_metadata = nullptr;
    self->on_trailerrs = nullptr;
    self->on_error = nullptr;
    self->on_complete = nullptr;
    self->on_complete = nullptr;
    self->on_cancel = nullptr;
  }
  return (PyObject *)self;
}

PyObject *PyHttpCallbacksObject_init(PyHttpCallbacksObject *self, PyObject *args, PyObject *kwargs) {
  for (int i = 0; i < 8; i++) {
    Py_INCREF(Py_None);
  }
  self->on_headers = Py_None;
  self->on_data = Py_None;
  self->on_metadata = Py_None;
  self->on_trailerrs = Py_None;
  self->on_error = Py_None;
  self->on_complete = Py_None;
  self->on_complete = Py_None;
  self->on_cancel = Py_None;

  Py_INCREF(self);
  return (PyObject *)self;
}

PyObject *PyHttpCallbacksObject_set_on_headers(PyHttpCallbacksObject *self, PyObject *args) {
  PyObject *temp;
  if (!PyArg_ParseTuple(args, "O", &temp)) {
    return nullptr;
  }

  if (!PyCallable_Check(temp)) {
    PyErr_SetString(PyExc_TypeError, "parameter must be callable");
    return nullptr;
  }

  Py_XINCREF(temp);
  Py_XDECREF(self->on_headers);
  self->on_headers = temp;

  Py_INCREF(self);
  return (PyObject *)self;
}

PyObject *PyHttpCallbacksObject_set_on_data(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}

PyObject *PyHttpCallbacksObject_set_on_metadata(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}

PyObject *PyHttpCallbacksObject_set_on_trailers(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}

PyObject *PyHttpCallbacksObject_set_on_error(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}

PyObject *PyHttpCallbacksObject_set_on_complete(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}

PyObject *PyHttpCallbacksObject_set_on_cancel(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}
