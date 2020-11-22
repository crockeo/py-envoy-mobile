#include "py_envoy_headers.h"

#include "library/common/types/c_types.h"


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
  uint8_t *name_data, *value_data;
  Py_ssize_t name_len, value_len;

  if (!PyArg_ParseTuple(args, "s#s#:set_header", &name_data, &name_len, &value_data, &value_len)) {
    return nullptr;
  }

  // NOTE: currently we assume that a header with the same value is never set twice. i.e. we just
  // append the envoy_header to the list, rather than trying to replace its existing value.
  if (self->headers.length >= self->capacity) {
    auto old_length = sizeof(envoy_header) * self->capacity;
    auto new_length = old_length * 2;

    envoy_header *new_headers = (envoy_header *)safe_malloc(new_length);
    memcpy(new_headers, self->headers.headers, old_length);

    free(self->headers.headers);
    self->headers.headers = new_headers;

    self->capacity *= 2;
  }

  envoy_header header = {
    copy_envoy_data(name_len, name_data),
    copy_envoy_data(value_len, value_data),
  };

  self->headers.headers[self->headers.length] = header;
  self->headers.length++;

  Py_INCREF(self);
  return (PyObject *)self;
}
