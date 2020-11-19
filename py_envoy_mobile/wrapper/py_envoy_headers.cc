#include "py_envoy_headers.h"


PyObject *PyHeadersObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  PyHeadersObject *self;
  self = (PyHeadersObject *)type->tp_alloc(type, 0);
  if (self != nullptr) {
    self->headers.length = 0;
    self->headers.headers = (envoy_header *)safe_malloc(sizeof(envoy_header));
    self->capacity = 1;
  }
  return (PyObject *)self;
}

void PyHeadersObject_dealloc(PyHeadersObject *self) {
  release_envoy_headers(self->headers);
  Py_TYPE(self)->tp_free((PyObject *)self);
}

PyObject *PyHeadersObject_set_header(PyHeadersObject *self, PyObject *args) {
  PyEnvoyDataObject *name;
  PyEnvoyDataObject *value;

  if (PyArg_ParseTuple(args, "OO", &name, &value) < 0) {
    Py_INCREF(self);
    return (PyObject *)self;
  }

  envoy_header header = {
    copy_envoy_data(name->data.length, name->data.bytes),
    copy_envoy_data(name->data.length, name->data.bytes),
  };

  // NOTE: currently we assume that a header with the same value is never set twice. i.e. we just
  // append the envoy_header to the list, rather than trying to replace its existing value.
  if (self->headers.length >= self->capacity) {
    envoy_header *newHeaders = (envoy_header *)safe_malloc(sizeof(envoy_header) * 2 * self->capacity);
    memcpy(newHeaders, self->headers.headers, sizeof(envoy_header) * self->headers.length);
    free(self->headers.headers);
    self->headers.headers = newHeaders;
  }

  self->headers.headers[self->headers.length] = header;
  self->headers.length++;

  Py_INCREF(self);
  return (PyObject *)self;
}
